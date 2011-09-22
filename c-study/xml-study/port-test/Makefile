
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

CC = gcc
CFLAGS = -Wall -O2 `xml2-config --cflags`
LDFLAGS = `xml2-config --libs`
TARGET = create_xml
OBJS = create_xml.o
TARGET_ = parser_xml
OBJS_ = parser_xml.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $< $(LDFLAGS) -o $@

#ALL:$(TARGET) $(TARGET_)
ALL: $(TARGET_)

$(TARGET):$(OBJS)
	$(LINK)

$(TARGET_):$(OBJS_)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(OBJS_) *~ *.d *.o $(TARGET_) $(TARGET)
