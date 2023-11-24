/* ****************************************
 * Lee una entrada analógica y muestra valor por
 * la serial.
 * Al inicio permite elegir en tiempo de ejecución el puerto
 * y el pin dentro del puerto a utilizar
 * No utiliza interrupciones sino polling

  Copyright (C) Alberto F. Hamilton Castro
  Dpto. de Ingeniería Informática de Sistemas
  Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */

#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <types.h>
#include <ad.h>

void pepe() {
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

/* Elección del puerto */
    serial_print("\r\nPuerto conversor a utilizar (0 - 1)?:");
    while((c = serial_recv()) != '0' && c != '1');
    serial_send(c); /* a modo de confirmación*/
    ad_set_conversor(0);

    /* Elección del pin dentro del puerto */
    serial_print("\r\nPin del puerto a utilizar (0 - 7)?:");
    while((c = serial_recv()) < '0' && c > '7');
    serial_send(c); /* a modo de confirmación*/
    ad_init();

    /*Pasamos a configurar AD correspondiente*/
    ad_set_fifo(0);
    ad_justify_right(1);

    /* resolución de 10 bits y 16 ciclos de muestreo */
    ad_set_sampling_time(16);
    ad_set_conversion_size_10(1);


    ad_set_init_pin(c - '0');
    ad_set_continuous_conversions(0);
    ad_set_sucesive_conversions(8);
    ad_install_callback(pepe);

  while(1) {
    serial_print("\r\nPulsa una tecla para una nueva conversión\r\n");
    serial_recv();
    ad_start();
  }
}
