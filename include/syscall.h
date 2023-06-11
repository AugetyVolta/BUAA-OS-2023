#ifndef SYSCALL_H
#define SYSCALL_H

void sys_push_running_sig(int num);

#ifndef __ASSEMBLER__

enum {
	SYS_putchar,
	SYS_print_cons,
	SYS_getenvid,
	SYS_yield,
	SYS_env_destroy,
	SYS_set_tlb_mod_entry,
	SYS_mem_alloc,
	SYS_mem_map,
	SYS_mem_unmap,
	SYS_exofork,
	SYS_set_env_status,
	SYS_set_trapframe,
	SYS_panic,
	SYS_ipc_try_send,
	SYS_ipc_recv,
	SYS_cgetc,
	SYS_write_dev,
	SYS_read_dev,
	SYS_sigaction,
	SYS_sigprocmask,
	SYS_kill,
	SYS_set_env_signal_caller,
	SYS_pop_running_sig,
	SYS_push_running_sig,
	MAX_SYSNO,
};

#endif

#endif
