#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_waitx(void)
{
  uint64 addr, addr1, addr2;
  uint w_time, r_time;
  argaddr(0, &addr);
  argaddr(1, &addr1); // user virtual memory
  argaddr(2, &addr2);
  int ret = waitx(addr, &w_time, &r_time);
  struct proc *p = myproc();
  if (copyout(p->pagetable, addr1, (char *)&w_time, sizeof(int)) < 0)
    return -1;
  if (copyout(p->pagetable, addr2, (char *)&r_time, sizeof(int)) < 0)
    return -1;
  return ret;
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
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

  argint(0, &pid);
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
sys_trace(void)
{
  int mask;
  argint(0, &mask);
  if (mask < 0)
    return -1;
  myproc()->mask = mask;
  return 0;
}

uint64 sys_sigalarm(void)
{
  uint64 addr;
  int ticks;
  argint(0, &ticks);
  if (ticks < 0)
    return -1;
  argaddr(1, &addr);
  if (addr < 0)
    return -1;
  myproc()->totalticks = ticks;
  myproc()->handler = addr;

  return 0;
}

uint64 sys_sigreturn(void)
{
  struct proc *p = myproc();
  memmove(p->trapframe, p->alarm_trapframe, PGSIZE);

  kfree(p->alarm_trapframe);
  p->alarm_trapframe = 0;
  p->alarm_on = 0;
  p->currentticks = 0;
  return p->trapframe->a0;
}
uint64 sys_settickets(void)
{
  int tickets;
  argint(0, &tickets);
  if (tickets < 0)
    return -1;
  myproc()->tickets = tickets;
  return 0;
}
uint64
sys_setpriority()
{
  int pid, priority;
  int arg_num[2] = {0, 1};

  argint(arg_num[0], &priority);
  if (priority < 0)
    return -1;

  argint(arg_num[1], &pid);
  if (pid < 0)
    return -1;

  return setpriority(priority, pid);
}