#include <lib.h>
/*
标准输出
i am 1001
@@@@@send 3 from 1001 to 800
i am 800
@@@@@send 5 from 800 to 1001
800 received sig2.
1001 received sig5.
[00001001] destroying 00001001
[00001001] free env 00001001
i am killed ...
[00000800] destroying 00000800
[00000800] free env 00000800
i am killed ...
*/
const int PP1 = 0x800, PP2 = 0x1001;
void handler(int num){
    int envid=syscall_getenvid();
    debugf("%x received sig%d.\n",envid,num);
}
int main() {
  u_int me, who, i;
  me = syscall_getenvid();
  struct sigaction sig;
  sigset_t set;
  sigemptyset(&set);
  sig.sa_handler=handler;
  sig.sa_mask=set;
  sigaction(2,&sig,NULL);
  sigaction(3,&sig,NULL);
  sigaction(5,&sig,NULL);
  debugf("i am %x\n", me);
  if (me == PP1) {
    who = PP2;
    kill(who,5);
    debugf("@@@@@send 5 from %x to %x\n", me, who);
  } else if (me == PP2) {
    who = PP1;
    kill(who,3);
    debugf("@@@@@send 3 from %x to %x\n", me, who);
  } else {
    debugf("unexpected envid %x\n", me);
    syscall_panic("halt");
  }
  for (i=0;i<10000000;) {
    i++;
    if(((i%100)==0)){
      //syscall_yield();
    }
    if (i == 10000) {
      kill(PP1,2);
      // kill(PP2,11);
      // syscall_yield();
    }
  }
  return 0;
}
