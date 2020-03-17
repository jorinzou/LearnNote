CC=gcc

OBJ=HttpServer
all:$(OBJ)

HERE=$(shell pwd)
HTTP_INC=$(HERE)/src/
incs=-I$(HERE)
incs+=-I$(HTTP_INC)


HTTP_LIB_DIR=$(HERE)/src/
files+=$(foreach dir,$(HTTP_LIB_DIR),$(wildcard $(HTTP_LIB_DIR)/*.c))

OBJS_C := $(patsubst %.c,%.o,$(files))


CFLAGS=-O0 -g 
CFLAGS+=$(incs) 

LDFLAGS=-lpthread 

$(OBJ):$(OBJS_C)
	$(CC)  $(LDFLAGS) -o $@ $^ 
			
$(OBJS_C):%.o:%.c
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	rm -rf $(HERE)/*.o
	rm -rf $(HERE)/$(OBJ)































