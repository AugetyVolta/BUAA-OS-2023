#include <lib.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact){
    /*
    signum的范围是1~64,不满足范围返回-1。
    */
    if(!(signum>=1&&signum<=64)){ 
        return -1;
    }
    return syscall_sigaction(signum,act,oldact);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
    return syscall_sigprocmask(how,set,oldset);
}
/*
// 清空信号集，将所有位都设置为 0
*/
void sigemptyset(sigset_t *set){ 
    set->sig[0]=0;
    set->sig[1]=0;
}
/*
// 清空信号集，将所有位都设置为 1
*/
void sigfillset(sigset_t *set){ 
    set->sig[0]=0xffffffff;
    set->sig[1]=0xffffffff;
} 
/*
// 向信号集中添加一个信号，即将指定信号的位设置为 1
*/
void sigaddset(sigset_t *set, int signum){ 
    set->sig[(signum-1)/32]|=1<<((signum-1)%32);
} 
/*
// 从信号集中删除一个信号，即将指定信号的位设置为 0
*/
void sigdelset(sigset_t *set, int signum){ 
    set->sig[(signum-1)/32]&=~(1<<((signum-1)%32));
}
/*
// 检查一个信号是否在信号集中，如果在则返回 1，否则返回 0
*/
int sigismember(const sigset_t *set, int signum){
    if((set->sig[(signum-1)/32]>>((signum-1)%32))&0x1){
        return 1;
    }
    else{
        return 0;
    }
}

int kill(u_int envid, int sig){
    if(!(sig>=1&&sig<=64)){
        return -1;
    }
    return syscall_kill(envid,sig);
}