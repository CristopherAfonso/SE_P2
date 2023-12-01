#include <gpio.h>
#include <sieteseg.h>
#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <timer.h>
#include <types.h>
#include <teclado.h>
#include <pwm.h>

int main() {
	/// Deshabilitamos interrupciones
  lock(); 
 
  serial_init();
  sieteSeg_init();
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  teclado_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");
  
  pwm_set_clock(0, 0);
  pwm_set_polarity(0, 1);
  pwm_set_alignment(1);
  pwm_set_channel_period(0, 200);
  pwm_modify_channel_duty_steps(0, 0);
  pwm_channel_enable(0);
 
  unlock();  ///< Habilitamos interrupciones
  
  uint16_t valSeg = 0;
  uint16_t valMot = 0;
  sieteSeg_valor(valSeg);
  
  while (1) {
    // Pedir un valor por teclado
    char val = teclado_getch_timeout(1000);
    
    if (val >= '0' && val <= '9') {
		uint16_t newValSeg = valSeg;
		newValSeg = newValSeg * 10;
		newValSeg += val - '0';
		
		if (newValSeg <= 100) {
			valSeg = newValSeg;
			sieteSeg_valor(valSeg);
		}
	} else if (val == '#') {
		valSeg /= 10;
		sieteSeg_valor(valSeg);
	} else if (val == '*') {
		valMot = valSeg;
		valSeg = 0;
		pwm_modify_duty_percentage(0, valMot);
	}
	
	serial_print("\r\nValor puesto: ");
	serial_printdecword(valSeg);
	serial_print(" \tValor motor: ");
	serial_printdecword(valMot);
  }

  return 0;
  
}
