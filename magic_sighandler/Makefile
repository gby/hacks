
.PHONY: all clean 

LIB_VERSION = 1.0.0
LIB_API_VERSION = 1

LIB_LDFLAGS = -shared
LIB_CFLAGS = -g

all: libmagic_signahler.so.$(LIB_VERSION)

libmagic_signahler.so.$(LIB_VERSION): magic_sighandler.c Makefile
	$(CC) -fPIC magic_sighandler.c $(LIB_CFLAGS) $(LIB_LDFLAGS) \
	-Wl,-soname,libmagic_sighandler.so.$(LIB_API_VERSION) -o libmagic_sighandler.so.$(LIB_VERSION)

clean:
	$(RM) -f libmagic_signahler.so.$(LIB_VERSION)
