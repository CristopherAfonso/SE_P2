// Prueba de la librería ad
/// @author Alberto Hamilton
/// @date 2023-11-28


#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <types.h>
#include <ad.h>

void atiendeAD() {
  serial_print("Se ha realizado una conversión\r\nResultado = ");
  uint16_t result[8];
  ad_retrieve_all(result,8);
  serial_printdecword(result[0]);
  serial_print(" ");
  serial_printdecword(result[1]);
  serial_print(" ");
  serial_printdecword(result[2]);
  serial_print(" ");
  serial_printdecword(result[3]);
  serial_print(" ");
  serial_printdecword(result[4]);
  serial_print(" ");
  serial_printdecword(result[5]);
  serial_print(" ");
  serial_printdecword(result[6]);
  serial_print(" ");
  serial_printdecword(result[7]);
  serial_print("\n\r");
}

int main () {
  char c;

  /* Deshabilitamos interrupciones */
  lock ();

  /*Encendemos led*/
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;

  serial_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");
  // Quitamos posible pulsación pendiente
  if (serial_receive_pending()) serial_recv();

  while(1) {
    /* Elección del puerto */
    serial_print("\r\nPuerto conversor a utilizar (0 - 1)?:");
    while((c = serial_recv()) != '0' && c != '1');
    serial_send(c); /* a modo de confirmación*/
    ad_set_conversor(c -'0');
    ad_init();

    /* Elección del pin dentro del puerto */
    serial_print("\r\nPin del puerto a utilizar (0 - 7)?:");
    while((c = serial_recv()) < '0' || c > '7');
    serial_send(c); /* a modo de confirmación*/
    ad_set_init_pin(c - '0');

    /*Pasamos a configurar AD correspondiente*/
    serial_print("\r\nUsar Fifo (0, 1)?:");
    while((c = serial_recv()) < '0' || c > '1');
    serial_send(c); /* a modo de confirmación*/
    ad_set_fifo(c - '0');

    serial_print("\r\nJustificación 0=izquierda, 1=derecha?:");
    while((c = serial_recv()) < '0' || c > '1');
    serial_send(c); /* a modo de confirmación*/
    ad_justify_right(c - '0');

    /* resolución de 10 bits y 16 ciclos de muestreo */
    uint8_t ciclos;
    do {
      serial_print("\r\nCiclos de muestreo (2, 4, 8, 16)?:");
      ciclos = serial_getdecbyte();
    } while(ciclos!=2 && ciclos!=4 && ciclos!=8 && ciclos!=16);
    ad_set_sampling_time(ciclos);

    serial_print("\r\nBits de conversion 0=8 bits, 1=10 bits?:");
    while((c = serial_recv()) < '0' || c > '1');
    serial_send(c); /* a modo de confirmación*/
    ad_set_conversion_size_10(c - '0');


    ad_set_continuous_conversions(0);

    uint8_t numC;
    do {
      serial_print("\r\nConversiones sucesivas (1, 4, 8)?:");
      numC = serial_getdecbyte();
    } while(numC!=1 && numC!=4 && numC!=8);
    ad_set_sucesive_conversions(numC);

    ad_install_callback(atiendeAD);

    unlock();

    while(1) {
      ad_print_status();
      serial_print("\r\nPulsa una tecla para una nueva conversión. C cambiar\r\n");
      if((serial_recv() | 0x20) == 'c') break;
      ad_start();
    }
  }
}
