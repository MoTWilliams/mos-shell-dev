CC=gcc

# Find all header files in src/ and its subdirectories. Only include each once.
INCLUDES = $(shell find src test -name '*.h' -exec dirname {} \; | sort -u)

# Turn all include directories into -I flags.
# Enforce C89 standard and enable all common warnings.
CFLAGS = -std=c89 -pedantic -Wall -Wextra -g $(addprefix -I, $(INCLUDES))

# Recursively include all .c files in src/
SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(SRC:.c=.o)

# For unit tests
TEST = $(wildcard test/*.c test/*/*.c test/*/*/*.c)
LIB_OBJ = $(OBJ)
LIB_ARCHIVE = libMoShell.a

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

moshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(LIB_ARCHIVE): $(LIB_OBJ)
	ar rcs $@ $^

TEST_OBJ = $(TEST:.c=.o)
TEST_BIN = runTests

$(TEST_BIN): $(TEST_OBJ) $(LIB_ARCHIVE)
	$(CC) -o $@ $^ $(CFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TEST_BIN) $(LIB_ARCHIVE)
	rm -f moshell

uninstall:
	@{ \
		if [ -f "$(HOME)/.local/bin/moshell" ]; then \
			rm ~/.local/bin/moshell; \
			if [ ! -f "$(HOME)/.local/bin/moshell" ]; then \
				echo "Mo's Shell successfully uninstalled."; \
			fi; \
		else \
			echo "Mo's Shell is not installed. Nothing to uninstall."; \
		fi; \
	}

install: moshell
	@{ \
		make uninstall; \
		cp moshell ~/.local/bin/moshell; \
		make clean; \
		if [ -f "$(HOME)/.local/bin/moshell" ]; then \
			echo "Mo's Shell successfully installed. Use 'moshell' to run."; \
		fi; \
	}

