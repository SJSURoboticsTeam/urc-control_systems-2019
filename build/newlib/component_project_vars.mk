# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/newlib/platform_include $(PROJECT_PATH)/esp-idf/components/newlib/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/newlib $(PROJECT_PATH)/esp-idf/components/newlib/lib/libc.a $(PROJECT_PATH)/esp-idf/components/newlib/lib/libm.a -lnewlib
COMPONENT_LINKER_DEPS += $(PROJECT_PATH)/esp-idf/components/newlib/lib/libc.a $(PROJECT_PATH)/esp-idf/components/newlib/lib/libm.a
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += newlib
component-newlib-build: 
