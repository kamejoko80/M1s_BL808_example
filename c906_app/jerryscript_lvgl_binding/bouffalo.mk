#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_ADD_INCLUDEDIRS += jerry-lvgl-binding

COMPONENT_SRCDIRS := .
COMPONENT_SRCDIRS += jerry-lvgl-binding