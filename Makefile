
CC = gcc
CFLAGS = -g -Wall -Werror

TARGETS = proj5 proj5d

# object files for each target
OBJS_proj5 = proj5.o
OBJS_proj5d = proj5d.o list.o random_joke.o create.o

all: $(TARGETS)

proj5: $(OBJS_proj5)
	$(CC) $(CFLAGS) -o $@ $^

proj5d: $(OBJS_proj5d)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o
	rm -rf *.dSYM

distclean: clean
