/**
 * Universidad de La Laguna (ULL)
 * Escuela Superior de Ingenierıa y Tecnologıa (ESIT)
 * Grado en Ingenierıa Informatica
 * Asignatura: Sistemas Empotrados
 * Curso: 4º
 * Itinerario 2: Ingenierıa de Computadores
 * Practica 2: Funciones y Programas para el manejo de la tarjeta DISEN-EXP
 * @author Cristopher Manuel Afonso Mora (alu0101402031@ull.edu.es)
 * @author Francesco La Spina (alu0101435022@ull.edu.es)
 * @date 30/11/2023
 *
 * @file prueba_potenciometro7seg.c
 * @brief Archivo que lee el valor de uno de los potenciómetros y
 * represente en los 7-segmentos el valor de la conversión con una
 * resolución de 10 bits.
 *
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 01/12/23 - Creacion del archivo:
 *            Se han creado el programa y se ha verificado que funciona
 *            correctamente
 * 03/12/23 - Se ha documentado este archivo y se ha terminado
 */

#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <types.h>
#include <timer.h>
#include <sieteseg.h>
#include <ad.h>

int main () {
  char c;
  /*Diferencial a usar en los direccionamientos para distinguir puerto 0 y 1 */

  uint16_t resultadoAnterior = 0;

  /* Deshabilitamos interrupciones */
  lock ();

  serial_init();
  sieteSeg_init(); ///< Inicializamos el 7 segmentos
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  serial_print("\r\n" __FILE__ " ==========\r\n");
  
  unlock ();

  while(1) {
    // Quitamos posible pulsación pendiente
    if (serial_receive_pending()) serial_recv();
    /* Elección del puerto */
    serial_print("\r\nPuerto conversor a utilizar (0 - 1)?:");
    while((c = serial_recv()) != '0' && c != '1');
    serial_send(c); /* a modo de confirmación*/
    ad_set_conversor(c - '0');

    /* Elección del pin dentro del puerto */
    serial_print("\r\nPin del puerto a utilizar (0 - 7)?:");
    while((c = serial_recv()) < '0' || c > '7');
    serial_send(c); /* a modo de confirmación*/
    ad_init();
    /*Pasamos a configurar AD correspondiente*/
    // _io_ports[M6812_ATD0CTL2 + DirAD] = M6812B_ADPU; /*Encendemos, justf. izda*/
    // _io_ports[M6812_ATD0CTL3 + DirAD] = 0; /*Sin fifo*/
    ad_set_fifo(0);
    ad_justify_right(1);



    /* resolución de 10 bits y 16 ciclos de muestreo */
    // _io_ports[M6812_ATD0CTL4 + DirAD] = M6812B_SMP0 | M6812B_SMP1;
    ad_set_sampling_time(16);
    ad_set_conversion_size_10(1);


    /* Modo scan con 8 resultados sobre el pin seleccionado */
    // _io_ports[M6812_ATD0CTL5 + DirAD] = M6812B_S8C;
    ad_set_init_pin(c - '0');
    ad_set_continuous_conversions(1);
    ad_set_sucesive_conversions(8);
    ad_start();
    ad_print_status();

    serial_print("\r\nConvirtiendo (pulsa para salir)\r\n");

    char simbolo[] = "/|\\-*";
    uint16_t itera = 0;
#define ITERA_CAMBIO (5000)
    while(!serial_receive_pending()) {

      itera++;
      if (!(itera % ITERA_CAMBIO)) {
        uint8_t simAct = itera/ITERA_CAMBIO;
        if(!simbolo[simAct]) {
          itera = 0;
          simAct = 0;
        }
        serial_send(simbolo[simAct]);
        serial_send('\b');
      }

      /* Esperamos a que se termine la conversión */
      // while(! (_io_ports[M6812_ATD0STAT0 + DirAD] & M6812B_SCF));
      ad_wait();

      /*Vemos si los 8 resultados son iguales */
      uint16_t resultado = ad_retrieve(0);
      uint8_t iguales = 1;
      for(uint8_t i = 0; iguales && i < 1; i++)
        iguales = resultado == ad_retrieve(i);
      if(! iguales)
        continue;
      if (resultado == resultadoAnterior)
        continue;

      /* Los 8 resultados son iguales y distintos a lo que teníamos antes*/
      serial_print("  Nuevo valor = ");
      serial_printdecword(resultado);
      sieteSeg_valor(resultado); ///< Mostramos el valor en el 7 segmentos
      serial_print("   \r");
      resultadoAnterior = resultado;
    }
  }
}
