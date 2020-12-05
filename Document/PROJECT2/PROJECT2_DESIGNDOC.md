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

我们该项目的Github仓库为私有仓库，如有需要，请联系我们。

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
第一个好处是，这可以缩短内核内部运行的时间。第二个好处是，这可以在讲命令传递给下一部分程序之前，检查参数是否超过限制以避免kernel fail。第三点好处是，分离参数和命令以便于进行更高级的预处理。

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

当我们创建一个子进程后，子进程要执行加载，但只当子进程从`start_process()`运行至`load()`后，我们才知道子进程是否真的加载成功。所以我们要保证子进程要运行至`load()`，才能得知子进程的加载成功与否的状态。需求中提到：如果子进程加载成功，便返回子进程的`tid`；如果子进程加载失败，则返回`-1`。在我们的初版代码中，不管子进程如何，我们都会直接返回。现在我们要想知道子进程是否加载成功，就要在子进程加载完毕后，再去判断其加载是否成功，然后才返回。

我们在重构版本的代码中，`process_wait()`部分改用了信号量来实现：信号量锁在上面，直到子进程执行完`loaded`之后，会在其执行失败时，通过`sema_up()`唤醒父进程；若执行成功，则唤醒父进程，此后父进程便可获取子进程是否加载成功的信息。

考虑以下需求：当`child_tid`不合法，则返回`-1`；当`child_tid`并不是你子进程的`tid`，返回`-1`；另外`child_tid`正在被其他进程等待，则返回`-1`。为了知道进程是否正在被别的进程等待，我们加入了新的数据结构`bool`量`iswaited`，表示进程是否被等。在`process_wait()`中，我们依次判断`child_tid`是否合法。当判断完后，令`is_waited=true`，表明进程处在等待状态。我们亦引入了`bool`量`finish`，表示进程是否已执行完。由于进程可能被多个子进程唤醒，故每次唤醒都要做判断，因此我们使用`while`，而非`if`。当执行完后，子进程不仅要向父进程返回其`status`，为了节省空间，还要把子进程的空间释放掉(`thread.h/struct child_process`)。

对于`process_exit()`：每次`exit`会进入到`syscall.c`中的`exit()`，又会进入到`thread.c`中的`thread_exit()`，其判断若在用户程序中，便会执行`process_exit()`。注意尽管`pagedir_destroy(pd)`，释放掉了进程的页，但`thread.h`中的`struct thread{}`没有被释放(`thread.h`中的`struct thread{}`在`schedule()`中被释放)。所以在`destroy`之后，仍然知道当前`process`的参数的，可以在`process_exit()`的后续来获取一些信息，如可以退出时输出`cur->name`, `cur->ret`。在退出时，亦会移出该进程的所有子进程，此时其所有子进程都会成为僵死进程，先前获取到的子进程的`status`便没有用了，于是遍历`pop`，把`child_status`释放。

最后是对僵死进程的处理。由于进程`exit`后，其`finish`置为`true`，要唤醒父亲，如果该进程为僵死进程，它没有父亲，则唤醒父亲的操作会引用出错(指针指向空的区域)，会造成`page fault`。所以每一次释放掉僵死进程所代表的`child`进程时，要将其指向的父进程的空间(`struct thread *parent`)置为`NULL`。这样便可以在`exit`时做判断：如果我有父亲，则唤醒，把需要传递的值赋给`relay_status`，即相当于父进程的`child_status`（我完成了，我的返回状态是什么，再把父亲给唤醒）

### 关键代码解析

**in `thread.h`**

```cpp
int loaded;
// 加载成功与否的状态：初始化为0，如果加载成功=1，如果加载失败=-1
bool iswaited;
/* if process_wait() has already
   been successfully called for the given TID, returns -1
   immediately, without waiting. */
bool finish; /* 子进程是否运行结束 */
struct child_process_status *relay_status 
// 传递子进程给父进程的信息。
// 目的是为了父进程若未退出，而子进程却退出了，使子进程退出时，子进程的相关信息仍然保留。
```

**in `process.c`**

```cpp
struct child_process_status *relay_status
// 传递子进程给父进程的信息。目的是为了父进程若未退出，而子进程却退出了，使子进程退出时，子进程的相关信息仍然保留。
```

```cpp
static void
start_process (void *file_name_){
  // ...
  /* 加载用户进程的eip和esp eip:执行指令地址 esp:栈顶地址 */
  success = load (token, &if_.eip, &if_.esp);
  if (!success){ // 子进程执行完loaded，如果加载失败，唤醒父进程
    thread_current()->relay_status->loaded = -1;
    sema_up(&thread_current()->parent->sema);
    exit(-1);
  }
  // ...
  // 执行成功，唤醒父进程，此后父进程便可获取子进程是否加载成功的信息
  thread_current()->relay_status->loaded = 1;
  sema_up(&thread_current()->parent->sema);
}
```

