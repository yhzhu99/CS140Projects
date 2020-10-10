# CS140 Projects
> OS Assignment

[CS 140: Operating Systems (Spring 2020)](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/index.php)

## Projects

[Problem Set 0: Synchronization](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/problemSet0.php)

[Project 1: Threads](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)

[Project 2: User Programs](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)

[Project 3: Virtual Memory](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)

[Project 4: File Systems](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintosProjects.php)

### Project 1: Threads

See the Pintos documentation for details on this project:

- [Assignment](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html)
- [Frequently asked questions](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/pintos_2.html#Project 1 FAQ)
- [Template for design document](http://web.stanford.edu/~ouster/cgi-bin/cs140-spring20/pintos/threads.tmpl)

Additional notes and modifications for Project 1:

- A common mistake students make throughout the Pintos projects is to use `malloc` carelessly. If you call `malloc`, you must check the result to make sure the system did not run out of memory (and you must do something reasonable if memory does run out). In addition, you must be sure that any memory you allocate is eventually freed.