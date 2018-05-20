.PHONY: memtest clean

CC = clang
CFLAGS = -g


SRCS = linkedlist.c talloc.c tokenizer.c parser.c interpreter.c main.c
HDRS = linkedlist.h value.h talloc.h parser.h tokenizer.h interpreter.h

OBJS = $(SRCS:.c=.o)

linkedlist: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
tokenizer: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
parser: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
interpreter: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

interpreter: $(OBJS)
	     $(CC) $(CFLAGS) $^ -o $@

memtest: interpreter
	valgrind --leak-check=full --show-leak-kinds=all ./$<    

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f linkedlist
	rm -f tokenizer
	rm -f parser
	rm -f interpreter
