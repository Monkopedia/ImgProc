TARGET=
SRCS=$(wildcard *.cpp) $(wildcard *.c)
PREFIX=..

all: depend OBJS

include $(PREFIX)/common.mk

OBJS : $(OBJS)

$(OBJDIR)/%.o : %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJDIR)/%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<
