# Pintos Project 1 

> 包含对函数的解析，宏定义等

### 1. devices/times.h times.c

宏定义 *TIMER_FREQ 100*

1.  void timer_init(void) 

   ```C
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

16. static void real_time_sleep (int64_t num, int32_t denom);

17. static void real_time_delay (int64_t num, int32_t denom);

