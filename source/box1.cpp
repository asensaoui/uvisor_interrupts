#include "mbed.h"
#include "rtos.h"
#include "uvisor-lib/uvisor-lib.h"







/* Private static memory for the secure box */
typedef struct {
  int secret;
} Contextbox2;


static const UvisorBoxAclItem Thread_2_acls[] = {
  {(void*) 0x40008000,      0x2000,            UVISOR_TACLDEF_PERIPH}, 
};

static void Thread_2(const void *);
/* Secure box configuration */
UVISOR_BOX_NAMESPACE(NULL);                   /* We won't specify a box namespace for this example. */
UVISOR_BOX_HEAPSIZE(4096);                    /* Heap size for the secure box */
UVISOR_BOX_MAIN(Thread_2,    /* Main thread for the secure box */
                osPriorityNormal,             /* Priority of the secure box's main thread */
                1024);                        /* Stack size for the secure box's main thread */
UVISOR_BOX_CONFIG(box2,              /* Name of the secure box */
                  Thread_2_acls,       /* ACLs list for the secure box */
                  1024,                       /* Stack size for the secure box */
                  Contextbox2);  /* Private static memory for the secure box. */



Serial pc(SERIAL_TX, SERIAL_RX);
DigitalOut led1(LED1);

static void but_func(void)
{
   led1 = !led1;
}


/* Main thread for the secure box */
static void Thread_2(const void *)
{
  InterruptIn button(SW1);
  button.mode(PullUp);
  button.fall(&but_func);

  while (true) {
    pc.printf("led1 : %d\r\n", led1.read());
    Thread::wait(1000);
  }

}