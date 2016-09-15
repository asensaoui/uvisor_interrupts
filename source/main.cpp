#include "mbed.h"
#include "rtos.h"
#include "uvisor-lib/uvisor-lib.h"







extern "C" void SVC_Handler(void);
extern "C" void PendSV_Handler(void);
extern "C" void SysTick_Handler(void);
extern "C" uint32_t rt_suspend(void);
UVISOR_SET_PRIV_SYS_HOOKS(SVC_Handler, PendSV_Handler, SysTick_Handler, rt_suspend);

/* Main box Access Control Lists (ACLs). */

static const UvisorBoxAclItem g_main_box_acls[] = {
    {GCR,                                   sizeof(*GCR),     UVISOR_TACLDEF_PERIPH},
    {TMR0,                                   sizeof(*TMR0),     UVISOR_TACLDEF_PERIPH}, 
    {GPIO0,                                 sizeof(*GPIO0),   UVISOR_TACLDEF_PERIPH}, 
    {UART0,                                 sizeof(*UART0),   UVISOR_TACLDEF_PERIPH},
    {(void *)0x42100000,										0x1000,						UVISOR_TACLDEF_PERIPH},
};

/* Enable uVisor, using the ACLs we just created. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_box_acls);



DigitalOut led2(LED2);


int main() {

    while(1){      
        led2 = !led2;
        Thread::wait(1000);
    }

}