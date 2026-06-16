#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include <stdio.h>

#define LED_GPIO GPIO_NUM_23
#define BUTTON_GPIO GPIO_NUM_0       // BOOT0 / IO0 button
#define POT_CHANNEL ADC_CHANNEL_6    // GPIO34 = ADC1 Channel 6

// Half-cycle delays for:
// 1/4Hz, 1/2Hz, 1Hz, 2Hz, 4Hz, 8Hz, 16Hz
static const int delays_ms[] = {
    2000,  // 1/4 Hz
    1000,  // 1/2 Hz
    500,   // 1 Hz
    250,   // 2 Hz
    125,   // 4 Hz
    62,    // 8 Hz
    31     // 16 Hz
};

static const float frequencies[] = {
    0.25,
    0.5,
    1.0,
    2.0,
    4.0,
    8.0,
    16.0
};

static const int num_speeds = 7;

void app_main(void)
{
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_GPIO);

    adc_oneshot_unit_handle_t adc_handle;

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };

    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_oneshot_chan_cfg_t adc_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    adc_oneshot_config_channel(adc_handle, POT_CHANNEL, &adc_config);

    int speed_index = 0;
    int last_button_state = 1;
    int led_state = 0;

    while (1)
    {
        int button_state = gpio_get_level(BUTTON_GPIO);

        if (last_button_state == 1 && button_state == 0)
        {
            speed_index = (speed_index + 1) % num_speeds;

            printf("BOOT0 pressed | New blink rate: %.2f Hz\n",
                   frequencies[speed_index]);

            vTaskDelay(pdMS_TO_TICKS(250));
        }

        last_button_state = button_state;

        led_state = !led_state;
        gpio_set_level(LED_GPIO, led_state);

        int pot_value = 0;
        adc_oneshot_read(adc_handle, POT_CHANNEL, &pot_value);

        printf("LED changed to %s | Potentiometer value: %d | Blink rate: %.2f Hz\n",
               led_state ? "ON" : "OFF",
               pot_value,
               frequencies[speed_index]);

        vTaskDelay(pdMS_TO_TICKS(delays_ms[speed_index]));
    }
}