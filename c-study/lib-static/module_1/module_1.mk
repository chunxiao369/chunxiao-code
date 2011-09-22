sp              :=  $(sp).x
dirstack_$(sp)  :=  $(d)
d               :=  $(dir)

LIBRARY = $(OBJ_DIR)/module_1.a
OBJS_$(d) = $(OBJ_DIR)/module_1.o

$(OBJS_$(d)) : CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall

DEPS_$(d)   :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)

-include $(DEPS_$(d))

$(LIBRARY):$(OBJS_$(d))
	@echo "*-------*create library, library is:   " $(LIBRARY);
	@echo "*-------*                libs_list is: " $(LIBS_LIST);
	$(MAKELIB)

$(OBJ_DIR)/%.o:$(d)/%.c
	$(COMPILE)


d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))

