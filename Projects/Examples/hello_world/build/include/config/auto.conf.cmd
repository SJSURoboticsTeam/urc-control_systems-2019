deps_config := \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/app_trace/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/aws_iot/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/bt/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/driver/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/esp32/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/esp_adc_cal/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/esp_http_client/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/ethernet/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/fatfs/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/freertos/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/heap/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/http_server/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/libsodium/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/log/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/lwip/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/mbedtls/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/mdns/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/openssl/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/pthread/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/spi_flash/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/spiffs/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/tcpip_adapter/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/vfs/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/wear_levelling/Kconfig \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/Kconfig.compiler \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/bootloader/Kconfig.projbuild \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/esptool_py/Kconfig.projbuild \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/components/partition_table/Kconfig.projbuild \
	/mnt/c/Users/Colin/Robotics2019/controlsystems2019/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
