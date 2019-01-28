# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/lwip/include/apps $(PROJECT_PATH)/esp-idf/components/lwip/lwip/src/include $(PROJECT_PATH)/esp-idf/components/lwip/port/esp32/include $(PROJECT_PATH)/esp-idf/components/lwip/port/esp32/include/arch $(PROJECT_PATH)/esp-idf/components/lwip/include_compat
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/lwip -llwip
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += $(PROJECT_PATH)/esp-idf/components/lwip/lwip
COMPONENT_LIBRARIES += lwip
component-lwip-build: 
