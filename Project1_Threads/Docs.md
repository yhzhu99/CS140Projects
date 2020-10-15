# Pintos Project 1 

> 包含对函数的解析，宏定义等

### 1. devices/times.h times.c

宏定义 *TIMER_FREQ 100*

1. void timer_init(void) 

   ```c
     /*建立timer使得每秒钟重段TIMER_FREQ次，并且在相应的中断登记)*/
      /* 0表示Channel0,Channel0和0号中断连接，使得可以用于周期性时钟中断(timer interrupt),2表示是一个周期性的暂停，这对于连接到一个中断控制器并由此生成周期性中断是有帮助的*/
      pit_configure_channel(0, 2, TIMER_FREQ);
      /*登记，对debug有帮助*/
      intr_register_ext(0x20, timer_interrupt, "8254 Timer");
   ```

2. void timer_calibrate(void)

   ```c
   /* Calibrates loops_per_tick, used to implement brief delays. */
   /* 矫正 每个Tick的圈数，被用于实现简单延迟*/
   unsigned high_bit, test_bit;
     ASSERT (intr_get_level () == INTR_ON);
     printf ("Calibrating timer...  ");
   
     /* Approximate loops_per_tick as the largest power-of-two
        still less than one timer tick. */
     loops_per_tick = 1u << 10;
     while (!too_many_loops (loops_per_tick << 1)) 
       {
         loops_per_tick <<= 1;
         ASSERT (loops_per_tick != 0);
       }
   
     /* Refine the next 8 bits of loops_per_tick. */
     high_bit = loops_per_tick;
     for (test_bit = high_bit >> 1; test_bit != high_bit >> 10; test_bit >>= 1)
       if (!too_many_loops (high_bit | test_bit))
         loops_per_tick |= test_bit;
   
     printf ("%'"PRIu64" loops/s.\n", (uint64_t) loops_per_tick * TIMER_FREQ);
   ```

3. int64_t timer_ticks(void)

   ```c
   /* 返回自从PINTOS启动活的timer ticks*/
   /* 返回之前的中断状态并禁止中断（防止被修改ticks）*/
   enum intr_level old_level = intr_disable ();
   /* ticks 的类型为静态无符号整型 */
   int64_t t = ticks;
   /* 设置old_level*/
   intr_set_level (old_level);
   return t;
   ```

4. int64_t timer_elapsed(int64_t)

   ```C
   /* 返回从 then 时间点的差值*/
   /* Returns the number of timer ticks elapsed since THEN, which
      should be a value once returned by timer_ticks(). */
   int64_t
   timer_elapsed (int64_t then) 
   {
     return timer_ticks () - then;
   }
   ```

5. void timer_sleep(int64_t ticks);

   ```c
   /* Sleeps for approximately TICKS timer ticks.  Interrupts must
      be turned on. */
   /* The Original One: BUSY WAITING */
   void
   timer_sleep (int64_t ticks) 
   {
       int64_t start = timer_ticks ();
       ASSERT (intr_get_level () == INTR_ON);
       while (timer_elapsed (start) < ticks) 
         thread_yield ();
   }
   
   /* The Enhanced One: */
   void
   timer_sleep (int64_t ticks) 
   {
       if(ticks <= 0)  return;
       ASSERT (intr_get_level () == INTR_ON);
       enum intr_level old_level = intr_disable();
       struct thread* current_thread = thread_current();
       current_thread->ticks_blocked = ticks;
       thread_block();
       intr_set_level(old_level);
   }
   ```

6. void timer_msleep(int64_t milliseconds);

   ```c
   /* 下面3个函数运行时中断必须打开*/ 
   /* Sleeps for approximately MS milliseconds.  Interrupts must be
      turned on. */
   void
   timer_msleep (int64_t ms) 
   {
     real_time_sleep (ms, 1000);
   }
   ```

7. void timer_usleep(int64_t microseconds);

   ```c
   /* Sleeps for approximately US microseconds.  Interrupts must be
      turned on. */
   void
   timer_usleep (int64_t us) 
   {
     real_time_sleep (us, 1000 * 1000);
   }
   ```

8. void timer_nsleep(int64_t nanoseconds);

   ```c
   /* Sleeps for approximately NS nanoseconds.  Interrupts must be
      turned on. */
   void
   timer_nsleep (int64_t ns) 
   {
     real_time_sleep (ns, 1000 * 1000 * 1000);
   }
   ```

