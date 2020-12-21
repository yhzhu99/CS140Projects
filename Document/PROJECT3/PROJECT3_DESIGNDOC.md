# PROJECT 3: VIRTUAL MEMORY DESIGN DOCUMENT

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

![](img/init-test.png)

113个测试点中，通过了113-26=87个点（87/113*100%=76.99%）

## PRELIMINARIES

> If you have any preliminary comments on your submission, notes for the TAs, or extra credit, please give them here.

我们该项目的Github仓库为私有仓库，如有需要，请联系我们。

由于我们并未完全实现Project3的所有内容，以下DesignDoc中的问题将非常坦诚地仅回答我们已实现的部分或是对该部分的理解。

需要注意的是，我们的代码**没有参考**任何网络上的现成代码，完全基于我们的Project2项目。

**给助教的提醒：**

- 如果某些组Project2是以方式二形式做的，而以方式一完成了Project3，则我有90%+的把握确信他们是抄的。
- 如果某些组Project3与Project2都以方式一做，然而Project3的代码并不基于Project2（Project2中的实现方式多样——用一个信号量同步/用两个信号量同步/使用管道来同步等，可以看`thread.h`中定义的数据结构），则我同样有90%+的把握确信他们是抄的。

> Please cite any offline or online sources you consulted while preparing your submission, other than the Pintos documentation, course text, lecture notes, and course staff.

1. 操作系统概念(原书第9版)/(美)Abraham Silberschatz等著
2. 原仓周老师PPT中的概念和课上讲解

## QUESTION 1: PAGE TABLE MANAGEMENT

### 需求分析

**分页部分：**

为从可执行文件加载的段实现分页。所有这些页面都应延迟加载，即仅在内核拦截它们的页面错误时才加载。逐出后，应将自加载以来已修改的页面（例如，如“脏位”所示）写入交换页。未经修改的页面（包括只读页面）绝对不能写入交换位置，因为它们始终可以从可执行文件中读取。

实现近似LRU的全局页面替换算法。您的算法至少应具有“第二次机会”或“时钟”算法的简单变体。

您的设计应考虑并行性。如果一页错误需要I/O，则与此同时，没有错误的进程应继续执行，而其他不需要I/O的页面错误应能够完成。这将需要一些同步工作。

您需要修改程序加载器的核心，即`userprog/process.c`中`load_segment()`中的循环。每次循环时，`page_read_bytes`都会从可执行文件中读取要读取的字节数，`page_zero_bytes`会在读取的字节后接收初始化为零的字节数。这两个总和为`PGSIZE(4,096)`。页面的处理取决于以下变量的值：

- 如果`page_read_bytes`等于`PGSIZE`，则应在首次访问时从基础文件中请求对页面进行分页。
- 如果`page_zero_bytes`等于`PGSIZE`，则根本不需要从磁盘读取页面，因为它全为零。您应该通过在首页错误时创建一个由全零组成的新页面来处理此类页面。
- 否则，`page_read_bytes`和`page_zero_bytes`都不等于`PGSIZE`。在这种情况下，将从底层文件中读取页面的初始部分，并将其余部分清零。


### DATA STRUCTURES

> A1: Copy here the declaration of each new or changed `struct` or `struct` member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

在本部分中，添加的新的数据结构或者是修改已有的数据结构。有：

**in `thread.h`**

- [NEW] `struct list child_list`
  - 定义子进程列表来储存父进程的所有子进程
- [NEW] `struct list_elem cpelem`
  - 定义`child_list`的`elem`
- [NEW] `tid_t parent_tid`
  - 定义父进程的`tid`

**in `process.c`**

- [CHANGED] `process_execute()`
  - implement starts a new thread running a user program loaded from FILENAME.
- [CHANGED] `start_process()`
  - implement 参数传递

### ALGORITHMS

> A2: In a few paragraphs, describe your code for accessing the data stored in the SPT about a given page.

很遗憾，由于我们尚并未完成这一部分。

> A3: How does your code coordinate accessed and dirty bits between kernel and user virtual addresses that alias a single frame, or alternatively how do you avoid the issue?

