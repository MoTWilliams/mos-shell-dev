CC=gcc

# Find all header files in src/ and its subdirectories. Only include each once.
INCLUDES = $(shell find src -name '*.h' -exec dirname {} \; | sort -u)

# Turn all includes into -I flags
CFLAGS = -g $(addprefix -I, $(INCLUDES))

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

install: moshell
	@{ \
		cp moshell ~/.local/bin/moshell; \
		make clean; \
		if [ -f "$(HOME)/.local/bin/moshell" ]; then \
			echo "Mo's Shell successfully installed. Use 'moshell' to run."; \
		fi; \
	}

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