```cpp
tid_t
process_execute (const char *file_name){
  // ...
  /* 子进程创建成功 */
  struct child_process_status *child_status = get_child_status(tid);
  while(child_status->loaded == 0) // 子进程若还没有加载
  {
    sema_down(&thread_current()->sema); /* 阻塞，等待子进程执行完loaded */
  }
  if(child_status->loaded == -1) /* 此时子进程已经加载完毕 */
  {
    return -1;
  }
  return tid;
}
```

```cpp
int
process_wait (tid_t child_tid UNUSED){
  // 依次判断child_tid是否合法
  if(child_tid == TID_ERROR){
    return -1;            /* TID invalid */
  }
  struct child_process_status *child_status = get_child_status(child_tid);
  if(child_status == NULL){
    return -1;              /* not child_tid */
  }
  if(child_status->iswaited){
    return -1;
  }
  child_status->iswaited = true;
  // 判断完后，child_tid合法，令is_waited=true，表明进程为等待状态。
  while(!child_status->finish){
  // 子进程执行完后的状态：若finish!=true，还未结束 (当进程执行完，会将其状态置为true)
    sema_down(&thread_current()->sema); // 锁住
  }
  // 若子进程执行完，finish=true：
  int res = child_status->ret_status; // 父进程获取子进程的状态信息
  list_remove(&child_status->elem); // 释放空间
  free(child_status); // 把子进程从我拥有的child_list中移出
  return res; // 把父进程获取到的信息返回
}
```

```cpp
/* Free the current process's resources. */
void process_exit (void){
  struct thread *cur = thread_current ();
  uint32_t *pd;
  /* Destroy the current process's page directory and switch back
    to the kernel-only page directory. */
  pd = cur->pagedir;
  if (pd != NULL) {
    /* Correct ordering here is crucial.  We must set
        cur->pagedir to NULL before switching page directories,
        so that a timer interrupt can't switch back to the
        process page directory.  We must activate the base page
        directory before destroying the process's page
        directory, or our active page directory will be one
        that's been freed (and cleared). */
    cur->pagedir = NULL;
    pagedir_activate (NULL);
    pagedir_destroy (pd);
    // 释放进程的页，(thread.h中的struct thread{}没有被释放)
    // thread.h中的struct thread{}在schedule()中被释放
    // 所以在destroy之后，仍然知道当前process的参数的，可以在process_exit()的后续来获取一些信息
  }
}
```

**in `syscall.c`**

```cpp
void
exit(int status)
{
  close_all_fd();
  thread_current()->ret = status;
  // 当前线程的return status复制为参数中穿过来的status
  thread_exit();
}
```

**in `thread.c`**

```cpp
void
thread_exit (void){
  // ...
#ifdef USERPROG
  process_exit ();
  // 如果在用户程序中，执行process_exit()
#endif
  intr_disable ();
  struct thread *cur = thread_current();
  printf ("%s: exit(%d)\n",cur->name, cur->ret); /* 输出进程name以及进程return值 */ 
  if(cur->parent!=NULL){
    cur->relay_status->ret_status = cur->ret;
    cur->relay_status->finish = true;
    sema_up(&cur->parent->sema);
  }
  /* Remove thread from all threads list, set our status to dying,
    and schedule another process.  That process will destroy us
    when it calls thread_schedule_tail(). */
  while(!list_empty(&cur->child_status)){
    /*在退出时，亦要移出该进程的所有子进程。此时其所有子进程都会成为僵死进程，先前获取到的子进程的status便没有用了，于是遍历pop，把child_status释放。*/
    struct child_process_status *child_status= list_entry(list_pop_front(&cur->child_status),struct child_process_status, elem);
    child_status->child->parent = NULL;
    free(child_status); // 释放掉整个struct child_process_status
  }
  list_remove (&thread_current()->allelem);
  thread_current ()->status = THREAD_DYING;
  schedule(); // schedule中释放了当前线程的所有资源(整个struct thread{})
  NOT_REACHED ();
}
```

## SURVEY QUESTIONS

Answering these questions is optional, but it will help us improve the course in future quarters.  Feel free to tell us anything you
want--these questions are just to spur your thoughts.  You may also choose to respond anonymously in the course evaluations at the end of the quarter.

> In your opinion, was this assignment, or any one of the three problems in it, too easy or too hard?  Did it take too long or too little time?

这个作业对我们而言非常难。我们基本上整整7天其他什么作业都没做，只用来理解和写Pintos。

> Did you find that working on a particular part of the assignment gave you greater insight into some aspect of OS design?

是的。尤其是process_wait()的部分，如何引入信号量，如何周全地考虑父子进程先后退出的同步问题，如何适当地释放资源，这些考虑使我们对OS的理解得到了很大的提升。

> Is there some particular fact or hint we should give students in future quarters to help them solve the problems?  Conversely, did you find any of our guidance to be misleading?

实验指导书已经足够明确，我们组在做本次Project 2的唯一参考基本上就是该实验指导书。

> Do you have any suggestions for the TAs to more effectively assist students, either for future quarters or the remaining projects?

希望助教能给我们更多的指导，比如在开始的时候带领我们理解一下实验的任务要求，用户进程相关的知识背景，以及核心代码的部分讲解。

> Any other comments?

感谢在实验过程中解答我们问题的每一位助教！
