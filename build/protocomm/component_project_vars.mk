# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/protocomm/include/common $(PROJECT_PATH)/esp-idf/components/protocomm/include/security $(PROJECT_PATH)/esp-idf/components/protocomm/include/transports
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/protocomm -lprotocomm
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += protocomm
component-protocomm-build: 
