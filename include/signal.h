#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
#define SIGKILL 9
#define SIGSEGV 11
#define SIGTERM 15

typedef struct sigset_t{
    int sig[2]; //最多 32*2=64 种信号
}sigset_t;

struct sigaction{
    void (*sa_handler)(int);
    sigset_t sa_mask;
};

TAILQ_HEAD(Sig_wait_list,proc_signal);
LIST_HEAD(Sig_free_list,proc_signal);
struct proc_signal{
    int signum;
    int sequence;
    TAILQ_ENTRY(proc_signal) sig_wait_link;
    LIST_ENTRY(proc_signal) sig_free_link;
};

#endif 