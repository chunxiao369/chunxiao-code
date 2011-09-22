# 现在生成了一个main.o文件
# ⑶生成可执行文件，注意要把静态库文件的路径加到-L参数里面，
# 把库文件名(去掉打头的lib和结尾的.a)加到-l参数后面。如下面所示
#main:main.o
#	gcc -o main main.o ./lib/libtest.a
#main.o:main.c
#	gcc  -I./lib -c main.c
CC = gcc
OBJ_DIR = objs
OBJECTS = ${OBJ_DIR}/main.o
TARGET = xuexe
CFLAGS_LOCAL = 
#STATIC_LIB_DIR = ./module_1
#COMPILE = $(CC) -I${STATIC_LIB_DIR} $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -c -o $@ $<
COMPILE = $(CC) -I${dir} $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MD -c -o $@ $<
MAKELIB = ${AR} -cr $@ $^ 

#COMPILE_TAGET = $(CC) -L${STATIC_LIB_DIR} ${OBJECTS}  -ltest -o $@ 
COMPILE_TAGET = $(CC)  ${OBJECTS}  ${LIBS_LIST} -o $@ 

all:${TARGET}

${OBJ_DIR}:
	mkdir ${OBJ_DIR}

${OBJ_DIR}/%.o:%.c
	${COMPILE} 

dir := module_1
include ${dir}/module_1.mk

${TARGET}:${OBJ_DIR} ${OBJECTS} ${LIBS_LIST}
	${COMPILE_TAGET}

clean: 
	rm -rf $(OBJ_DIR) $(TARGET)

