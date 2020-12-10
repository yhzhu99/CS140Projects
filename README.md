# CS140 Projects
> OS Assignment

[CS 140: Operating Systems (Spring 2020)](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/index.php)

## Projects

- [x] [Problem Set 0: Synchronization](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/problemSet0.php)
- [x] [Project 1: Threads](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)
- [x] [Project 2: User Programs](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)
- [ ] [Project 3: Virtual Memory](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)
- [ ] [Project 4: File Systems](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)

### Project 1: Threads

See the Pintos documentation for details on this project:

- [Assignment](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html)
- [Frequently asked questions](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html#Project%201%20FAQ)
- [Template for design document](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/threads.tmpl)

Additional notes and modifications for Project 1:

- A common mistake students make throughout the Pintos projects is to use `malloc` carelessly. If you call `malloc`, you must check the result to make sure the system did not run out of memory (and you must do something reasonable if memory does run out). In addition, you must be sure that any memory you allocate is eventually freed.

### Projects 2: User Programs

See the Pintos documentation for details on this project:

- [Assignment](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html)
- [Frequently asked questions](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html#Project%201%20FAQ)
- [Template for design document](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/threads.tmpl)

## 评分标准

方式一的评测依据包括设计方案、文档质量和测试用例的通过数。采用这种方式，除提交能够直接编译并运行的完整项目源代码外，同时提交的文档第（1）部分为小组成员学号姓名及各自在本实验的贡献百分比，截图github或其他所采用平台的各成员工作总结。然后后续内容按实验规定的文档模板编写即可。选用这种方式的小组，源代码需要能通过查重检测，重合度高的源码视为作弊，相应取消本实验成绩。

方式二的评测依据主要是分析文档中分析相关函数的个数和分析质量，也参考源码编译后测试用例的通过数。采用这种方式，除提交能够直接编译并运行的完整项目源代码外，同时提交的分析文档内容包括：

- 小组成员学号姓名及各自在本实验的贡献百分比；
- 所分析代码通过本实验测试用例的情况；
- 实验相关函数的调用关系图及其功能说明；
- 相关数据结构的分析说明；
- 相关函数流程图的分析说明。
