/* Whenever a user process wants to access some kernel functionality, 
  it invokes a system call. This is a skeleton system call handler. 
  Currently, it just prints a message and terminates the user process. 
  In part 2 of this project you will add code to do everything else 
  needed by system calls. */

#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include <string.h>

static void syscall_handler (struct intr_frame *);
void halt(void);
void exit(int status);
int exec(const char *cmd_line);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int *p = f->esp;
  switch (*p)
  {
  case SYS_HALT:
    halt();
    break;
  case SYS_EXIT:
    exit(*(p+1));
    break;
  case SYS_EXEC:
    exec(*(p+1));
    break;
  default:
    break;
  }
  // printf ("system call!\n");
  // thread_exit ();
}

void 
halt(void)
{
  shutdown_power_off();
}

void
exit(int status)
{
  thread_current()->ret = status;
  thread_exit();
}

int
exec(const char *cmd_line)
{
  return process_execute(cmd_line);
}