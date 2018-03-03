#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// User defined sys calls (David, Part 2)

// Converts timezones, disregard leap years
// hourDiff = hours needed to convert to new timezone
// relative to the current timeZone
// Part 2
void timeZoneConvert(struct rtcdate* d, int hourDiff)
{
  // hourDiff shouldn't be greater than a day
  if(hourDiff < -24 || hourDiff > 24) return;
  uint daysOfMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  //cprintf("time(): %d\n", ((int)d->hour+hourDiff) <0);
  // desired timeZone is behind by hourDiff
  if((int)d->hour+ hourDiff < 0)
  {
      d->hour = 24 + hourDiff;
      d->day--;
      //cprintf("time:() %d\n",d->hour);  
      // decrement the month
      if(d->day <= 0)
      {
        d->month--;
        // decrement the year
        if(d->month <= 0)
        {
          d->year--;
          if(d->year <= 0) d->year = 0; // shouldn't hit this normally
          d->month = 12; // Should be december
        }
        d->day = daysOfMonth[d->month-1];
      }
  }
  // the desired timeZone is ahead by hourDiff
  else if(d->hour + hourDiff >= 24)
  {
    d->day++;
    // increment month if needed
    if(d->day > daysOfMonth[d->month-1])
    {
      d->month++;
      d->day = 1;
      // increment to new year
      if(d->month > 12)
      { 
        d->year++;
        d-> month = 1;
      }
    }
    d->hour = d->hour + hourDiff - 24; // adjust hours 
  }
  d->hour += hourDiff; // No adjustments needed
}

int sys_date(void)
{
  struct rtcdate* date;
  //cprintf("date(): %d\n", &date);
  if(argrtcdate(0,&date, sizeof(*date)) < 0)
    return -1;
  //cprintf("date(): date addr %d\n", date);
  cmostime(date); // Time is in UTC
  //cprintf("Time UTC: %d-%d-%d %d:%d:%d\n", date->year, date->month, date->day, date->hour, date->minute, date->second);
  //date->hour =19;
  //date->day = 31;
  //date->month = 12;
  //cprintf("Time UTC modded: %d-%d-%d %d:%d:%d\n", date->year, date->month, date->day, date->hour, date->minute, date->second);
  timeZoneConvert(date, -5); // To EST
  /* cprintf("date(): sec %d\n", &date->second);
   cprintf("date(): min %d\n", &date->minute);
   cprintf("date(): hour %d\n",  &date->hour);
   cprintf("date(): day %d\n", &date->day);
   cprintf("date(): month %d\n", &date->month);
   cprintf("date(): year %d\n", &date->year); */
  return 0;
}


// xv6 sys calls
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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
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
