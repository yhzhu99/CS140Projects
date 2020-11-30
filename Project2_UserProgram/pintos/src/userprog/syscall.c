/* Whenever a user process wants to access some kernel functionality, 
  it invokes a system call. This is a skeleton system call handler. 
  Currently, it just prints a message and terminates the user process. 
  In part 2 of this project you will add code to do everything else 
  needed by system calls. */

/* limitaions:
- Concurrent accesses will interfere with one another. You should use synchronization to ensure that only one process at a time is executing file system code.
- The number of files that may be created is also limited.
- File data is allocated as a single extent, that is, data in a single file must occupy a contiguous range of sectors on disk. External fragmentation can therefore become a serious problem as a file system is used over time.
- No subdirectories.
- File names are limited to 14 characters.
- There is no file system repair tool anyway.
- If a file is open when it is removed, its blocks are not deallocated and it may still be accessed by any threads that have it open, until the last one closes it. 
*/

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
#include "threads/vaddr.h"
#include "pagedir.h"
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
void syscall_seek(struct intr_frame *);
void syscall_tell(struct intr_frame *);
void syscall_close(struct intr_frame *);

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
void seek(int,unsigned);
unsigned tell(int);
void close(int);

struct fd* find_fd_by_num(int);
bool pointer_valid(uint32_t,int);

/* file descriptor */
struct fd{
    int num;
    struct file *file;
    struct list_elem allelem;
    struct list_elem elem;
};
struct list file_list;

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

bool
pointer_valid(uint32_t esp,int num)
{
  int i;
  struct thread *cur = thread_current();
  for(i=0;i<num;i++)
  {
    if(!is_user_vaddr(esp) || pagedir_get_page(cur->pagedir,esp) == NULL)
    {
      return false;
    }
  }
  return true;
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
  if(!pointer_valid(f->esp,1))
  {
    exit(-1);
  }
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
    syscall_seek(f);
    break;
  case SYS_TELL:
    syscall_tell(f);
    break;
  case SYS_CLOSE:
    syscall_close(f);
    break;
  default:
    exit(-1);
    break;
  }
  //printf ("system call!\n");
  //thread_exit ();
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
  if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
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
  if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  char *cmd_line = *(char**)(f->esp+4);
  f->eax = exec(cmd_line);
}


pid_t
exec(const char* cmd_line)
{
  return process_execute(cmd_line);
}

void 
syscall_wait(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  pid_t pid = *(int*)(f->esp+4);
  f->eax = wait(pid);
}

int 
wait(pid_t pid)
{
  return process_wait(pid);
}

void 
syscall_create(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,2))
  {
    exit(-1);
  }
  char *file = *(char**)(f->esp+4);
  unsigned initial_size = *(int *)(f->esp+8);
  f->eax = create(file,initial_size);
}

bool 
create(const char*file , unsigned initial_size)
{
  return filesys_create(file,initial_size);
}

void
syscall_remove(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  char *file = *(char**)(f->esp+4);
  f->eax = remove(file);
}

bool 
remove(const char* file)
{
  return filesys_remove(file);
}

void 
syscall_open(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  char *file = *(char**)(f->esp+4);
  f->eax = open(file);
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
  list_push_back(&file_list,&fd->allelem);
  list_push_back(&cur->fd_list,&fd->elem);
  file_deny_write(f);
  return fd->num;
}

void
syscall_filesize(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  int fd = *(int*)(f->esp+4);
  f->eax = filesize(fd);
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
   if(!pointer_valid(f->esp+4,3))
  {
    exit(-1);
  }
  int fd = *(int*)(f->esp+4);
  void *buffer = *(char**)(f->esp+8);
  unsigned size = *(unsigned*)(f->esp+12);
  f->eax = read(fd,buffer,size);
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
   if(!pointer_valid(f->esp+4,3))
  {
    exit(-1);
  }
  int fd = *(int*)(f->esp+4);
  void *buffer = *(char**)(f->esp+8);
  unsigned size = *(unsigned*)(f->esp+12);
  f->eax = write(fd,buffer,size);
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

void
syscall_seek(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,2))
  {
    exit(-1);
  }
  int fd = *(int*)(f->esp+4);
  unsigned position = *(unsigned*)(f->esp+8);
  seek(fd,position);
}

void
seek(int num, unsigned position)
{
  struct fd *fd = find_fd_by_num(num);
  file_seek(fd->file,position); 
}

void
syscall_tell(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  int td = *(int*)(f->esp+4);
  f->eax = tell(td);
}

unsigned
tell(int num)
{
  struct fd *fd = find_fd_by_num(num);
  return file_tell(fd->file);
}

void
syscall_close(struct intr_frame *f)
{
   if(!pointer_valid(f->esp+4,1))
  {
    exit(-1);
  }
  int fd = *(int*)(f->esp+4);
  close(fd);
}

void
close(int num)
{
  struct fd *fd = find_fd_by_num(num);
  file_close(fd->file);
  file_allow_write(fd->num);
}