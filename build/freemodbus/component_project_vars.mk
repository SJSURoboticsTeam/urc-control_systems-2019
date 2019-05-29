# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/freemodbus/modbus/include $(PROJECT_PATH)/esp-idf/components/freemodbus/modbus_controller
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/freemodbus -lfreemodbus
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += freemodbus
component-freemodbus-build: 
