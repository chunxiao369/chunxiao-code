sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

LIBRARY = $(OBJ_DIR)/tb.a
OBJS_$(d) = \
	$(OBJ_DIR)/tb.o \

DEPS_$(d) :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)

$(LIBRARY): $(OBJS_$(d))
	$(MAKELIB)

$(OBJ_DIR)/%.o: $(d)/%.c
	$(COMPILE)

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
