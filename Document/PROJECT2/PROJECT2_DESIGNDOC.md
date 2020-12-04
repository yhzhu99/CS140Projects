PROJECT 2: USER PROGRAMS DESIGN DOCUMENT

[TOC]

## GROUP

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

## PRELIMINARIES

> If you have any preliminary comments on your submission, notes for the TAs, or extra credit, please give them here.

Our GitHub repository is private. Please contact us if necessary.

> Please cite any offline or online sources you consulted while preparing your submission, other than the Pintos documentation, course text, lecture notes, and course staff.

1. 操作系统概念(原书第9版)/(美)Abraham Silberschatz等著
2. 原仓周老师PPT中的概念和课上讲解

## QUESTION 1: ARGUMENT PASSING

### 需求分析

此部分为参数传递部分。

总体上来考察本次Project2的第一部分的需求分析，本组成员发现：在本次pintos实验中，项目需要能够通过输入的命令以及参数，相对应的执行不同部分的代码，以完成相对不同的功能以及作用。面对这样的需求，需要尽可能地将相对应的命令以及参数传递到项目中处理命令和参数内容的代码段处。

具体的考察这一部分（参数传递部分）的需求。目前，`process_execute()`不支持将参数传递给新进程。本项目需要通过扩展实现此功能(能够通过`process_execute()`传递参数给新的进程)。由于参数的输入，是通过字符串的形式输入的，并且不同参数之间，用空格分隔。同时，需要注意的是，参数之间用来分隔的空格，并不一定是单个空格，也有可能是多个空格。但在参数分隔中，连续的多个空格，同时等效于单个空格的分隔效果。在输入的命令行中，第一个单词是程序名称，也就是对应的命令名称，第二个单词是第一个参数，第三个单词是第二个参数，，依此类推。也就是说，举一个例子`process_execute("grep foo bar")`应该运行 `grep`，随后传递两个参数`foo`和`bar`。

本组需要完成的项目就是修改`process_execute()`，使之具备能够读取文件名，以及读取不同长度参数，并分析不同参数的功能。

### 设计思路

参数传递部分的代码主要分布在src/userprog/process.c文件中。

在设计思路上，参数传递部分的主要思想非常简单。参数传递的主要设计思想就是将输入的命令行读入进来，初步分为命令/文件名和参数两大部分。随后通过空格分隔符，将参数进一步分隔为不同的参数1、参数2、参数3……给下一步的程序继续处理.

### DATA STRUCTURES

> A1: Copy here the declaration of each new or changed `struct` or `struct` member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

在本内容中，添加的新的数据结构或者是修改已有的数据结构。

有：

struct list child_list;             /* 子进程列表 */

struct list_elem cpelem;            /* elem for child_list */

tid_t parent_tid;                   /* 父进程的tid */

- [NEW] `struct list child_list`
  - 定义子进程列表来储存父进程的所有子进程
- [NEW] `struct list_elem cpelem`
  - 定义child_list 的 elem 
- [NEW] `tid_t parent_tid`
  - 定义父进程的tid

### ALGORITHMS

> A2: Briefly describe how you implemented argument parsing.  How do you arrange for the elements of argv[] to be in the right order? How do you avoid overflowing the stack page?

> A2: 简要描述你是怎么实现Argument parsing的。你是如何安排argv[]中的elements，使其在正确的顺序的？你是如何避免stack page的溢出的？

第一个部分是参数解析Argument Parsing的问题。

Process_execute 提供的file_name 包括了 命令command和arguments string。首先，我们先将第一个token和其他剩余部分分开来，这样就分成了两部分，分别为命令和参数串。用command的名字命名新的thread，随后传递参数给start_process(),load()以及setup_stack()。在其他部分需要这个command名字的时候，总是可以从thread名字处获取。

当设置stack的时候，优先处理参数串和实际上是线程名称的命令名称。随后进行对齐、扫描等操作，将剩下的参数串分成的每个token的地址都添加进page中，生成argv[],argc，return addr等东西。

第二个部分是避免堆栈页面溢出的问题。

我们在实施之前并没有预先计算出所需要的空间，但是，我们在实际处理溢出的时候，采用了这种办法，就是在每次使用esp之前，检查esp的有效性。

### RATIONALE

> A3: Why does Pintos implement strtok_r() but not strtok()?

> A3: 为什么Pintos中实现strtok_r()而不是strtok()？

strtok_r() 和strtok()之间的唯一区别就是save_ptr(). save_ptr() 在 streak_r()中提供了一个占位符。在pintos中，内核可以将命令行分为命令/文件名和参数串两个部分，所以我们需要把参数的地址存放在之后可以获取到的地方。

