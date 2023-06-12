#include<lib.h>
int global=0;
void handler(int num){
    debugf("Reach handler ,now the signum is %d!\n",num);
    global=1;
}


struct sigset_t set2;
//测试了子进程会继承父进程的处理函数/写时复制
//父进程15号被阻塞，子进程不会阻塞,阻塞SIGTERM号信号，尝试给父进程发送
int main(int argc,char**argv){
    struct sigset_t set;
    struct sigaction sig;
    sigemptyset(&set);
    sigaddset(&set,1);
    sigaddset(&set,2);
    sigaddset(&set,15);
    sig.sa_handler=handler;
    sig.sa_mask=set;
    panic_on(sigaction(2,&sig,NULL));
    panic_on(sigprocmask(2,&set,NULL));
    // sigdelset(&set,15);
    int ret=fork(),temp=0;
    if(ret!=0){
        // panic_on(sigprocmask(2,&set,NULL));
        debugf("child is %x\n", ret);
        kill(ret,2); //自己阻塞自己
        for(int i=0;i<1000000;i++)temp+=i;
        debugf("Father : global %d.\n",global);
    }else{
            kill(2048, 2); //还是会被阻塞
            // kill(0,2); 
            for(int i=0;i<10000;i++)temp+=i;
            debugf("Child : global %d.\n",global);
            kill(2048,15);
    }
    return 0;
}