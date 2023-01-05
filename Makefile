CC = gcc
CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c99 -Wall -Wextra -Wvla -Isrc -g

SRC = $(wildcard src/*/*.c src/*/*/*.c)
MAIN = src/main.c
MAIN_OBJ = $(MAIN:.c=.o)
SRC_OBJ = $(SRC:.c=.o)

TEST_SRC = $(wildcard tests/*/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)

EXE = 42sh
TEST_EXE = testsuite

all: $(EXE)

$(EXE): $(SRC_OBJ) $(MAIN_OBJ)
	$(CC) -o $(EXE) $^

debug: $(SRC_OBJ) $(MAIN_OBJ)
	$(CC) -o $(EXE) -fsanitize=address $^

test: $(SRC_OBJ) $(TEST_OBJ)
	$(CC) -o $(TEST_EXE) -lcriterion $^

$(TEST_SRC): %.o: %.c
	$(CC) $(CFLAGS) -lcriterion $< -o $@

clean:
	$(RM) -f $(SRC_OBJ) $(TEST_OBJ) $(MAIN_OBJ) $(EXE) $(TEST_EXE)