### SYNCHRONIZATION

> A4: When two user processes both need a new frame at the same time, how are races avoided?

### RATIONALE

> A5: Why did you choose the data structure(s) that you did for representing virtual-to-physical mappings?

## QUESTION 2: PAGING TO AND FROM DISK


### DATA STRUCTURES

> B1: Copy here the declaration of each new or changed `struct` or `struct` member, global or static variable, `typedef`, or enumeration.  Identify the purpose of each in 25 words or less.

**全局变量**

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

**in `struct thread`**

- [NEW] `struct list fd_list`
  - 表示线程拥有的fd列表
- [NEW] `struct list fd_list`
  - 表示线程拥有的fd列表
- [NEW] `struct list child_status`
  - 表示子进程状态列表
- [NEW] `struct file *execfile`
  - 表示线程正在执行的文件
- [NEW] `struct child_process_status *relay_status`
  - 表示转发给父进程的子进程状态
- [NEW] `struct semaphore sema`
  - 表示子进程等待的信号量

**in `struct child_process_status`**

- [NEW] `struct int ret_status`
  - 表示子进程的返回状态
- [NEW] `struct int tid`
  - 表示子进程的tid
- [NEW] `struct thread* child`
  - 表示指向子进程的指针
- [NEW] `bool finish`
  - 表示子进程是否完成的状态
- [NEW] `bool iswaited`
  - 表示子进程是否等待的状态
- [NEW] `bool loaded`
  - 表示子进程是否等待的状态
- [NEW] `struct list_elem elem`
  - 表示子进程状态结构体的列表元素

### ALGORITHMS

> B2: When a frame is required but none is free, some frame must be evicted.  Describe your code for choosing a frame to evict.

> B3: When a process P obtains a frame that was previously used by a process Q, how do you adjust the page table (and any other data structures) to reflect the frame Q no longer has?

> B4: Explain your heuristic for deciding whether a page fault for an invalid virtual address should cause the stack to be extended into the page that faulted.

### SYNCHRONIZATION

> B5: Explain the basics of your VM synchronization design. In particular, explain how it prevents deadlock.  (Refer to the textbook for an explanation of the necessary conditions for deadlock.)

> B6: A page fault in process P can cause another process Q's frame to be evicted.  How do you ensure that Q cannot access or modify the page during the eviction process?  How do you avoid a race between P evicting Q's frame and Q faulting the page back in?

> B7: Suppose a page fault in process P causes a page to be read from the file system or swap.  How do you ensure that a second process Q cannot interfere by e.g. attempting to evict the frame while it is still being read in?

> B8: Explain how you handle access to paged-out pages that occur during system calls.  Do you use page faults to bring in pages (as in user programs), or do you have a mechanism for "locking" frames into physical memory, or do you use some other design?  How do you gracefully handle attempted accesses to invalid virtual addresses?

### RATIONALE

> B9: A single lock for the whole VM system would make synchronization easy, but limit parallelism.  On the other hand, using many locks complicates synchronization and raises the possibility for deadlock but allows for high parallelism. Explain where your design falls along this continuum and why you chose to design it this way.

## QUESTION 3: MEMORY MAPPED FILES

## 需求分析

实现内存映射文件，包括以下系统调用。

- 系统调用：`mapid_t mmap(int void *addr)`
  - 将以fd打开的文件映射到进程的虚拟地址空间。整个文件被映射到从addr开始的连续虚拟页面中。
  - 您的VM系统必须延迟加载mmap区域中的页面，并将mmaped文件本身用作映射的后备存储。也就是说，逐出mmap映射的页面会将其写回到映射的文件中。
  - 如果文件的长度不是PGSIZE的倍数，则最终映射页中的某些字节会“伸出”到文件末尾。当页面从文件系统中出现故障时，将这些字节设置为零，而当页面写回到磁盘时将其丢弃。
  - 如果成功，此函数将返回一个“映射ID”，该ID唯一标识进程中的映射。失败时，它必须返回-1，否则不应为有效的映射ID，并且进程的映射必须保持不变。
  - 如果以fd打开的文件的长度为零字节，则对mmap的调用可能会失败。如果addr不是页面对齐的，或者映射的页面范围与任何现有的映射页面集（包括堆栈或可执行加载时映射的页面）重叠，则必须失败。如果addr为0，它也必须失败，因为某些Pintos代码假定未映射虚拟页面0。最后，代表控制台输入和输出的文件描述符0和1是不可映射的。
