sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

dir := $(TOP)/tt/ta
include $(dir)/ta.mk

dir := $(TOP)/tt/tb
include $(dir)/tb.mk


d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
