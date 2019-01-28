# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/asio/asio/asio/include $(PROJECT_PATH)/esp-idf/components/asio/port/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/asio -lasio
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += $(PROJECT_PATH)/esp-idf/components/asio/asio
COMPONENT_LIBRARIES += asio
component-asio-build: 
