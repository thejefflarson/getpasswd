U := $(shell uname -s)
ifeq ($(U),Darwin)
	CC = xcrun clang
	CFLAGS = -fsanitize=address -framework Foundation -framework Security
endif
ifeq ($(U),Linux)
	CC = clang
	LDLIBS = `pkg-config libsecret-1 --libs --cflags`
endif

getpasswd: getpasswd.c

clean:
	rm getpasswd

install: getpasswd
	cp $< /usr/local/bin/
