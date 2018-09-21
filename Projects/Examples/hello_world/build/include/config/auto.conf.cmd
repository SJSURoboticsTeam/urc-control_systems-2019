deps_config := \
	/Users/ariskoumis/esp/esp-idf/components/app_trace/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/bt/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/driver/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/esp32/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/esp_http_client/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/ethernet/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/fatfs/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/freertos/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/heap/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/libsodium/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/log/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/lwip/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/openssl/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/pthread/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/spiffs/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/vfs/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/ariskoumis/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/ariskoumis/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/ariskoumis/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/ariskoumis/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
