OBJDIR=$(PREFIX)/.objs
INCLUDES=-I $(PREFIX)/Filter -I $(PREFIX)/Image -I $(PREFIX)/Util -I $(PREFIX)/Descriptors
LIBS=-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/include/ImageMagick-6 -lMagickWand-6.Q16 -lMagickCore-6.Q16 -lm -lMagickCore-6.Q16 -lreadline
CFLAGS=-g -Wall $(INCLUDES) $(LIBS)
CC=g++

OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;

include .depend
