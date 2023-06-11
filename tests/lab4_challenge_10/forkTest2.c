#include<lib.h>
int global=0;
void handler(int num){
    debugf("Reach handler ,now the signum is %d!\n",num);
    global=1;
}

void handlertwo(int num) {
    debugf("Reach handlertwo ,now the signum is %d!\n",num);
}

struct sigset_t set2;
//测试了子进程会继承父进程的处理函数/写时复制
int main(int argc,char**argv){
    struct sigset_t set;
    struct sigaction sig;
    sigemptyset(&set);
    sigaddset(&set,1);
    sigaddset(&set,2);
    sig.sa_handler=handler;
    sig.sa_mask=set;
    panic_on(sigaction(2,&sig,NULL));
    panic_on(sigprocmask(1,&set,NULL));
    sigdelset(&set,2);
    int ret=fork(),temp=0;
    if(ret!=0){
        debugf("child is %d\n", ret);
        kill(ret,2);
        for(int i=0;i<1000000;i++)temp*=i;
        //sig.sa_handler=handlertwo;
        //panic_on(sigaction(2,&sig,NULL));
        debugf("Father : global %d.\n",global);
        }else{
            kill(2048, 2);
            for(int i=0;i<10000;i++)
                temp+=i;
            debugf("Child : global %d.\n",global);
        }
    return 0;
}