##
# SaneTrace
#
# @file
# @version 0.1

all: sanetrace test1 test2 test3 test4 test5

sanetrace: main.c
	clang -o sanetrace -g main.c

test1: tests/test1.c
	clang -o test1 tests/test1.c

test2: tests/test2.c
	clang -o test2 tests/test2.c

test3: tests/test3.c
	clang -o test3 tests/test3.c

test4: tests/test4.c
	clang -o test4 tests/test4.c

test5: tests/test5.c
	clang -o test5 tests/test5.c

clean:
	rm *~ sanetrace test1 test2 test3 test4
# end
