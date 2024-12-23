#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_ADD_INCLUDEDIRS += jerry-lvgl-binding/common
COMPONENT_ADD_INCLUDEDIRS += jerry-lvgl-binding/widgets
COMPONENT_ADD_INCLUDEDIRS += jerry-lvgl-binding/extra/widgets/win
COMPONENT_ADD_INCLUDEDIRS += jerry-lvgl-binding/extra/widgets/analogclock

COMPONENT_SRCDIRS := .
COMPONENT_SRCDIRS += jerry-lvgl-binding/common
COMPONENT_SRCDIRS += jerry-lvgl-binding/widgets
COMPONENT_SRCDIRS += jerry-lvgl-binding/extra/widgets/win
COMPONENT_SRCDIRS += jerry-lvgl-binding/extra/widgets/analogclock
