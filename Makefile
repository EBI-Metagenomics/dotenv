CC ?= cc
CFLAGS += -g -Wall -Wextra -std=c99

OBJECTS := dotenv.o test_dotenv.o

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

test_dotenv: $(OBJECTS) test.valid.env
	$(CC) $(OBJECTS) -o test


test: test_dotenv
	./test

.PHONY: clean
clean:
	rm -f $(OBJECTS) test
