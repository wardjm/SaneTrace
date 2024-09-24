##
# SaneTrace
#
# @file
# @version 0.1

all: sanetrace test

sanetrace: main.c
	clang -o sanetrace main.c

test: test.c
	clang -o test test.c

clean:
	rm *~ sanetrace test
# end
