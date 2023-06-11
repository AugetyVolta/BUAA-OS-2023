#include <lib.h>
struct sigset_t set2;
struct sigset_t set;
int mark = 0;

void handler_5(int num) {
kill(0,10);
debugf("\nsignal 5 should appear secondly mark = %d\n\n",mark);
mark++;
}
void handler_10(int num) {
debugf("\nsignal 10 should appear first mark = %d\n\n",mark);
mark++;
}
void handler_1(int num) {
debugf("\nsignal 1 should appear  mark = %d\n\n",mark);
mark++;
}

int main(int argc, char **argv) {
    sigemptyset(&set);
    struct sigaction sig;
    sig.sa_handler = handler_5;
    sig.sa_mask = set;
    sigaction(5,&sig,NULL);
    struct sigaction sig2;
    sig2.sa_handler = handler_10;
    sig2.sa_mask = set;
    
    sigaddset(&set, 1);
    sigaddset(&set, 10);
    struct sigaction sig3;
    sig3.sa_handler = handler_1;
    sig3.sa_mask = set;
    
    sigaction(1,&sig3,NULL);
    sigaction(5,&sig,NULL);
    sigaction(10,&sig2,NULL);
    panic_on(sigprocmask(0, &set, NULL));

    kill(0,1);
    kill(0,5);
    debugf("clear mask\n");
    sigemptyset(&set);
    panic_on(sigprocmask(2, &set, NULL));
    return 0;
 }