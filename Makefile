.PHONY: memtest clean

CC = clang
CFLAGS = -g


SRCS = linkedlist.c talloc.c tokenizer.c parser.c main_parse.c
HDRS = linkedlist.h value.h talloc.h parser.h tokenizer.h

OBJS = $(SRCS:.c=.o)

linkedlist: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
tokenizer: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
parser: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

memtest: parser
	valgrind --leak-check=full --show-leak-kinds=all ./$<    

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f linkedlist
	rm -f tokenizer
	rm -f parser