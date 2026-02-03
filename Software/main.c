/**
 sebulli.com
 */
#include "tusb.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>


#include "pico/stdlib.h"
#include <pico/time.h>

#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
#include "hardware/pwm.h"

#include "global.h"
#include "main.h"



uint32_t slice_num;



int main()
{
    stdio_init_all();

      gpio_init(SWITCH);
    gpio_set_dir(SWITCH, GPIO_IN);

    gpio_init(HOLD);
    gpio_set_dir(HOLD, GPIO_OUT);
    gpio_put(HOLD, 1);
  
   

    // Tell the HV pin that the PWM is in charge of its value.
    gpio_set_function(PIEZO, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the HV PWM pin
    slice_num = pwm_gpio_to_slice_num(PIEZO);

   
    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();


    // 125MHz / 12.5 / 10000 = 1kHz
    config.top = 10000;
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 12.5f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(PIEZO, 5000); // 50% PWM
    sleep_ms(200);
    pwm_set_gpio_level(PIEZO, 0); // 0% PWM

    // 125MHz / 12.5 / 2941 = 3.4kHz
    config.top = 2941;
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 12.5f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(PIEZO, 0); // 50% PWM

    for (int i=0; i< 30; i++)
    {
        sleep_ms(1000);
    }

    for (int ii=0; ii< 20; ii++)
    {
        for (int iii=0; iii< 40; iii++)
        {
            if (iii==0 || iii== 4)
            {
                pwm_set_gpio_level(PIEZO, 1500); // 50% PWM
            }
            else
            {
                pwm_set_gpio_level(PIEZO, 0); // 0% PWM
            }

            sleep_ms(50);
            if (gpio_get(SWITCH))
            {
                iii = 100;
                ii  = 100;
            }
            
        }
    }
    gpio_put(HOLD, 0);

    // Endless loop
    while (true)
    {
    }

    return 0;
}

