#include <stdio.h>
#include <stdint.h>

#define F_CPU 12000000
#define F_CPU_PRESCALED (F_CPU / 1024.0)
#define RASPNET_DELAY_MS (40 / 2.0)
#define RASPNET_DELAY (RASPNET_DELAY_MS / 1000.0)
#define RASPNET_COUNTER (uint16_t)(RASPNET_DELAY * F_CPU_PRESCALED - 1)
#define JOB_COUNTER (uint16_t)(5 * F_CPU_PRESCALED - 1)

// TEST DELAY MACROS
int main()
{
    printf("%f\n", RASPNET_DELAY);
    printf("%d\n", RASPNET_COUNTER);
    printf("%d\n", JOB_COUNTER);
    return 0;
}
