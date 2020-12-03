# PROJECT 2: USER PROGRAMS DESIGN DOCUMENT

[toc]

## 1.GROUP

> Fill in the names and email addresses of your group members.


| NAME | SID | MAIL | RATIO |
| - | - | - | - |
| 朱英豪 | 18373722 | 18373722@buaa.edu.cn | 25% |
| 施哲纶 | 18373044 | 18373044@buaa.edu.cn | 25% |
| 胡鹏飞 | 18373059 | 18373059@buaa.edu.cn | 25% |
| 朱晨宇 | 18373549 | 18373549@buaa.edu.cn | 25% |

> 主要负责内容


| NAME | RESPONSIBLE FOR |
| - | - |
| 朱英豪 | 需求、思路设计；文档编写 |
| 施哲纶 | 具体算法实现；文档审核 |
| 胡鹏飞 | 项目前期调研；理解Pintos |
| 朱晨宇 | 负责Debug，代码风格检查 |

> Github记录

> 样例通过情况

## 2.PRELIMINARIES

> If you have any preliminary comments on your submission, notes for the TAs, or extra credit, please give them here.

Our GitHub repository is private. Please contact us if necessary.

> Please cite any offline or online sources you consulted while preparing your submission, other than the Pintos documentation, course text, lecture notes, and course staff.

1. 操作系统概念(原书第9版)/(美)Abraham Silberschatz等著
2. 原仓周老师PPT中的概念和课上讲解

## 3. QUESTION 1: ARGUMENT PASSING

### 需求分析

### 设计思路

### DATA STRUCTURES

> A1: Copy here the declaration of each new or changed `struct` or `struct` member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

### 3.2 ALGORITHMS

> A2: Briefly describe how you implemented argument parsing.  How do you arrange for the elements of argv[] to be in the right order? How do you avoid overflowing the stack page?

> A2: 简要描述你是怎么实现Argument parsing的。你是如何安排argv[]中的elements，使其在正确的顺序的？你是如何避免stack page的溢出的？

### 3.3 RATIONALE

> A3: Why does Pintos implement strtok_r() but not strtok()?

> A3: 为什么Pintos中实现strtok_r()而不是strtok()？

> A4: In Pintos, the kernel separates commands into a executable name and arguments.  In Unix-like systems, the shell does this separation.  Identify at least two advantages of the Unix approach.

> A4: 在Pintos中，kernel将命令分成了可执行文件的name以及参数。在Unix-like的系统中，shell完成这部分的分隔。列举至少2种Unix这样做的好处。

## 4. QUESTION 2: SYSTEM CALLS

### 需求分析

### 设计思路

### DATA STRUCTURES

