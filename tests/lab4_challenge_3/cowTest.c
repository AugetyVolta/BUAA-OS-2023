#include <lib.h>
/*
期望输出
Father: 1.
I can see envid 4097.
Child: 0.
I can see envid 0.
*/
sigset_t set2;
void handler(int num){
    debugf("I can see envid %d.\n",num);
}
int main(int argc, char **argv) {
    sigset_t set;
    struct sigaction sig;
    struct sigaction swap_sig;
    sig.sa_handler=handler;
    sigemptyset(&set);
    sigaddset(&set, 1);
    sigaddset(&set, 2);
    panic_on(sigprocmask(0, &set, NULL));
    sigdelset(&set, 2);
    panic_on(sigaction(10, &sig, NULL));//注册函数
    int ret = fork();
    if (ret != 0) {
        panic_on(sigprocmask(0, &set2, &set));
        debugf("Father: %d.\n", sigismember(&set, 2));
        panic_on(sigaction(10, &sig, &swap_sig));
        swap_sig.sa_handler(ret); //用于检查子进程是否继承了父进程的处理函数
    } else {
        debugf("Child: %d.\n", sigismember(&set, 2));
        panic_on(sigaction(10, &sig, &swap_sig));
        swap_sig.sa_handler(ret); //用于检查子进程是否继承了父进程的处理函数
    }
    return 0;
}