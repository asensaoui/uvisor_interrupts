#include "mbed.h"
#include "rtos.h"
#include "uvisor-lib/uvisor-lib.h"


typedef struct {
  InterruptIn *sw;
  DigitalOut *led;
  RawSerial *pc;
} my_box_context;

static const UvisorBoxAclItem acl[] = {
};

static void my_box_main(const void *);

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(4096);
UVISOR_BOX_MAIN(my_box_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_CONFIG(my_box, acl, UVISOR_BOX_STACK_SIZE, my_box_context);

volatile int var = 0; // not secure just for test purpose
 
void isr() {
    var = 1;
    
}
 
void my_thread(void) {
    while (true) {
        if(var){
          *uvisor_ctx->led = !*uvisor_ctx->led;
          uvisor_ctx->pc->printf("LED = %d\n", uvisor_ctx->led->read());
          var = 0;
        }
        Thread::wait(1000);
    }
}
 
static void my_box_main (const void *) {
    

    /* Init Serial */
    RawSerial *pc;    
    if(!(pc = new RawSerial(USBTX, USBRX)))
      return;
    uvisor_ctx->pc = pc;

    /* Init led */
    if(!(uvisor_ctx->led = new DigitalOut(LED1)))
      return;
    *uvisor_ctx->led = 0;

    /* Init interrupt */
    if(!(uvisor_ctx->sw = new InterruptIn(SW1)))
      return;
    uvisor_ctx->sw->mode(PullUp);
    uvisor_ctx->sw->fall(isr);

    osStatus status;

      /* Start ScThread Thread */
    Thread * thread = new Thread();
    status = thread->start(my_thread);
    if (status != osOK) {
      uvisor_ctx->pc->printf("Could not start my_thread .\r\n");
      uvisor_error(USER_NOT_ALLOWED);
    }

}