#include <env.h>
#include <lib.h>
#include <mmu.h>

static void __attribute__((noreturn)) signal_caller(struct Trapframe *tf,int num,void (*sa_handler)(int)) {
    if(sa_handler){
        void (*func)(int);
        func=sa_handler;
        func(num);
        syscall_pop_running_sig();
        // tf->cp0_epc+=4; //不加才是正确的，加了会导致访问错误的地方，产生SIGSEGV
        syscall_set_trapframe(0,tf);
    }
    else if(num==SIGSEGV||num==SIGKILL||num==SIGTERM){
        syscall_pop_running_sig();
        debugf("The process ended successfully.\n");
        exit(); //直接结束进程
    }
}

void exit(void) {
	// After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
	close_all();
#endif

	syscall_env_destroy(0);
	user_panic("unreachable code");
}

volatile struct Env *env;
extern int main(int, char **);

void libmain(int argc, char **argv) {
	syscall_set_env_signal_caller(0,signal_caller);
	// set env to point at our env structure in envs[].
	env = &envs[ENVX(syscall_getenvid())];
	// call user main routine
	main(argc, argv);

	// exit gracefully
	exit();
}
