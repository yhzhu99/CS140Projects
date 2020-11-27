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

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}

// /* Terminates Pintos by calling shutdown_power_off() (declared in devices/shutdown.h). This should be seldom used, because you lose some information about possible deadlock situations, etc. */

// void 
// halt (void)
// {
//   shutdown_power_off();  
// }

// /* Terminates the current user program, returning status to the kernel. If the process's parent waits for it (see below), this is the status that will be returned. Conventionally, a status of 0 indicates success and nonzero values indicate errors. */

// void 
// exit (int status)
// {
//   // terminates current user program 
//   // return status to the kernel
// }

// /* Runs the executable whose name is given in cmd_line, passing any given arguments, and returns the new process's program id (pid). Must return pid -1, which otherwise should not be a valid pid, if the program cannot load or run for any reason. Thus, the parent process cannot return from the exec until it knows whether the child process successfully loaded its executable. You must use appropriate synchronization to ensure this. */
// pid_t 
// exec (const char * cmd_line)
// {

// }