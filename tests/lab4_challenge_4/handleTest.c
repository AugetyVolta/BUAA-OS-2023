#include <lib.h>
/*
期望输出
Reach handler, now the signum is 3!
Reach handler, now the signum is 2!
global = 2.
*/
int global = 0;
void handler(int num) {
    debugf("Reach handler, now the signum is %d!\n", num);
    global++;
}

#define TEST_NUM 2
int main(int argc, char **argv) {
    sigset_t set;
    sigemptyset(&set);
    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_mask = set;
    panic_on(sigaction(TEST_NUM, &sig, NULL));
    panic_on(sigaction(3, &sig, NULL));
    sigaddset(&set, TEST_NUM);
    panic_on(sigprocmask(0, &set, NULL));
    kill(0, TEST_NUM);
    kill(0, 3);
    int ans = 0;
    for (int i = 0; i < 10000000; i++) {
        ans += i;
    }
    panic_on(sigprocmask(1, &set, NULL));
    debugf("global = %d.\n", global);
    return 0;
}