- 系统调用：`void munmap (mapid_t mapping)`
  - 取消映射由映射指定的映射，该映射必须是由尚未取消映射的同一进程先前调用mmap返回的映射ID。
  - 进程退出时，无论是通过退出还是通过任何其他方式，都将隐式取消对所有映射的映射。当取消映射时，无论是隐式还是显式地，该进程写入的所有页面都将被写回到文件中，而未写入的页面则不能被写入。然后，将页面从进程的虚拟页面列表中删除。
  - 关闭或删除文件不会取消映射任何映射。创建后，按照Unix约定，映射将一直有效，直到调用munmap或进程退出为止。有关更多信息，请参见删除打开的文件。您应该使用file_reopen函数为文件的每个映射获取对文件的单独且独立的引用。
  - 如果两个或多个进程映射同一文件，则不要求它们看到一致的数据。 Unix通过使两个映射共享相同的物理页面来处理此问题，但是mmap系统调用还具有一个参数，允许客户端指定页面是共享页面还是私有页面（即写时复制）。

进程退出时，无论是通过退出还是通过任何其他方式，都将隐式取消对所有映射的映射。 当取消映射时，无论是隐式还是显式，由该进程写入的所有页面都将被写回到该文件中，而未写入的页面则必须不被写入。 然后，将页面从进程的虚拟页面列表中删除。

关闭或删除文件不会取消映射任何映射。创建后，按照Unix约定，映射将一直有效，直到调用munmap或进程退出为止。有关更多信息，请参见删除打开的文件。您应该使用file_reopen函数为文件的每个映射获取对文件的单独且独立的引用。

如果两个或多个进程映射同一文件，则不要求它们看到一致的数据。Unix通过使两个映射共享相同的物理页面来处理此问题，但是mmap系统调用还具有一个参数，允许客户端指定页面是共享页面还是私有页面（即写时复制）。

### ALGORITHMS

> C2: Describe how memory mapped files integrate into your virtual memory subsystem.  Explain how the page fault and eviction processes differ between swap pages and other pages.

> C3: Explain how you determine whether a new file mapping overlaps any existing segment.

### RATIONALE

> C4: Mappings created with "mmap" have similar semantics to those of data demand-paged from executables, except that "mmap" mappings are written back to their original files, not to swap.  This implies that much of their implementation can be shared. Explain why your implementation either does or does not share much of the code for the two situations.

## SURVEY QUESTIONS

Answering these questions is optional, but it will help us improve the course in future quarters. Feel free to tell us anything you want--these questions are just to spur your thoughts. You may also choose to respond anonymously in the course evaluations at the end of the quarter.

> In your opinion, was this assignment, or any one of the three problems in it, too easy or too hard?  Did it take too long or too little time?

这个作业对我们而言非常难。我们基本上整整10天其他什么作业都没做，只用来理解和写Pintos。

> Did you find that working on a particular part of the assignment gave you greater insight into some aspect of OS design?

是的。尤其是`process_wait()`的部分，如何引入信号量，如何周全地考虑父子进程先后退出的同步问题，如何适当地释放资源，这些考虑使我们对OS的理解得到了很大的提升。

> Is there some particular fact or hint we should give students in future quarters to help them solve the problems?  Conversely, did you find any of our guidance to be misleading?

实验指导书已经足够明确，我们组在做本次Project 3的唯一参考基本上就是该实验指导书。

> Do you have any suggestions for the TAs to more effectively assist students, either for future quarters or the remaining projects?

希望助教能给我们更多的指导，比如在开始的时候带领我们理解一下实验的任务要求，用户进程相关的知识背景，以及核心代码的部分讲解。

> Any other comments?

感谢在实验过程中解答我们问题的每一位助教！
