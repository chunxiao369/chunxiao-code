
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

PKG_LIB = `pkg-config --libs libfixbuf`
PKG_INC = `pkg-config --cflags libfixbuf`

CC = gcc
CFLAGS = $(PKG_INC)
LDFLAGS = $(PKG_LIB) 
TARGET = fix_test
OBJS = fix_test.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

ALL:$(TARGET)

$(TARGET):$(OBJS)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) *~ *.d *.o $(TARGET)
