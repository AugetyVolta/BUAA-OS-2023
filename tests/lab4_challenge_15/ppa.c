#include<lib.h>
int global=0;
void handler(int num){
    debugf("Reach handler ,now the signum is %d!\n",num);
    global=1;
}

struct sigset_t set2;
int main(int argc,char**argv){
    struct sigset_t set;
    struct sigaction sig;
    sigemptyset(&set);
    sigaddset(&set,1);
    //sigaddset(&set,2);
    sigaddset(&set,15);
    sig.sa_handler=handler;
    sig.sa_mask=set;
    panic_on(sigaction(2,&sig,NULL));
    //panic_on(sigprocmask(2,&set,NULL));
    // sigdelset(&set,2);
    int ret=fork(),temp=0;
    if(ret!=0){
        // sigprocmask(2,&set,NULL);
        debugf("father envid %d\n",syscall_getenvid());
        debugf("child is %d\n", ret);
        if (syscall_getenvid() == 4097) {
            kill(8195, 2);
        } else if (syscall_getenvid() == 2048) {
            kill(6146, 2);
        }
        for(int i=0;i<10000000;i++)temp+=i;
        debugf("Father : global %d.\n",global);
    }else{  
            debugf("child envid %d\n",syscall_getenvid());
            if (syscall_getenvid() == 8195) {
            kill(4097, 2);
        } else if (syscall_getenvid() == 6146) {
            kill(2048, 2);
        }
            for(int i=0;i<10000;i++)temp+=i;
            debugf("Child : global %d.\n",global);
    }
    return 0;
}