9. void timer_mdelay(int64_t milliseconds)

   ```c
   /* Busy-waits for approximately MS milliseconds.  Interrupts need
      not be turned on.
   
      Busy waiting wastes CPU cycles, and busy waiting with
      interrupts off for the interval between timer ticks or longer
      will cause timer ticks to be lost.  Thus, use timer_msleep()
      instead if interrupts are enabled. */
   void
   timer_mdelay (int64_t ms) 
   {
     real_time_delay (ms, 1000);
   }
   ```

10. void timer_udelay(int64_t microseconds)

    ```c
    /* Sleeps for approximately US microseconds.  Interrupts need not
       be turned on.
    
       Busy waiting wastes CPU cycles, and busy waiting with
       interrupts off for the interval between timer ticks or longer
       will cause timer ticks to be lost.  Thus, use timer_usleep()
       instead if interrupts are enabled. */
    void
    timer_udelay (int64_t us) 
    {
      real_time_delay (us, 1000 * 1000);
    }
    ```

11. void timer_ndelay(int64_t nanoseconds);

    ```c
    /* Sleeps execution for approximately NS nanoseconds.  Interrupts
       need not be turned on.
    
       Busy waiting wastes CPU cycles, and busy waiting with
       interrupts off for the interval between timer ticks or longer
       will cause timer ticks to be lost.  Thus, use timer_nsleep()
       instead if interrupts are enabled.*/
    void
    timer_ndelay (int64_t ns) 
    {
      real_time_delay (ns, 1000 * 1000 * 1000);
    }
    ```

12. void time_print_stats(void)

    ```c
    /* Prints timer statistics. */
    /* 打印系统自从启动的timer tick*/
    void
    timer_print_stats (void) 
    {
      printf ("Timer: %"PRId64" ticks\n", timer_ticks ());
    }
    ```

13. static intr_handler_func timer_interrupt;

    ```c
    /* Timer interrupt handler. */
    /* The Original Version: */
    static void
    timer_interrupt (struct intr_frame *args UNUSED)
    {
    /* 系统ticks++ */
      ticks++;
    /* 每一次被时钟中断处理器调用，由此，这个函数可以在外部中断上下文中运行*/
      thread_tick ();
    }
    
    /* The Enhanced One: */
    static void
    timer_interrupt (struct intr_frame *args UNUSED)
    {
    /* 系统ticks++ */
      ticks++;
    /* 每一次被时钟中断处理器调用，由此，这个函数可以在外部中断上下文中运行*/
      thread_tick ();
    /* 在所有线程中引用 "func"函数，通过aux传递，这个函数必须在中断停止时调用*/
      thread_foreach(check_blocked_time, NULL);
    }
    ```

14. static bool too_many_loops (unsigned loops);

    ```c
    /* Returns true if LOOPS iterations waits for more than one timer
       tick, otherwise false. */
    static bool
    too_many_loops (unsigned loops) 
    {
      /* Wait for a timer tick. */
      int64_t start = ticks;
      while (ticks == start)
        barrier ();
    
      /* Run LOOPS loops. */
      start = ticks;
      busy_wait (loops);
    
      /* If the tick count changed, we iterated too long. */
      barrier ();
      return start != ticks;
    }
    ```

15. static void busy_wait (int64_t loops);

    ```c
    /* Iterates through a simple loop LOOPS times, for implementing
       brief delays.
    
       Marked NO_INLINE because code alignment can significantly
       affect timings, so that if this function was inlined
       differently in different places the results would be difficult
       to predict. */
    /* 忙等待 */
    static void NO_INLINE
    busy_wait (int64_t loops) 
    {
      while (loops-- > 0)
        barrier ();
    }
    ```

