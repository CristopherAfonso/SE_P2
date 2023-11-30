#include <gpio.h>
#include <sieteseg.h>
#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <types.h>

int main() {
  /// Deshabilitamos interrupciones
  lock();

  serial_init();
  sieteSeg_init();
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  serial_print("\r\n" __FILE__ " ==========\r\n");

  sieiteSeg_valor(1234);

  unlock();  ///< Habilitamos interrupciones

  serial_print("\r\nTerminada inicialización");

  while (1) {
    // Pedir un valor por teclado
    serial_print("\r\nIntroduzca un valor: ");
    uint16_t val = serial_getdecword();
    sieteSeg_valor(val);
  }

  return 0;
}
