
CC = gcc
CFLAGS = -g -Wall -Werror

TARGETS = proj5 proj5d

# object files for each target
OBJS_proj5 = proj5.o
OBJS_proj5d = proj5d.o write_to_file.o random_joke.o list.o create.o edit.o delete_joke.o dadsay.o

all: $(TARGETS)

proj5: $(OBJS_proj5)
	$(CC) $(CFLAGS) -o $@ $^

proj5d: $(OBJS_proj5d)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

create.o: write_to_file.c write_to_file.h
	$(CC) $(CFLAGS) -c create.c

edit.o: write_to_file.c write_to_file.h
	$(CC) $(CFLAGS) -c edit.c

dadsay.o: dadsay.c random_joke.h
	$(CC) $(CFLAGS) -c dadsay.c

clean:
	rm -f $(TARGETS) *.o
	rm -rf *.dSYM

distclean: clean
