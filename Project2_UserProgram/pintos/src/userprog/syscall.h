#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;
struct lock file_lock;
void syscall_init (void);
void close_all_fd();
void exit(int);
#endif /* userprog/syscall.h */