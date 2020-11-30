#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;
void syscall_init (void);
/* file descriptor */
struct fd{
    int num;
    struct file *file;
    struct list_elem allelem;
    struct list_elem elem;
};
struct list file_list;
#endif /* userprog/syscall.h */