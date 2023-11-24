/**
 *
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Sistemas Empotrados
 * Curso: 4º
 * Práctica 01: Libreria PWM
 * @file test_pwm_1.c
 * @author Cheuk Kelly Ng Pante (alu0101364544@ull.edu.es)
 * @author Carlos Pérez Fino (alu0101340333@ull.edu.es)
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
  serial_print("\r\nTest PWM");
  serial_print("\r\n######### Registros #########");
  pwm_print_status();
}
