sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

NAME = moduleA
LIBRARY = $(OBJ_DIR)/lib$(NAME).a
OBJS_$(d) = $(OBJ_DIR)/moduleA.o

$(OBJS_$(d)) : CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall

DEPS_$(d) :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)
LDFLAGS_GLOBAL := $(LDFLAGS_GLOBAL) -l$(NAME)

#-include $(DEPS_$(d))

$(LIBRARY): $(OBJS_$(d))
	$(MAKELIB)

$(OBJ_DIR)/%.o: $(d)/%.c
	$(COMPILE)

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
