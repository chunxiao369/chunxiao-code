sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

LIBRARY = $(OBJ_DIR)/module.a
OBJS_$(d) = $(OBJ_DIR)/module.o

$(OBJS_$(d)) : CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall
#CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall

DEPS_$(d) :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)

$(LIBRARY): $(OBJS_$(d))
	$(MAKELIB)

$(OBJ_DIR)/%.o: $(d)/%.c
	$(COMPILE)

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
