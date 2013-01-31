OBJDIR=$(PREFIX)/.objs
INCLUDES=
CFLAGS=-g -Wall $(INCLUDES)
LIBS=-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/include/ImageMagick -lMagickWand-Q16 -lMagickCore-Q16 -lm -lMagickCore-Q16 -lreadline
CC=g++

OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;

include .depend