16. static void real_time_sleep (int64_t num, int32_t denom);

    ```c
    /* Sleep for approximately NUM/DENOM seconds. */
    /* 大约睡眠 NUM/DENOM秒 */
    static void
    real_time_sleep (int64_t num, int32_t denom) 
    {
      /* Convert NUM/DENOM seconds into timer ticks, rounding down.
              
            (NUM / DENOM) s          
         ---------------------- = NUM * TIMER_FREQ / DENOM ticks. 
         1 s / TIMER_FREQ ticks
      */
      int64_t ticks = num * TIMER_FREQ / denom;
    
      ASSERT (intr_get_level () == INTR_ON);
      if (ticks > 0)
        {
          /* We're waiting for at least one full timer tick.  Use
             timer_sleep() because it will yield the CPU to other
             processes. */                
          timer_sleep (ticks); 
        }
      else 
        {
          /* Otherwise, use a busy-wait loop for more accurate
             sub-tick timing. */
          real_time_delay (num, denom); 
        }
    }
    ```

    

17. static void real_time_delay (int64_t num, int32_t denom);

    ```c
    /* Busy-wait for approximately NUM/DENOM seconds. */
    /* 忙等待大约 NUM/DENOM秒 */
    static void
    real_time_delay (int64_t num, int32_t denom)
    {
      /* Scale the numerator and denominator down by 1000 to avoid
         the possibility of overflow. */
      ASSERT (denom % 1000 == 0);
      busy_wait (loops_per_tick * num / 1000 * TIMER_FREQ / (denom / 1000)); 
    }
    ```

### 2. threads/thread.h thread.c

1. 宏定义

   ```c
   #ifndef THREADS_THREAD_H
   #define THREADS_THREAD_H
   /* Thread identifier type.
      You can redefine this to whatever type you like. */
   typedef int tid_t;
   #define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */
   
   /* Thread priorities. */
   #define PRI_MIN 0                       /* Lowest priority. */
   #define PRI_DEFAULT 31                  /* Default priority. */
   #define PRI_MAX 63                      /* Highest priority. */
   ```

2. 变量

   ```c
   /* States in a thread's life cycle. */
   enum thread_status
     {
       THREAD_RUNNING,     /* Running thread. */
       THREAD_READY,       /* Not running but ready to run. */
       THREAD_BLOCKED,     /* Waiting for an event to trigger. */
       THREAD_DYING        /* About to be destroyed. */
     };
   
   /* The `elem' member has a dual purpose.  It can be an element in
      the run queue (thread.c), or it can be an element in a
      semaphore wait list (synch.c).  It can be used these two ways
      only because they are mutually exclusive: only a thread in the
      ready state is on the run queue, whereas only a thread in the
      blocked state is on a semaphore wait list. */
   struct thread
     {
       /* Owned by thread.c. */
       tid_t tid;                          /* Thread identifier. */
       enum thread_status status;          /* Thread state. */
       char name[16];                      /* Name (for debugging purposes). */
       uint8_t *stack;                     /* Saved stack pointer. */
       int priority;                       /* Priority. */
       struct list_elem allelem;           /* List element for all threads list. */
   
       /* Shared between thread.c and synch.c. */
       struct list_elem elem;              /* List element. */
   
   #ifdef USERPROG
       /* Owned by userprog/process.c. */
       uint32_t *pagedir;                  /* Page directory. */
   #endif
   
       /* Owned by thread.c. */
       unsigned magic;                     /* Detects stack overflow. */
     
       /* Time for blocked */
       int64_t ticks_blocked;
     };
   
   /* If false (default), use round-robin scheduler.
      If true, use multi-level feedback queue scheduler.
      Controlled by kernel command-line option "-o mlfqs". */
   extern bool thread_mlfqs;
   ```

   3. thread_init(void);

   4. void thread_start(void)l

   5. void thread_tick(void);

   6. void thread_print_stats(void);

   7. typedef void thread_func(void *aux);

   8. tid_t thread_create(const char* name, int priority, thread_func*, void *);

   9. void thread_block(void);

   10. void thread_unblock(struct thread*)

   11. struct thread *thread_current(void);

   12. tid_t thread_tid(void);

   13. const char *thread_name(void);

   14. void thread_exit(void) NO_RETURN;

   15. void thread_yield(void);

       /* Performs some operation on thread t, given auxiliary data AUX. */

   16. typedfe void thread_action_func(struct thread* t, void *aux);

   17. void thread_foreach(thread_action_func*, void *)

   18. int thread_get_priority(void);

   19. void thread_set_priority(int);

   20. int thread_get_nice (void);

   21. void thread_set_nice(int);

   22. int thread_get_recent_cpu(void);

   23. int thread_get_load_avg(void);

   24. *own function* void check_blocked_time(struct thread* t, void* aux);

   25. 