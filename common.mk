OBJDIR=$(PREFIX)/.objs
INCLUDES=
CFLAGS=-g -Wall $(INCLUDES)
LIBS=-I/usr/include/ImageMagick -lMagickWand -lMagickCore -lreadline
CC=g++

OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;

include .depend
