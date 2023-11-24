/* ****************************************
* General wave output (non-square) through OC2.
* Period is specified in two variables.

   Copyright (C) Alberto F. Hamilton Castro
   Dpto. de Ingeniería Informática de Sistemas
   Universidad de La Laguna

  License: GPLv3

  *************************************** */

#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

#define  TCM_FACTOR (3)  // The power of 2 to apply to the factor
#define  TCM_FREQ (M6812_CPU_E_CLOCK / (1 << TCM_FACTOR))
// Go from microseconds to ticks
#define  USG_2_TICKS(us)  ((us) * (TCM_FREQ / 1000000L))
// Go from milliseconds to ticks
#define  MSG_2_TICKS(ms)  ((ms) * (TCM_FREQ / 1000L))

// Global variables, shared by main program and interrupt handling routine

// We initialize the period according to the microseconds of duration that we want
// is done at COMPILE time
uint16_t PeriodHigh = USG_2_TICKS(5000);  //timer ticks high part
uint16_t PeriodLow = USG_2_TICKS(3000);  //timer ticks lower part

uint16_t irq_count  = 0; // Increases with each interruption


int main () {

  // We disable interruptions
  lock ();

  // We turn the LED on
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;


  serial_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");

  // Timer Initialization
  _io_ports[M6812_TMSK2] = TCM_FACTOR;

  // OC2 Inverts the pin on each shot
  _io_ports[M6812_TCTL2] &= ~M6812B_OM2;
  _io_ports[M6812_TCTL2] |= M6812B_OL2;

  // we prepare first channel shot
  _IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TCNT) + PeriodHigh;


  // we configure channel 2 as output comparator
  _io_ports[M6812_TIOS] |= M6812B_IOS2;


  _io_ports[M6812_TFLG1] = M6812B_IOS2; // We lower the OC2 flag
  _io_ports[M6812_TMSK1] |= M6812B_IOS2; // we enable OC2 interrupts
  _io_ports[M6812_TSCR] = M6812B_TEN; // We enable timer

  unlock(); // we enable interruptions

  serial_print("\r\n Initialization completed");

  while(1) {
    char c;
    c = serial_recv();
    //We echo received character
    serial_send(c);

    serial_print("\r\n\n irqs count: ");
    serial_printdecword(irq_count);

    if(c == '+') PeriodHigh += 10;
    if(c == '-') PeriodHigh -= 10;

    serial_print("\tPeriod usg : ");
    serial_printdecword(PeriodHigh / USG_2_TICKS(1));
    serial_send(' ');
    serial_printdecword(PeriodLow / USG_2_TICKS(1));

    // We invert the led
    _io_ports[M6812_PORTG] ^= M6812B_PG7;

  }
}


 // OC2 interruption handler
void __attribute__((interrupt)) vi_ioc2 (void) {
  _io_ports[M6812_TFLG1] = M6812B_IOS2;  // We lower the OC2 flag

  // we prepare next shot
  if(_io_ports[M6812_PORTT] & M6812B_PT2)
    _IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TC2) + PeriodHigh;
  else
    _IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TC2) + PeriodLow;

  irq_count++;
}

