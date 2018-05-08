.PHONY: memtest clean

CC = clang
CFLAGS = -g


SRCS = linkedlist.c talloc.c tokenizer.c main_tokenize.c
HDRS = linkedlist.h value.h talloc.h tokenizer.h

OBJS = $(SRCS:.c=.o)

linkedlist: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
    
tokenizer: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

memtest: tokenizer
	valgrind --leak-check=full --show-leak-kinds=all ./$<    

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f linkedlist
	rm -f tokenizer
