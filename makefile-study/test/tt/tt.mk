sp := $(sp).x
dirstack_$(sp) := $(d)
d := $(dir)

#dir := $(TOP)/tt/ta
dir := tt/ta
include $(dir)/ta.mk

#dir := $(TOP)/tt/tb
dir := tt/tb
include $(dir)/tb.mk


d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
