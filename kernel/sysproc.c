#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;
  backtrace();
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 
sys_sigalarm(void) 
{
   int ticks;
   uint64 p;
   if(argint(0, &ticks) < 0)
     return -1;
   if(argaddr(1, &p) < 0) 
     return -1;
   myproc()->timer = ticks;
   myproc()->handler = (void (*) ())p; //Pointer to function that returns void and takes no arg 
   return 0; 
}
uint64
sys_sigreturn(void)
{
   //Restore state from process saved trapframe
   struct proc *p = myproc();
    p->trapframe->ra = p->saveframe->ra;
    p->trapframe->sp = p->saveframe->sp;
    p->trapframe->gp = p->saveframe->gp;
    p->trapframe->tp = p->saveframe->tp;
    p->trapframe->t0 = p->saveframe->t0;
    p->trapframe->t1 = p->saveframe->t1;
    p->trapframe->t2 = p->saveframe->t2;
    p->trapframe->s0 = p->saveframe->s0;
    p->trapframe->s1 = p->saveframe->s1;
    p->trapframe->a1 = p->saveframe->a1;
    p->trapframe->a2 = p->saveframe->a2;
    p->trapframe->a3 = p->saveframe->a3;
    p->trapframe->a4 = p->saveframe->a4;
    p->trapframe->a5 = p->saveframe->a5;
    p->trapframe->a6 = p->saveframe->a6;
    p->trapframe->a7 = p->saveframe->a7;
    p->trapframe->s2 = p->saveframe->s2;
    p->trapframe->s3 = p->saveframe->s3;
    p->trapframe->s4 = p->saveframe->s4;
    p->trapframe->s5 = p->saveframe->s5;
    p->trapframe->s6 = p->saveframe->s6;
    p->trapframe->s7 = p->saveframe->s7;
    p->trapframe->s8 = p->saveframe->s8;
    p->trapframe->s9 = p->saveframe->s9;
    p->trapframe->s10 = p->saveframe->s10;
    p->trapframe->s11 = p->saveframe->s11;
    p->trapframe->t3 = p->saveframe->t3;
    p->trapframe->t4 = p->saveframe->t4;
    p->trapframe->t5 = p->saveframe->t5;
    p->trapframe->t6 = p->saveframe->t6;
    p->trapframe->epc = p->saveframe->epc;
    p->counter = 0;
   return 0;
}
