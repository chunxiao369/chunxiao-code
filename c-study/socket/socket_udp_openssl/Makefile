
# 
# compile tool
# compile param
# link    param
# exe file
# *.o file
# compile command 
# link command
#

#CC = ppc_82xx-gcc
CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lssl -lcrypto
TARGET = server
AA = crypto_utils.o
OBJS = socket-server.o
TARGET_ = client
OBJS_ = socket-client.o
COMPILE  = $(CC) $(CFLAGS) -MD -c -o $@ $<
LINK = $(CC) $^ $(LDFLAGS) -o $@

ALL:$(TARGET) $(TARGET_)

$(TARGET):$(OBJS)
	#openssl genrsa -out server.key 2048
	#openssl req -new -x509 -key server.key -out server.crt -days 365 -subj "/C=US/ST=Test/L=Local/O=DevOrg/OU=Dev/CN=localhost"
	$(LINK)

$(TARGET_):$(OBJS_)
	$(LINK)

%.o:%.c
	$(COMPILE)

-include $(OBJS:.o=.d)

clean:
	rm -f $(OBJS) $(OBJS_) *~ *.d *.o $(TARGET_) $(TARGET)
