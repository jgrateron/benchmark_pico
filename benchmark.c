
#include "math.h"
#include "string.h"
#include <stdio.h>
#include <inttypes.h>
#include "pico/stdlib.h"

#define BENCHMARK_MULTIPLIER 10

// M1 - Fixed-point maths test.
//
void benchmark_m1(void)
{
    uint16_t i;
    uint16_t result1 = 0;
    uint32_t result2 = 0L;
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0; i < UINT16_MAX >> 1; i++)
        {
            result1 += (i % 32767) * (i % 32767);
        }

        for (i = 0; i < UINT32_MAX >> 17; i++)
        {
            result2 += (i % 2147483647L) * (i % 2147483647L);
        }
    }
}

// M2 - Floating-point maths test.
//
void benchmark_m2(void)
{
    double i;
    double result = 0.F;
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0.F; i < 1000.F; i++)
        {
            result += sin(i) * sinh(i);
        }   
    }
}

// M3 - Logical operations test.
//
void benchmark_m3(void)
{
    uint32_t i;
    uint32_t result = 0;
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0; i < UINT32_MAX >> 13; i++)
        {
            result |= (i & 1) ^ (i & 2);
        }
    }
}

// M4 - Looping and conditional jump test.
//
void benchmark_m4(void)
{
    uint32_t i;
    uint8_t result = 0;
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0; i < UINT32_MAX >> 12; i++)
        {
            if ((i % 2) == 0) 
                result++;
            else
                result--;
        }    
    }
}

// M5 - Recursion and stack test.
//
void benchmark_m5_recurse(uint8_t depth)
{
    if (depth == 0)
        return;
    else
        benchmark_m5_recurse(--depth);
}

void benchmark_m5(void)
{
    uint16_t i;
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0; i < UINT16_MAX; i++)
        {
            benchmark_m5_recurse(32);
        }
    }
}

void benchmark_m6(void)
{
    uint16_t i;
    char s1[11] = "0123456789";
    char s2[11];
    uint16_t m;

    for (m = 0; m < BENCHMARK_MULTIPLIER; m++)
    {
        for (i = 0; i < UINT16_MAX; i++)
        {
            strncpy(s2, s1, 10);
            s2[5] = '\0';
            strncat(s2, s1 + 5, 5);
        }
    }
}

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    stdio_init_all();
    sleep_ms(2500);
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    absolute_time_t ini_time;
    int64_t diff;
    while (true) {
        printf("Iniciando calculos\n");
        gpio_put(LED_PIN, 1);
        ini_time = get_absolute_time();
        benchmark_m1();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m1 %" PRId64 "\n",diff);

        gpio_put(LED_PIN, 0);
        ini_time = get_absolute_time();
        gpio_put(LED_PIN, 1);
        benchmark_m2();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m2 %" PRId64 "\n",diff);
        gpio_put(LED_PIN, 0);

        gpio_put(LED_PIN, 1);
        ini_time = get_absolute_time();
        benchmark_m3();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m3 %" PRId64 "\n",diff);
        gpio_put(LED_PIN, 0);

        gpio_put(LED_PIN, 1);
        ini_time = get_absolute_time();
        benchmark_m4();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m4 %" PRId64 "\n",diff);
        gpio_put(LED_PIN, 0);

        gpio_put(LED_PIN, 1);
        ini_time = get_absolute_time();
        benchmark_m5();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m5 %" PRId64 "\n",diff);
        gpio_put(LED_PIN, 0);
        
        gpio_put(LED_PIN, 1);
        ini_time = get_absolute_time();
        benchmark_m6();
        diff = absolute_time_diff_us(ini_time, get_absolute_time());
        printf("benchmark_m6 %" PRId64 "\n",diff);
        gpio_put(LED_PIN, 0);
        sleep_ms(5000);
    }
#endif
}
