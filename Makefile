all: schedule two

schedule: schedule.c queue.c
	gcc -Wall -g -o schedule schedule.c queue.c

two: two.c
	gcc -Wall -g -o two two.c

.PHONY: clean
clean:
	rm -f schedule two