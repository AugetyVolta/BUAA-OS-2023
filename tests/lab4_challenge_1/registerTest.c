#include <lib.h>
#define TEST_NUM 10
/*
期望输出
sigaddset TEST_OK.
sigdelset TEST_OK.
You have changed register func for sig10.
register successfully!
You have passed register Test for sig10!
*/
void handler(int num){
    debugf("You have passed register Test for sig%d!\n",num);
}

void handler1(int num){
    debugf("You have changed register func for sig%d.\n",num);
}

int main(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,3); //添加id为3的信号
    sigaddset(&set,5); //添加id为5的信号
    struct sigaction sig;
    struct sigaction swap_sig;
    if(sigismember(&set,3)==0){ //检查3是否在
        user_panic("wrong in sigaddset_1\n");
    }
    if(sigismember(&set,5)==0){ //检查5是否在
        user_panic("wrong in sigaddset_2\n");
    }
    debugf("sigaddset TEST_OK.\n");
    sigdelset(&set,5);//删除id为5的信号
    if(sigismember(&set,3)==0){ //检查3是否还在
        user_panic("wrong in sigdelset_1\n");
    }
    if(sigismember(&set,5)==1){ //检查5是否已经被删除
        user_panic("wrong in sigdelset_2\n");
    }
    debugf("sigdelset TEST_OK.\n");
    sig.sa_handler=handler;
    sig.sa_mask=set;
    if(sigaction(TEST_NUM, &sig, NULL)<0){
        user_panic("first register failed.\n");
    }
    sig.sa_handler=handler1; //修改处理函数
    if(sigaction(TEST_NUM, &sig, &swap_sig)<0){
        user_panic("second register failed.\n");
    }
    //测试函数是否重新设置
    sig.sa_handler(TEST_NUM);
    if(swap_sig.sa_mask.sig[0]==4){ //0b100,第三位信号
        debugf("register successfully!\n");
    }
    else{
        debugf("register failed.\n");
    }
    //测试交换出来的函数是否正确
    swap_sig.sa_handler(TEST_NUM);
    return 0;
}