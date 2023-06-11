#include <lib.h> 
    struct sigset_t set2;
    void handler(int num){
            debugf("HANDLER:%x %d\n",syscall_getenvid(),num);
      }
     int main(int argc, char **argv) {
         struct sigset_t set;
       set.sig[0]=set.sig[1]=0;
        struct sigaction act;
       act.sa_mask=set;
       act.sa_handler=handler;
       sigaction(1,&act,NULL);
       int ret = fork();
       if (ret != 0) {
           kill(ret,1);
           debugf("Father!!!!!!!!!!\n");
      } else {
          debugf("siojdioqjd\n");
         //while(1);
        }
     return 0;
}           