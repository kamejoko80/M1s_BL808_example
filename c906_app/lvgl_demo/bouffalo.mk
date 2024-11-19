#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_ADD_INCLUDEDIRS += custom
COMPONENT_ADD_INCLUDEDIRS += generated
COMPONENT_ADD_INCLUDEDIRS += generated/guider_customer_fonts
COMPONENT_ADD_INCLUDEDIRS += generated/guider_fonts
COMPONENT_ADD_INCLUDEDIRS += generated/images

COMPONENT_SRCDIRS := .
COMPONENT_SRCDIRS += custom
COMPONENT_SRCDIRS += generated
COMPONENT_SRCDIRS += generated/guider_customer_fonts
COMPONENT_SRCDIRS += generated/guider_fonts
COMPONENT_SRCDIRS += generated/images