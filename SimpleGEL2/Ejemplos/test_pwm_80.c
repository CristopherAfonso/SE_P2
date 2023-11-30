/**
 *
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Sistemas Empotrados
 * Curso: 4º
 * Práctica 01: Libreria PWM
 * @file test_pwm_2.c
 * @author Cheuk Kelly Ng Pante (alu0101364544@ull.edu.es)
 * @author Carlos Pérez Fino (alu0101340333@ull.edu.es)
 * @author Alberto Hamilton
 * @brief
 * @version 0.1
 * @date
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>
#include <types.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include <pwm.h>

int main() {
  serial_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");

  while (1) {
    char c;
    serial_print("\r\nIndica canal a utilizar (0-3): ");
    while(((c = serial_recv()) < '0') || (c > '3'));
    serial_send(c); /* a modo de confirmación*/
    uint8_t canal = c - '0';

    serial_print("\r\nIndica Factor división del reloj (0-7): ");
    while(((c = serial_recv()) < '0') || (c > '7'));
    serial_send(c); /* a modo de confirmación*/
    pwm_set_clock((canal == 0) || (canal == 1), c - '0');

    serial_print("\r\nIndica polaridad (0-1): ");
    while(((c = serial_recv()) < '0') || (c > '1'));
    serial_send(c); /* a modo de confirmación*/
    pwm_set_polarity(canal, c - '0');


    serial_print("\r\nIndica alineamiento (1=Izquierdo, 0=Centrado): ");
    while(((c = serial_recv()) < '0') || (c > '1'));
    serial_send(c); /* a modo de confirmación*/
    pwm_set_alignment(c - '0');


    serial_print("\r\nIndica periodo del canal: ");
    uint8_t periodo = serial_getdecbyte();
    pwm_set_channel_period(canal, periodo);

    serial_print("\r\nIndica duty inicial: ");
    uint8_t duty = serial_getdecbyte();
    pwm_modify_channel_duty_steps(canal, duty);
    pwm_channel_enable(canal);

    pwm_print_status();

    while(1) {
      serial_print("\r\nCambiar Duty, cambiar Porcentaje, cambiar Canal (D,P,C)");
      while(((c = (serial_recv()| 0x20)) != 'd')  && (c != 'p') && (c != 'c'))
        serial_send(c); /* a modo de confirmación*/

      if(c == 'c') break;
      if(c == 'd') {
        serial_print("\r\nIndica duty: ");
        uint8_t duty = serial_getdecbyte();
        pwm_modify_channel_duty_steps(canal, duty);
      } else {
        serial_print("\r\nIndica porcentaje: ");
        uint8_t porcent = serial_getdecbyte();
        pwm_modify_duty_percentage (canal, porcent);
      }
      pwm_print_status();
    }
  }
}
