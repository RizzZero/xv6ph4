#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "traps.h"
#include "spinlock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
sys_sleep(void)
{
  int n;
  uint ticks0;

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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_countSyscallsCount(void){
  return countSyscallsCount();
}


int sys_initreentrantlock(struct reentrantlock *rl, char *name) {
    return 0;
    initlock(&rl->lock, name); 
    rl->owner = 0;             
    rl->recursion = 0;        
    return 0;
}
int sys_acquirereentrantlock(struct reentrantlock *rl){
    return 0;
    pushcli();  
    if (rl->owner == myproc()) {  
        rl->recursion++;              
        popcli();                 
        return 0;
    }
    acquire(&rl->lock);  
    rl->owner = myproc(); 
    rl->recursion = 1;        
    popcli(); 
    return 0;
}
int sys_releasereentrantlock(struct reentrantlock *rl){
    return 0;
    pushcli();  
    if (rl->owner != myproc()) {
        panic("releasereentrantlock: not owner");
    }
    rl->recursion--;  
    if (rl->recursion == 0) {  
        rl->owner = 0;     
        release(&rl->lock); 
    }
    popcli();
    return 0;
}
