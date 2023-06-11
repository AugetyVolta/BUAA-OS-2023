#include <lib.h>
void handler(int num){
    debugf("I can see envid %d.\n",num);
}
sigset_t set2;
int main(int argc, char **argv) {
    sigset_t set;
    struct sigaction sig;
    struct sigaction swap_sig;
    sigemptyset(&set);
    sigaddset(&set, 1);
    sigaddset(&set, 2);
    sig.sa_handler=handler;
    sig.sa_mask=set;
    panic_on(sigprocmask(0, &set, NULL));
    sigdelset(&set, 2);
    panic_on(sigaction(10, &sig, NULL));//注册函数
    panic_on(sigaction(5, &sig, NULL));//注册函数
    int ret = fork();
    if (ret != 0) {
        kill(ret,10);
        panic_on(sigprocmask(0, &set2, &set));
        debugf("Father: %d.\n", sigismember(&set, 2));
    } else {
        debugf("Child: %d.\n", sigismember(&set, 2));
    }
    for(int i=0;i<100000;i++);
    return 0;
}