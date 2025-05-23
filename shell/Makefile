CC=gcc

# Find all header files in src/ and its subdirectories. Only include each once.
INCLUDES = $(shell find src -name '*.h' -exec dirname {} \; | sort -u)

# Turn all include directories into -I flags.
# Enforce C89 standard and enable all common warnings.
CFLAGS = -std=c89 -pedantic -Wall -Wextra -g $(addprefix -I, $(INCLUDES))

# Recursively include all .c files in src/
SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

moshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm moshell

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

