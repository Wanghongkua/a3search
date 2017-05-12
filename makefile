program_C_SRCS := $(wildcard *.c)

ALL:a3search

a3search: ${program_C_SRCS}
	gcc -o a3search -O3 ${program_C_SRCS} -I.


debug: ${program_C_SRCS}
	gcc -o a3search -g ${program_C_SRCS} -I.

clean:
	rm -f *.o a3search
