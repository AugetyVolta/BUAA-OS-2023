#include <lib.h>
const int PP1 = 0x800, PP2 = 0x1001;
void handler(int num){
    debugf("into sighandler to %d.\n",num);
    int envid=syscall_getenvid();
    debugf("%x received sig%d.\n",envid,num);
}
int main() {
  u_int me, who, i=0;
  me = syscall_getenvid();
  struct sigaction sig;
  struct sigset_t set;
  struct sigset_t shed;
  sigemptyset(&set);
  sigemptyset(&shed);
  sigaddset(&shed, 2); //首先屏蔽2/3号
  sigaddset(&shed, 3);
  sig.sa_handler=handler;
  sig.sa_mask=set;
  sigaction(2,&sig,NULL); //2号被屏蔽，不应该优先处理。
  sigaction(3,&sig,NULL);//3号被屏蔽，不应该优先处理。
  sigaction(5,&sig,NULL);
  debugf("i am %x\n", me);
  sigprocmask(2, &shed, NULL);
  if (me == PP1) {
    who = PP2;
    kill(who, 2);
    kill(who, 3);
    kill(who, 5);
    debugf("@@@@PP1 sent 2,3,5 to PP2@@@@\n");
  }if(me == PP2){
    for(int i=0;i<1000000;i++);
  }
  return 0;
}
