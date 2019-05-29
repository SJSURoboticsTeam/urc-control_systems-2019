# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/esp-idf/components/mbedtls/port/include $(PROJECT_PATH)/esp-idf/components/mbedtls/mbedtls/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/mbedtls -lmbedtls
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += $(PROJECT_PATH)/esp-idf/components/mbedtls/mbedtls
COMPONENT_LIBRARIES += mbedtls
component-mbedtls-build: 
