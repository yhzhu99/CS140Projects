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
    printf("SYS_EXIT\n");
    //exit(*(p+1));
    break;
  case SYS_EXEC:
    printf("SYS_EXEC\n");
    //exec(*(p+1));
    break;
  case SYS_WAIT:
    printf("SYS_WAIT!\n");
    break;
  case SYS_CREATE:
    printf("SYS_WAIT!\n");
    break;
  case SYS_REMOVE:
    printf("SYS_REMOVE!\n");
    break;
  case SYS_OPEN:
    printf("SYS_OPEN!\n");
    break;
  case SYS_FILESIZE:
    printf("SYS_FILESIZE!\n");
    break;  
  case SYS_READ:
    printf("SYS_READ!\n");
    break;
  case SYS_WRITE:
    printf("SYS_WRITE!\n");
    break;
  case SYS_SEEK:
    printf("SYS_SEEK!\n");
    break;
  case SYS_TELL:
    printf("SYS_TELL!\n");
    break;
  case SYS_CLOSE:
    printf("SYS_CLOSE!\n");
    break;
  default:
    exit(-1);
    break;
  }
  //printf ("system call!\n");
  thread_exit ();
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