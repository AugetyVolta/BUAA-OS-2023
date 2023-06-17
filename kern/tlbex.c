#include <env.h>
#include <pmap.h>
#include <syscall.h>
#include <debugk.h>
extern struct Sig_free_list sig_free_list;
extern void schedule(int yield) __attribute__((noreturn));
static void passive_alloc(u_int va, Pde *pgdir, u_int asid) {
  struct Page *p = NULL;

  if (va < UTEMP) {
    #if !defined(LAB) || LAB >= 4
    sys_kill(0,11);
    #endif
    //panic("address too low");
  }

  if (va >= USTACKTOP && va < USTACKTOP + BY2PG) {
    printk("va %x \n",va);
    panic("invalid memory");
  }

  if (va >= UENVS && va < UPAGES) {
    panic("envs zone");
  }

  if (va >= UPAGES && va < UVPT) {
    panic("pages zone");
  }

  if (va >= ULIM) {
    panic("kernel address");
  }

  panic_on(page_alloc(&p));
  panic_on(page_insert(pgdir, asid, p, PTE_ADDR(va), PTE_D));
}

/* Overview:
 *  Refill TLB.
 */
Pte _do_tlb_refill(u_long va, u_int asid) {
  Pte *pte;
  /* Hints:
   *  Invoke 'page_lookup' repeatedly in a loop to find the page table entry
   * 'pte' associated with the virtual address 'va' in the current address space
   * 'cur_pgdir'.
   *
   *  **While** 'page_lookup' returns 'NULL', indicating that the 'pte' could
   * not be found, allocate a new page using 'passive_alloc' until 'page_lookup'
   * succeeds.
   */
  while (page_lookup(cur_pgdir, va, &pte) == NULL) {
    passive_alloc(va, cur_pgdir, asid);
  }
  /* Exercise 2.9: Your code here. */
  return *pte;
}

#if !defined(LAB) || LAB >= 3
/* Overview:
 *   This is the TLB Mod exception handler in kernel.
 *   Our kernel allows user programs to handle TLB Mod exception in user mode,
 * so we copy its context 'tf' into UXSTACK and modify the EPC to the registered
 * user exception entry.
 *
 * Hints:
 *   'env_user_tlb_mod_entry' is the user space entry registered using
 *   'sys_set_user_tlb_mod_entry'.
 *
 *   The user entry should handle this TLB Mod exception and restore the
 * context.
 */
void do_tlb_mod(struct Trapframe *tf) {
  struct Trapframe tmp_tf = *tf;

  if (tf->regs[29] < USTACKTOP || tf->regs[29] >= UXSTACKTOP) {
    tf->regs[29] = UXSTACKTOP;
  }
  tf->regs[29] -= sizeof(struct Trapframe);
  *(struct Trapframe *)tf->regs[29] = tmp_tf;

  if (curenv->env_user_tlb_mod_entry) {
    tf->regs[4] = tf->regs[29];
    tf->regs[29] -= sizeof(tf->regs[4]);
    // Hint: Set 'cp0_epc' in the context 'tf' to
    // 'curenv->env_user_tlb_mod_entry'.
    /* Exercise 4.11: Your code here. */
    tf->cp0_epc = curenv->env_user_tlb_mod_entry;
  } else {
    panic("TLB Mod but no user handler registered");
  }
}

void do_signal(struct Trapframe *tf){
  struct proc_signal *signal;
  if(TAILQ_EMPTY(&curenv->sig_wait_list)){
    return;
  }
  TAILQ_FOREACH(signal,&curenv->sig_wait_list,sig_wait_link){
    if(signal!=NULL&&signal->signum>=1&&signal->signum<=64){
        if(signal->signum==SIGSEGV||signal->signum==SIGKILL){
            //直接处理
        }
        else if(signal->sequence<=curenv->env_cur_signal){
            break;
        }
        else {
            int mask=curenv->env_sigset_t.sig[(signal->signum-1)/32];
            if(((mask>>((signal->signum-1)%32))&0x1)){
              continue;
            }
        }
        TAILQ_REMOVE(&curenv->sig_wait_list,signal,sig_wait_link);
        signal_handle(signal->signum,tf,signal->sequence);
        signal->signum=0;
        LIST_INSERT_HEAD(&sig_free_list,signal,sig_free_link);
        break;
    }
  }
}

void signal_handle(int num,struct Trapframe *tf,int signal_index) {
  if (curenv->env_sigaction[num].sa_handler||
      num==SIGSEGV||num==SIGKILL||num==SIGTERM) {
    struct Trapframe tmp_tf = *tf;
    if (tf->regs[29] < USTACKTOP || tf->regs[29] >= UXSTACKTOP) {
      tf->regs[29] = UXSTACKTOP;
    }
    tf->regs[29] -= sizeof(struct Trapframe);
    *(struct Trapframe *)tf->regs[29] = tmp_tf;
    if(curenv->env_signal_caller){
      tf->regs[4] = tf->regs[29];
      tf->regs[5] = num;
      tf->regs[6] = curenv->env_sigaction[num].sa_handler;
      tf->regs[7] = signal_index;
      tf->regs[29] -= sizeof(tf->regs[4]); 
      tf->regs[29] -= sizeof(tf->regs[5]);
      tf->regs[29] -= sizeof(tf->regs[6]);
      tf->regs[29] -= sizeof(tf->regs[7]);
      tf->cp0_epc = curenv->env_signal_caller;
    }
  } else {
    //默认不处理
  }
}
#endif
