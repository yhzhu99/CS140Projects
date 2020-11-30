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
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include <string.h>

static void syscall_handler (struct intr_frame *);

void syscall_halt(void);
void syscall_exit(struct intr_frame *);
void syscall_exec(struct intr_frame *);
void syscall_wait(struct intr_frame *);
void syscall_create(struct intr_frame *);
void syscall_remove(struct intr_frame *);
void syscall_open(struct intr_frame *);
void syscall_filesize(struct intr_frame *);
void syscall_read(struct intr_frame *);
void syscall_write(struct intr_frame *);

void halt(void);
void exit(int);
pid_t exec(const char*);
int wait(pid_t pid);
bool create(const char*,unsigned);
bool remove(const char*);
int open(const char*);
int filesize(int fd);
int read(int,void *,unsigned);
int write(int,const void *,unsigned);

struct fd* find_fd_by_num(int num);

struct fd*
find_fd_by_num(int num)
{
  struct list_elem *e;
  for (e = list_begin (&file_list); e != list_end (&file_list); e = e->prev)
  {
    struct fd *fd = list_entry (e, struct fd, allelem);
    if(fd->num == num)return fd;
  }
  return NULL;
}


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  list_init(&file_list);
}

static void
syscall_handler (struct intr_frame *f) 
{
  int *p = f->esp;
  switch (*p)
  {
  case SYS_HALT:
    syscall_halt();
    break;
  case SYS_EXIT:
    syscall_exit(f);
    break;
  case SYS_EXEC:
    syscall_exec(f);
    break;
  case SYS_WAIT:
    syscall_wait(f);
    break;
  case SYS_CREATE:
    syscall_create(f);
    break;
  case SYS_REMOVE:
    syscall_remove(f);
    break;
  case SYS_OPEN:
    syscall_open(f);
    break;
  case SYS_FILESIZE:
    syscall_filesize(f);
    break;  
  case SYS_READ:
    syscall_read(f);
    break;
  case SYS_WRITE:
    syscall_write(f);
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
syscall_halt(void)
{
  halt();
}

void 
halt(void)
{
  shutdown_power_off();
}

void 
syscall_exit(struct intr_frame *f)
{
  int status = *(int*)(f->esp+4);
  exit(status);
}

void
exit(int status)
{
  //todo: close all files
  thread_current()->ret = status;
  thread_exit();
}

void 
syscall_exec(struct intr_frame *f)
{
  char *cmd_line = *(char**)(f->esp+4);
  exec(cmd_line);
}


pid_t
exec(const char* cmd_line)
{
  return process_execute(cmd_line);
}

void 
syscall_wait(struct intr_frame *f)
{
  pid_t pid = *(int*)(f->esp+4);
  wait(pid);
}

int 
wait(pid_t pid)
{
  return process_wait(pid);
}

void 
syscall_create(struct intr_frame *f)
{
  char *file = *(char**)(f->esp+4);
  unsigned initial_size = *(int *)(f->esp+8);
  create(file,initial_size);
}

bool 
create(const char*file , unsigned initial_size)
{
  return filesys_create(file,initial_size);
}

void
syscall_remove(struct intr_frame *f)
{
  char *file = *(char**)(f->esp+4);
  remove(file);
}

bool 
remove(const char* file)
{
  return filesys_remove(file);
}

void 
syscall_open(struct intr_frame *f)
{
  char *file = *(char**)(f->esp+4);
  open(file);
}

int 
open(const char* file)
{
  struct file *f = filesys_open(file);
  if(f==NULL)return -1;                                /* open failed */
  struct fd *fd = malloc(sizeof(struct fd));
  fd->file = f;                                        
  fd->num = 2;                                         /* File descriptors numbered 0 and 1 are reserved for the console */
  struct thread *cur = thread_current();
  list_push_back(&file_list,fd->allelem);
  list_push_back(&cur->fd_list,fd->elem);
  return fd->num;
}

void
syscall_filesize(struct intr_frame *f)
{
  int fd = *(int*)(f->esp+4);
  filesize(fd);
}

int 
filesize(int num)
{
  struct fd *fd = find_fd_by_num(num);
  if(fd == NULL)
  {
    exit(-1);
  }
  return file_length(fd->file);
}

void 
syscall_read(struct intr_frame *f)
{
  int fd = *(int*)(f->esp+4);
  void *buffer = *(char**)(f->esp+8);
  unsigned size = *(unsigned*)(f->esp+12);
  read(fd,buffer,size);
}

int 
read(int num,void *buffer,unsigned size)
{
  /* Fd 0 reads from the keyboard using input_getc(). */
  if(num == 0)
  {
    int i;
    for(i=0;i<size;i++){
      (*((char**)buffer))[i] = input_getc();
    }
    return size;
  }
  struct fd* fd = find_fd_by_num(num);
  return file_read(fd->file,buffer,size);
}

void 
syscall_write(struct intr_frame *f)
{
  int fd = *(int*)(f->esp+4);
  void *buffer = *(char**)(f->esp+8);
  unsigned size = *(unsigned*)(f->esp+12);
  write(fd,buffer,size);
}

int 
write(int num,const void* buffer,unsigned size)
{
  /* Fd 1 writes to the console. Your code to write to the console should write all of buffer in one call to putbuf() */
  if(num == 1)
  {
    int i;
    putbuf(buffer,size);
    return size;
  }
  struct fd* fd = find_fd_by_num(num);
  return file_write(fd->file,buffer,size); 
}