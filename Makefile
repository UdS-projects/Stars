.PHONY: all clean

LDFLAGS=-fsanitize=address

all: stars test

stars: star.o main.o image.o
	$(CC) $(LDFLAGS) -o $@ $^

test: test.o image.o star.o
	$(CC) $(LDFLAGS) -o $@ $^
	
%.o: %.c
	$(CC) -O0 -g -Wall -pedantic -fsanitize=address --std=c99 -o $@ -c $<

clean:
	rm -f *.o *.pbm stars test
