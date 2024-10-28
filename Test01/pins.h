#ifndef __PINS_H_
#define __PINS_H_

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#define RASPNET_DELAY_MS 10
#define RASPNET_COUNTER (int)(RASPNET_DELAY_MS * (F_CPU / 1024) / 1000 - 1)
#define JOB_COUNTER (int)(5000 * (F_CPU / 1024 - 1))

void pins_init();
void pins_clock();
void pins_enable_relay();
void pins_disable_relay();
void pins_led_toggle();
void pins_timer1_reset();
#endif