> A4: In Pintos, the kernel separates commands into a executable name and arguments.  In Unix-like systems, the shell does this separation.  Identify at least two advantages of the Unix approach.

> A4: 在Pintos中，kernel将命令分成了可执行文件的name以及参数。在Unix-like的系统中，shell完成这部分的分隔。列举至少2种Unix这样做的好处。
>
> 第一个好处是，这可以缩短内核内部运行的时间。第二个好处是，这可以在讲命令传递给下一部分程序之前，检查参数是否超过限制以避免kernel fail。第三点好处是，分离参数和命令以便于进行更高级的预处理。

## QUESTION 2: SYSTEM CALLS

### 需求分析

初始操作系统中已经有了支持加载和运行用户程序的功能，但是没有与用户进行I/O交互的功能，所以我们需要完善我们的代码来实现用户可以通过命令行的形式来运行自己想要运行的程序，并且可以在命令行中传入一定的参数，实现人机交互。在之前的项目当中，我们直接通过命令行使得程序直接在内核之中编译运行，这显然是不安全的，系统内核涉及到整个操作系统的安全性，尽管在我们的项目当中我们可以轻易的在内核中直接编译，但是提供程序接口给用户，来实现程序同系统直接按的通信是十分有必要的。典型的系统调用有halt, exit, exec, wait, create, remove, open, filesize, read, write, seek, tell, close等，这些也是文档中要求我们完成的部分。

### 设计思路

通过阅读Pintos操作系统的官方文档，我们可以知道系统调用时会传入参数f，其类型为`struct intr_frame`，并且指针的解引用的值为`lib/syscall-nr.h`中已经定义好的枚举变量，我们需要根据不同的枚举变量来执行不同的函数，所以这里采用一个switch选择器进行选择是十分合理的。对于不同的函数系统调用函数而言，我们第一步的工作都是根据不同的指针类型对传入的值进行检查，所以需要先写出检查函数`pointer_valid`和`char_pointer_valid`对传入的参数进行检查。在打开文件时创建结构体`struct file f`来保存`fd`的值和文件名，并且将结构体中的`elem`存入当前线程的`fd_list`。在`read, filesize, write, tell, close, seek`系统调用都需要根据`fd->num`来获取`fd`，这促使我们将这一过程抽象成函数`find_fd_by_num(int num)`函数，以方便我们对代码进行检查。在执行系统调用函数之后，仍然需要对传入的参数进行检查空指针和个数检查，这就促使我们将每一个系统调用函数铲拆分成两部分，第一部分为检查参数部分，而第二部分为具体的执行代码，使得结构整体上十分清晰。具体的系统调用函数参照官方文档所述进行编写即可。

### DATA STRUCTURES

