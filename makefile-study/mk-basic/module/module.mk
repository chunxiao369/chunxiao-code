sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

LIBRARY = $(OBJ_DIR)/module.a
OBJS_$(d) = $(OBJ_DIR)/module.o

$(OBJS_$(d)) : CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall -Dssssssssss
#CFLAGS_LOCAL := -I$(d) -O2 -g -W -Wall 
#CFLAGS_LOCAL := -Dbbb 

DEPS_$(d) :=  $(OBJS_$(d):.o=.d)
LIBS_LIST := $(LIBS_LIST) $(LIBRARY)

#-include $(DEPS_$(d))

$(LIBRARY): $(OBJS_$(d))
	@echo "|----create library"
	@echo  $(CFLAGS_LOCAL)
	$(MAKELIB)
	@echo "|----" $(d) "|----" $(LIBRARY) "|----" $(OBJS_$(d)) "|----" $(dir)

$(OBJ_DIR)/%.o: $(d)/%.c
	@echo "|----create obj_dir"
	$(COMPILE)
	@echo "|----" $(d) "|----" $(LIBRARY) "|----" $(OBJS_$(d)) "|----" $(dir)
 

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
