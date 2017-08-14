KQClient
========

Small file-system watcher for BSD.


### make
```make
$ make all
cc -O2 -Weverything -c kqclient.c
cc -O2 -Weverything -o kqclient kqclient.o
TESTS: Passed.
```

### Test-drive

```
$ touch 1
$ ./kqclient 1 &
$ touch 1
/usr/home/you/kq/1 UNKNOWN_EVENT 1 (8)
$ mv 1 2
/usr/home/you/kq/1 RENAME 1 (32)
$ echo 3 > 2
/usr/home/you/kq/1 WRITE 1 (14)
$ mv 2 1
/usr/home/you/kq/1 RENAME 1 (32)
$ rm 1
/usr/home/you/kq/1 DELETE 1 (1)
$ fg
./kqclient 1
^C
```

Credits
-------

* Based on Jonathan Lemons example.
