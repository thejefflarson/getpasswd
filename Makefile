U := $(shell uname -s)
ifeq ($(U),Darwin)
	CC = xcrun clang
	CFLAGS = -fsanitize=address -framework Foundation -framework Security
endif
ifeq ($(U),Linux)
	CC = clang
	CFLAGS = -fsanitize=address $(pkg-config gnome-keyring --libs --cflags)
endif

getpasswd: getpasswd.c

clean:
	rm getpasswd

install:
	cp getpasswd /usr/local/bin/
