#ifndef __PINS_H_
#define __PINS_H_

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#define F_CPU_PRESCALED (F_CPU / 1024.0)
#define RASPNET_DELAY_MS (10 / 2.0)
#define RASPNET_DELAY (RASPNET_DELAY_MS / 1000.0)
#define RASPNET_COUNTER (uint8_t)(RASPNET_DELAY * F_CPU_PRESCALED - 1)
#define JOB_COUNTER (uint16_t)(5 * F_CPU_PRESCALED - 1)

void pins_init();
void pins_clock();
void pins_enable_relay();
void pins_disable_relay();
void pins_led_toggle();
void pins_timer1_reset();
#endif