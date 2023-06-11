#include <lib.h>
#define TEST_NUM 10
/*
期望输出
env0 received following sigs:
5 17 23 35 24 56
*/
void handler(int num){
    debugf("You have passed register Test for sig%d!\n",num);
}
int main(){
    sigset_t set;
    sigemptyset(&set);
    struct sigaction sig;
    sig.sa_handler=handler;
    sig.sa_mask=set;
    sigaction(5, &sig, NULL);
    sigaction(17, &sig, NULL);
    sigaction(23, &sig, NULL);
    sigaction(35, &sig, NULL);
    sigaction(24, &sig, NULL);
    sigaction(56, &sig, NULL);
    if(kill(0,5)<0){
        user_panic("wrong in kill_1\n");
    }
    if(kill(0,17)<0){
        user_panic("wrong in kill_2\n");
    }
    if(kill(0,23)<0){
        user_panic("wrong in kill_3\n");
    }
    if(kill(0,35)<0){
        user_panic("wrong in kill_4\n");
    }
    if(kill(0,24)<0){
        user_panic("wrong in kill_5\n");
    }
    if(kill(0,56)<0){
        user_panic("wrong in kill_6\n");
    }
    if(kill(0,100)!=-1){ //测试错误条件
        user_panic("wrong in kill_7\n");
    }
    //getwaitlist(0);
    int ans = 0;
    for (int i = 0; i < 10000000; i++) {
        ans += i;
    }
    return 0;
}