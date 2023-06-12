#include <lib.h>
const int PP1 = 0x800, PP2 = 0x1001;
int finished = 0;
void handler(int num){
    debugf("into sighandler to %d.\n",num);
    int envid=syscall_getenvid();
    debugf("%x received sig%d.\n",envid,num);
    if(num == 5){
      finished++;
      debugf("adding finished!\n");
    }
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
  sigaction(5,&sig,NULL);//5号设置finished加一
  //由于屏蔽位被替换，PP2应当能收到5、2、3
  debugf("i am %x\n", me);
  sigprocmask(2, &shed, NULL);
  if (me == PP1) {
    kill(PP2, 5);
    debugf("@@@@PP1 sent 5 to %x@@@@\n", PP2);
    debugf("PP1 start wait finished sig\n");
    while(finished <1){
      i++;
      if((i%10000000)  == 0){
        debugf("%x now finished is %d\n", me,finished);
      }
    }
    debugf("pp1received start sig and send!\n");
    who = PP2;
    kill(who, 5);
    kill(who,5);
    kill(who, 3);
    kill(who, 5);
    kill(who, 2);// 共发送4次5信号，PP2应当均可接收到
    debugf("@@@@PP1 sent 5,3,2 to PP2@@@@\n");
  }if(me == PP2){
    debugf("wait PP1 start!\n");
    while(finished < 1){
      i++;
      if((i%10000000) == 0){
        debugf("%x now finished is %d\n", me,finished);
      }
    } 
    //pp2完成初始化，给pp1发送开始信号。
    kill(PP1, 5);
    debugf("@@@@@send 5 from %x to %x\n", me, PP1);
    debugf("PP2 for start\n");
    while(finished <4){
      i++;
      if((i%10000000) == 0){
        debugf("%x now finished is %d\n", me,finished);
      }
    } 
  }
  return 0;
}
