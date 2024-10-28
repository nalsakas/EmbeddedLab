#ifndef __PINS_H_
#define __PINS_H_

#ifndef F_CPU
#define F_CPU 12000000
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#define RASPNET_DELAY_MS (50 / 2.0)
#define F_CPU_PRESCALED (F_CPU / 1024.0)
#define RASPNET_DELAY (RASPNET_DELAY_MS / 1000.0)
#define RASPNET_COUNTER (uint16_t)(RASPNET_DELAY * F_CPU_PRESCALED - 1)

void pins_init();
void pins_clock();
void pins_enable_relay();
void pins_disable_relay();
void pins_led_toggle();
#endif
