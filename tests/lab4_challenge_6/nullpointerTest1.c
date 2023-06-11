#include <lib.h>

/*
期望输出
The process ended successfully.
*/
#include <lib.h>

int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int *test = NULL;
void sgv_handler(int num) {
    debugf("Segment fault appear!\n");
    test = &a[0];
    debugf("test = %d.\n", *test);
    exit();
}

int main(int argc, char **argv) { //没有注册处理函数，以默认方式处理
    sigset_t set;
    sigemptyset(&set);
    struct sigaction sig;
    sig.sa_handler = sgv_handler;
    sig.sa_mask = set;
    *test = 10;
    debugf("test = %d.\n", *test);
    return 0;
}