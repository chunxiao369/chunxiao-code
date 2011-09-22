sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

LIBRARY = $(OBJ_DIR)/tb.a
OBJS_$(d) = \
	$(OBJ_DIR)/tb.o \

CFLAGS_LOCAL := -Daaaa 
$(OBJS_$(d)) : CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall -Dwwwwwwwwww

CFLAGS_LOCAL := -Dddddddddddddddddddd
DEPS_$(d) :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)

#-include $(DEPS_$(d))

$(LIBRARY): $(OBJS_$(d))
	@echo "|----create library"
	@echo  $(CFLAGS_LOCAL)
	@echo  embedway xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx$^
	$(MAKELIB)
	@echo "|----" $(d) "|----" $(LIBRARY) "|----" $(OBJS_$(d)) "|----" $(dir)

$(OBJ_DIR)/%.o: $(d)/%.c
	@echo "|----create obj_dir"
	$(COMPILE)
	@echo "|----" $(d) "|----" $(LIBRARY) "|----" $(OBJS_$(d)) "|----" $(dir)

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
