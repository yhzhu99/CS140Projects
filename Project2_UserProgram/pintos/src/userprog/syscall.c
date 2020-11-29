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
void exit(struct intr_frame *);
pid_t exec(struct intr_frame *);
int wait(struct intr_frame *);
bool create(struct intr_frame *);



void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  int *p = f->esp;
  switch (*p)
  {
  case SYS_HALT:
    halt();
    break;
  case SYS_EXIT:
    exit(f);
    break;
  case SYS_EXEC:
    exec(f);
    break;
  case SYS_WAIT:
    wait(f);
    break;
  case SYS_CREATE:
    create(f);
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
exit(struct intr_frame *f)
{
  int status = *((int*)(f->esp)+1);
  //todo: close all files
  thread_current()->ret = status;
  thread_exit();
}

pid_t
exec(struct intr_frame* f)
{
  char *cmd_line = (char*)(f->esp+4);
  return process_execute(cmd_line);
}

int 
wait(struct intr_frame *f)
{
  pid_t pid = *(int*)(f->esp+4);
  return process_wait(pid);
}