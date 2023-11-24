/* ****************************************
  Determines the percentage (with respect to the total period)
  that a signal is high and sends it over the serial.
  Signal is received by IC1


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

// Global variables to store the three necessary moments of time
uint16_t Falling0, Falling1, Rising;

// Flag so that the RTI indicates that it has the three times
// It must be volatile so that the compiler does not optimize the checks
volatile uint8_t Calculate;

uint16_t irq_count;

int main () {
  uint16_t PorAnt = 0;  // Previous percentage

  // We disable interruptions
  lock ();

  // We turn the LED on
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;


  serial_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");

  // We initialize the global variables
  Falling0 = Falling1 = Rising = 0;
  Calculate = 0;

  irq_count = 0;

  // We initialize peripherals
  // Timer Initialization
  _io_ports[M6812_TMSK2] = TCM_FACTOR;

  // we configure channel 1 as input capture
  _io_ports[M6812_TIOS] &= ~M6812B_IOS1;

  // Capture on any edge (rising or falling)
  _io_ports[M6812_TCTL4] = M6812B_EDG1B | M6812B_EDG1A;

  _io_ports[M6812_TFLG1] = M6812B_IOS1; // We lower the IC1 flag
  _io_ports[M6812_TMSK1] |= M6812B_IOS1; // we enable IC1 interrupts
  _io_ports[M6812_TSCR] = M6812B_TEN; // We enable timer

  unlock(); // we enable interruptions

  serial_print("\r\n Initialization completed");

  while(1) {
    uint16_t Percent, Period, Highs;

    // We wait for Calculate to go to 1
    while(! Calculate) {
      //__asm__ __volatile__ ("wai");
    };

    // We do calculations
    Period = Falling1 - Falling0;
    Highs = (Rising - Falling0);
    Percent = (long)Highs * 100/(long)Period;
    // We only print if there have been changes
    if(Percent != PorAnt) {
      PorAnt = Percent;
      serial_print("\rPorcentaje = ");
      serial_printdecword(Percent);
      serial_print("\t");
      serial_printdecword(Period);
      serial_print("\t");
      serial_printdecword(Highs);
      serial_print("\t");
      serial_printdecword(irq_count);
    }
    Falling0 = 0;
    Calculate = 0; // to indicate that we have finished
  }
}


// IC1 interruption handler
void __attribute__((interrupt)) vi_ioc1 (void) {
  _io_ports[M6812_TFLG1] = M6812B_IOS1; // We lower the IC1 flag
  irq_count++;

  if(Calculate) return; // The main program is calculating, we do not touch anything

  // We see if it was falling or rising edge
  if(_io_ports[M6812_PORTT] & M6812B_PT1) {
    // Rising, we only store
    Rising = _IO_PORTS_W(M6812_TC1);
    _io_ports[M6812_PORTG] |= M6812B_PG7; // We turn LED on
  } else if (! Falling0)
    // Falling0 is == 0, it means we don't have it yet
    Falling0 = _IO_PORTS_W(M6812_TC1);
  else {
    Falling1 = _IO_PORTS_W(M6812_TC1);
    Calculate = 1;
    _io_ports[M6812_PORTG] &= ~M6812B_PG7; // We turn LED off
  }

}

