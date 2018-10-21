#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Source.h"
// #include "Arm_I2C.h"
#include "IMU.h"
#include "Magnet_Encoder.h"
#include "RTOS_Tasks.h"
#include "common_init.h"

extern "C" void app_main()
{
    printf("Arm Testing Wifi \n");
    Arm_Init();
    // I2C_master_init(I2C_EXAMPLE_MASTER_SDA_IO, I2C_EXAMPLE_MASTER_SCL_IO);
    xTaskCreate(vReadDataTask     , "vReadDataTask"       , 2048, NULL, 2, NULL);
    xTaskCreate(vSendDataTask     , "vSendDataTask"       , 2048, NULL, 2, NULL);
    xTaskCreate(vI2CTask          , "vI2CTask"            , 2048, NULL, 2, NULL);
    // xTaskCreate(vIMUTask          , "vIMUTask"            , 2048, NULL, 2, NULL);
    // xTaskCreate(vBIMUTask          , "vBIMUTask"            , 2048, NULL, 1, NULL);
    xTaskCreate(vElbowTask        , "vElbowTask"          , 2048, NULL, 1, NULL);
    xTaskCreate(vRotundaTask      , "vRotundaTask"        , 2048, NULL, 1, NULL);
    xTaskCreate(vShoulderTask     , "vShoudlerTask"       , 2048, NULL, 1, NULL);
    xTaskCreate(vWristPitchTask   , "vWristPitchTask"     , 2048, NULL, 1, NULL);
    xTaskCreate(vWristRotationTask, "vWristRotationTask"  , 2048, NULL, 1, NULL);
    xTaskCreate(vClawTask         , "vClawTask"           , 2048, NULL, 1, NULL);
    xTaskCreate(vCamTask         , "vCamTask"           , 2048, NULL, 1, NULL);
    // xTaskCreate(vMotorTestTask	  , "vMotorTestTask"    , 2048, NULL, 1, NULL);
    // xTaskCreate(vMagRawTest		  , "vMagRawTest"		, 2048, NULL, 1, NULL);
    // xTaskCreate(vImuIdRead		  , "vImuIdRead"		, 2048, NULL, 1, NULL);
    // xTaskCreate(vQuadEncTestTask   , "vQuadEncTestTask"  , 2048, NULL, 1, NULL);
    // xTaskCreate(vAdcTask           , "vAdcTask"          , 2048, NULL, 1, NULL);
}
