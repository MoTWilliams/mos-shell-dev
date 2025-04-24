CC=gcc
CFLAGS=-Iheaders -Iutility
DEPS = $(wildcard headers/*.h utility/*.h)
SRC = $(wildcard src/*.c src/builtins/*.c)
OBJ = $(SRC:.c=.o)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

moshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o src/builtins/*.o
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