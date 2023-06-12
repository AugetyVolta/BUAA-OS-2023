#include <lib.h> 
/*
father:2048 child:4097
[00000800] destroying 00000800
[00000800] free env 00000800
i am killed ...
cnt:1 HANDLER:1001 10
cnt:2 HANDLER:1001 10
cnt:3 HANDLER:1001 10
cnt:4 HANDLER:1001 10
CONGRATULATION:TEST PASSED!
The process ended successfully.
[00001001] destroying 00001001
[00001001] free env 00001001
i am killed ...
*/
    struct sigset_t set2;
    int  cnt=0;
    void handler(int num){
            cnt++;
            debugf("cnt:%d HANDLER:%x %d\n",cnt,syscall_getenvid(),num);
            if(cnt==10){
                  debugf("CONGRATULATION:TEST PASSED!\n");
                  kill(0,SIGKILL);
            }
      }
     int main(int argc, char **argv) {
             struct sigset_t set;
       set.sig[0]=set.sig[1]=0;
        struct sigaction act;
       act.sa_mask=set;
       act.sa_handler=handler;
       sigaction(10,&act,NULL);
     int ret = fork();
       debugf("father:%d child:%d\n",syscall_getenvid(),ret);
       if (ret != 0) {
               for(int i=0;i<10;i++){
                  kill(ret,10);
               }
      } else {
           while(1);
      }
     return 0;
   }       