> B1: Copy here the declaration of each new or changed `struct` or `struct' member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

- [NEW] `struct lock file_lock`
  - 定义文件锁来限制多个线程同时修改同一个文件
- [NEW] `typedef int pid_t`
  - 定义线程标识号的类型
- [NEW] `int fd_num`
  - 定义非负整数file descriptor
- [NEW] `struct fd`
  - 定义新的结构体来表示一个线程打开的文件
- [NEW] `struct thread* parent`
  - 表示线程的父进程
- [NEW] `struct list fd_list` in `struct thread`
  - 表示线程拥有的fd列表
- [NEW] `struct list fd_list` in `struct thread`
  - 表示线程拥有的fd列表
- [NEW] `struct list child_status` in `struct thread`
  - 表示子进程状态列表
- [NEW] `struct file *execfile `in `struct thread`
  - 表示线程正在执行的文件
- [NEW] `struct child_process_status *relay_status; `in `struct thread`
  - 表示转发给父进程的子进程状态 
- [NEW] `struct semaphore sema;`in `struct thread`
  - 表示子进程等待的信号量
- [NEW] `struct int ret_status` in `struct child_process_status`
  - 表示子进程的返回状态
- [NEW] `struct int tid` in `struct child_process_status`
  - 表示子进程的tid
- [NEW] `struct thread* child ` in   `struct child_process_status`
  - 表示指向子进程的指针
- [NEW] `bool finish ` in   `struct child_process_status`
  - 表示子进程是否完成的状态
- [NEW] `bool iswaited` in `struct child_process_status`
  - 表示子进程是否等待的状态
- [NEW] `bool loaded` in `struct child_process_status`
  - 表示子进程是否等待的状态
- [NEW] `struct list_elem elem` in `struct child_process_status`
  - 表示子进程状态结构体的列表元素

> B2: Describe how file descriptors are associated with open files. Are file descriptors unique within the entire OS or just within a single process?

文件描述符通过维护一个唯一的非负整数且不唯零和一的方法来对文件进行联系。我们需要保存文件和`fd`之间对应关系，这就促使我们将其打包成一个结构体，同时是打开的文件是属于某个进程的，这就需要我们来定义一个列表来对这个整体进行维护，同时在整体中出入`struct list_elem elem`元素即可实现列表。文件描述符在整个OS中都是唯一的，因为我们这里在`syscall.c`中维护一个起始值为2的全局变量，每一次打开一个文件就加一，这样就简单实现了文件描述符的唯一性。

> B2: 描述文件描述符是如何与打开文件相联系的。文件描述符是在整个中唯一还是仅在单个进程中唯一？

### ALGORITHMS

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

### SYNCHRONIZATION

> B7: The "exec" system call returns -1 if loading the new executable fails, so it cannot return before the new executable has completed loading.  How does your code ensure this?  How is the load success/failure status passed back to the thread that calls "exec"?

> B7: 如果新的可执行文件加载失败，"exec"系统调用会返回-1，所以它不能够在该新的可执行文件成功加载之前返回。你的代码是如何保证这一点的？加载成功/失败的状态是如何传递回调用"exec"的线程的？

> B8: Consider parent process P with child process C.  How do you ensure proper synchronization and avoid race conditions when P calls wait(C) before C exits?  After C exits?  How do you ensure that all resources are freed in each case?  How about when P terminates without waiting, before C exits?  After C exits?  Are there any special cases?

> B8: 考虑有父进程P和它的子进程C。当P在Cexit之前调用wait(C)时，你如何确保同步以及如何避免争用的情况？你如何确保在每种情况下，所有的资源都被释放？如果P在C exit之前，没有waiting便终止？如果在C exit之后？有什么特殊情况吗？

### RATIONALE

> B9: Why did you choose to implement access to user memory from the kernel in the way that you did?

> B9: 为什么你使用这种方式来实现从内核对用户内存的访问？

> B10: What advantages or disadvantages can you see to your design for file descriptors?

> B10: 你对文件描述符的设计有什么优劣吗？

> B11: The default tid_t to pid_t mapping is the identity mapping. If you changed it, what advantages are there to your approach?

> B11: 默认的tid_t到pid_t的映射是identity mapping。如果你进行了更改，那么你的方法有什么优点？

## 核心：文件同步

### 解决思路

### 关键代码解析
## 核心：进程同步

`struct child_process_status *relay_status`传递子进程给父进程的信息。目的是为了父进程若未退出，而子进程却退出了，使子进程退出时，子进程的相关信息仍然保留。

### 解决思路

#### 初版

在初级版本中，我们考虑到，在进行本项目的过程中，有着进程同步的需求，而这个需求无疑是十分必要的。

因此，我们首先简单地思考了一下进程同步的问题，发现需要一个能够使进程等待的函数，来帮助我们完成进程同步的需求。

本组同学在`process_wait()`中，添加/修改代码。在直观的思维理解中，必然是做出了这样的策略：父进程利用`process_excute()`创建子进程，随后在`process_wait()`中，父进程等待子进程结束，返回子进程的tid。

```c++
 struct thread* child = get_child_by_tid(&thread_current()->child_list,child_tid);
  if(child==NULL)return -1; /* not child_tid */
  int ret = child->ret;
  //printf("child thread info:tid:%d name:%s parent_id:%d\n",child->tid,child->name,child->parent_tid);
  while(child!=NULL){
    thread_yield();
    child = get_child_by_tid(&thread_current()->child_list,child_tid);
  }
  return ret;
```

这是一种相对来说非常直观并且直接的方法。这种方法用到了一些新的结构。

```c++
struct list child_list;             /* 子进程列表 */
struct list_elem cpelem;            /* elem for child_list */
tid_t parent_tid;                   /* 父进程的tid */
```

当然，这种直观的方法存在某些问题。这些问题和解决这些问题的方法，将在下一部分重构中，详细的讲解。

#### 重构

当我们创建一个子进程后，子进程要执行加载，但只当子进程从start_process()运行至load()后，我们才知道子进程是否真的加载成功。所以我们要保证子进程要运行至load()，才能得知子进程的加载成功与否的状态。需求中提到：如果子进程加载成功，便返回子进程的tid；如果子进程加载失败，则返回-1。在我们的初版代码中

### 关键代码解析

## SURVEY QUESTIONS

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