> B1: Copy here the declaration of each new or changed `struct` or `struct' member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

> B2: Describe how file descriptors are associated with open files. Are file descriptors unique within the entire OS or just within a single process?

> B2: 描述文件描述符是如何与打开文件相联系的。文件描述符是在整个中唯一还是仅在单个进程中唯一？

### 4.2 ALGORITHMS

> B3: Describe your code for reading and writing user data from the kernel.

> B3: 描述你用来从kernel中读写文件的代码。

> B4: Suppose a system call causes a full page (4,096 bytes) of data to be copied from user space into the kernel.  What is the least and the greatest possible number of inspections of the page table (e.g. calls to pagedir_get_page()) that might result?  What about for a system call that only copies 2 bytes of data?  Is there room for improvement in these numbers, and how much?

> B4: 假设一个系统调用造成一整页的数据(4096 bytes)从用户空间复制到kernel。
> 
> 求可能造成的最小和最大的页表的检查次数。(e.g. 对pagedir_get_page()的调用)。如果系统调用只copy了2 bytes的数据呢？还有没有空间优化？可以优化多少？

> B5: Briefly describe your implementation of the "wait" system call and how it interacts with process termination.

> B5: 简要描述你"wait"系统调用的实现以及它是如何与进程停止交互的。

> B6: Any access to user program memory at a user-specified address can fail due to a bad pointer value.  Such accesses must cause the process to be terminated.  System calls are fraught with such accesses, e.g. a "write" system call requires reading the system call number from the user stack, then each of the call's three arguments, then an arbitrary amount of user memory, and any of these can fail at any point.  This poses a design and error-handling problem: how do you best avoid obscuring the primary function of code in a morass of error-handling?  Furthermore, when an error is detected, how do you ensure that all temporarily allocated resources (locks, buffers, etc.) are freed?  In a few paragraphs, describe the strategy or strategies you adopted for managing these issues.  Give an example.

> 任何在用户指定的地址上对用户程序的内存的访问可能因为指针错误而失败。此类访问一定导致进程终止。系统调用充满了这样的访问。如一个“写”系统调用需要先从用户栈中读系统调用号，然后每一个调用的3个参数，然后是任意数量的用户内存。任何这些都可能造成失败。这构成一个设计错误处理的问题：如何最好地避免混淆主要错误处理的烦恼？此外，当错误被检查到，你如何保证所有的临时开出的资源（锁、缓冲区等）都被释放？用几段话来描述你处理这些问题的策略。

### 4.3 SYNCHRONIZATION

> B7: The "exec" system call returns -1 if loading the new executable fails, so it cannot return before the new executable has completed loading.  How does your code ensure this?  How is the load success/failure status passed back to the thread that calls "exec"?

> B7: 如果新的可执行文件加载失败，"exec"系统调用会返回-1，所以它不能够在该新的可执行文件成功加载之前返回。你的代码是如何保证这一点的？加载成功/失败的状态是如何传递回调用"exec"的线程的？

> B8: Consider parent process P with child process C.  How do you ensure proper synchronization and avoid race conditions when P calls wait(C) before C exits?  After C exits?  How do you ensure that all resources are freed in each case?  How about when P terminates without waiting, before C exits?  After C exits?  Are there any special cases?

> B8: 考虑有父进程P和它的子进程C。当P在Cexit之前调用wait(C)时，你如何确保同步以及如何避免争用的情况？你如何确保在每种情况下，所有的资源都被释放？如果P在C exit之前，没有waiting便终止？如果在C exit之后？有什么特殊情况吗？

### 4.4 RATIONALE

> B9: Why did you choose to implement access to user memory from the kernel in the way that you did?

> B9: 为什么你使用这种方式来实现从内核对用户内存的访问？

> B10: What advantages or disadvantages can you see to your design for file descriptors?

> B10: 你对文件描述符的设计有什么优劣吗？

> B11: The default tid_t to pid_t mapping is the identity mapping. If you changed it, what advantages are there to your approach?

> B11: 默认的tid_t到pid_t的映射是identity mapping。如果你进行了更改，那么你的方法有什么优点？

## 5. SURVEY QUESTIONS

Answering these questions is optional, but it will help us improve the course in future quarters.  Feel free to tell us anything you
want--these questions are just to spur your thoughts.  You may also choose to respond anonymously in the course evaluations at the end of the quarter.

> In your opinion, was this assignment, or any one of the three problems in it, too easy or too hard?  Did it take too long or too little time?

All of these three are too hard for our team. It takes us over 200 hours to finish the assignment!

> Did you find that working on a particular part of the assignment gave you greater insight into some aspect of OS design?

Yes. Especially the last part (BSD Scheduler) -- taking more aspects in OS into account, the thought of balancing threads' different scheduling needs...

> Is there some particular fact or hint we should give students in future quarters to help them solve the problems?  Conversely, did you find any of our guidance to be misleading?

No. The guidance is quite clear.

> Do you have any suggestions for the TAs to more effectively assist students, either for future quarters or the remaining projects?

It is recommended that when doing experiments, the TAs should first familiarize us with the OS, not let us explore it ourselves.

> Any other comments?

Thank you to all the teaching assistants!
