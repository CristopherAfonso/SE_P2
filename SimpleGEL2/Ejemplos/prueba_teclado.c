#include <gpio.h>
#include <sieteseg.h>
#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <timer.h>
#include <types.h>
#include <teclado.h>

int main() {
	/// Deshabilitamos interrupciones
  lock(); 
 
  serial_init();
  sieteSeg_init();
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  teclado_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");
 
  unlock();  ///< Habilitamos interrupciones
  
  uint16_t valSeg = 0;
  sieteSeg_valor(valSeg);
  
  while (1) {
    // Pedir un valor por teclado
    serial_print("\r\nValor puesto: ");
    char val = teclado_getch_timeout(1000);
    serial_send(val);
    
    if (val >= '0' && val <= '9') {
		// 6552 5
		valSeg = valSeg - ((valSeg/1000) * 1000); 
		valSeg = valSeg * 10;
		valSeg += val - '0';
		sieteSeg_valor(valSeg);
	}
  }

  return 0;
  
}
