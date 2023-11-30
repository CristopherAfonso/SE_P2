/**
 *
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Sistemas Empotrados
 * Curso: 4º
 * Práctica 01: Libreria PWM
 * @file pwm.c
 * @author Cheuk Kelly Ng Pante (alu0101364544@ull.edu.es)
 * @author Carlos Pérez Fino (alu0101340333@ull.edu.es)
 * @brief Implementación de las funciones de la librería PWM
 * @version 0.1
 * @date 24-11-2023
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/pwm.h"

#define M6812_DESPL7_3 ~(7 << 3)
#define M6812_DESPL7_0 ~(7 << 0)

void pwm_set_clock(bool clock_A, int8_t factor_N) {
  if (clock_A == true) {  // reloj A -> canal 0 y 1
    // serial_print("Reloj A");
    _io_ports[M6812_PWCLK] &= M6812_DESPL7_3;
    _io_ports[M6812_PWCLK] |= (factor_N << 3);
    _io_ports[M6812_PWPOL] &= ~M6812B_PCLK0;
    _io_ports[M6812_PWPOL] &= ~M6812B_PCLK1;
  } else {  // reloj B -> canales 2 y 3
    // serial_print("Reloj B");
    _io_ports[M6812_PWCLK] &= M6812_DESPL7_0;
    _io_ports[M6812_PWCLK] |= (factor_N << 0);
    _io_ports[M6812_PWPOL] &= ~M6812B_PCLK2;
    _io_ports[M6812_PWPOL] &= ~M6812B_PCLK3;
  }
}

void pwm_set_polarity(int8_t port, bool up) {
  switch (port) {
    case 0:
      if (up == true) {
        _io_ports[M6812_PWPOL] |= M6812B_PPOL0;
      } else {
        _io_ports[M6812_PWPOL] &= ~M6812B_PPOL0;
      }
      break;
    case 1:
      if (up == true) {
        _io_ports[M6812_PWPOL] |= M6812B_PPOL1;
      } else {
        _io_ports[M6812_PWPOL] &= ~M6812B_PPOL1;
      }
      break;
    case 2:
      if (up == true) {
        _io_ports[M6812_PWPOL] |= M6812B_PPOL2;
      } else {
        _io_ports[M6812_PWPOL] &= ~M6812B_PPOL2;
      }
      break;
    case 3:
      if (up == true) {
        _io_ports[M6812_PWPOL] |= M6812B_PPOL3;
      } else {
        _io_ports[M6812_PWPOL] &= ~M6812B_PPOL3;
      }
      break;
  }
}

void pwm_set_alignment(bool left_align) {
  if (left_align == true) {  // Alineamiento a la izquierda
    _io_ports[M6812_PWCTL] &= ~M6812B_CENTR;
  } else {  // Alineamiento centrado
    _io_ports[M6812_PWCTL] |= M6812B_CENTR;
  }
}

void pwm_set_channel_period(int8_t channel, int8_t steps) {
  switch (channel) {
    case 0:
      _io_ports[M6812_PWPER0] = steps;
      break;
    case 1:
      _io_ports[M6812_PWPER1] = steps;
      break;
    case 2:
      _io_ports[M6812_PWPER2] = steps;
      break;
    case 3:
      _io_ports[M6812_PWPER3] = steps;
      break;
  }
}

uint8_t pwm_get_channel_period(int8_t channel) {
  return _io_ports[M6812_PWPER0 + channel];
}

void pwm_channel_enable(int8_t channel) {
  switch (channel) {
    case 0:
      _io_ports[M6812_PWEN] |= M6812B_PWEN0;
      break;
    case 1:
      _io_ports[M6812_PWEN] |= M6812B_PWEN1;
      break;
    case 2:
      _io_ports[M6812_PWEN] |= M6812B_PWEN2;
      break;
    case 3:
      _io_ports[M6812_PWEN] |= M6812B_PWEN3;
      break;
  }
}

void pwm_channel_disable(int8_t channel) {
  switch (channel) {
    case 0:
      _io_ports[M6812_PWPER0] &= ~M6812B_PWEN0;
      break;
    case 1:
      _io_ports[M6812_PWPER1] &= ~M6812B_PWEN1;
      break;
    case 2:
      _io_ports[M6812_PWPER2] &= ~M6812B_PWEN2;
      break;
    case 3:
      _io_ports[M6812_PWPER3] &= ~M6812B_PWEN3;
      break;
  }
}

void pwm_modify_channel_duty_steps(int8_t channel, int8_t steps) {
  _io_ports[M6812_PWDTY0 + channel] = steps;
}

void pwm_modify_duty_percentage(int8_t channel, int8_t percentage) {
  uint8_t period = pwm_get_channel_period(channel);
  pwm_modify_channel_duty_steps(channel, (period * percentage) / 100);
}

void pwm_print_status() {
  serial_print("\r\nM6812_PWCLK: ");
  serial_printbinbyte(_io_ports[M6812_PWCLK]);
  serial_print("\r\n");

  serial_print("PWPOL: ");
  serial_printbinbyte(_io_ports[M6812_PWPOL]);
  serial_print("\r\n");

  serial_print("PWEN: ");
  serial_printbinbyte(_io_ports[M6812_PWEN]);
  serial_print("\r\n");

  serial_print("PWPER0: ");
  serial_printdecbyte(_io_ports[M6812_PWPER0]);
  serial_print("\r\n");

  serial_print("PWPER1: ");
  serial_printdecbyte(_io_ports[M6812_PWPER1]);
  serial_print("\r\n");

  serial_print("PWPER2: ");
  serial_printdecbyte(_io_ports[M6812_PWPER2]);
  serial_print("\r\n");

  serial_print("PWPER3: ");
  serial_printdecbyte(_io_ports[M6812_PWPER3]);
  serial_print("\r\n");

  serial_print("PWDTY0: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY0]);
  serial_print("\r\n");

  serial_print("PWDTY1: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY1]);
  serial_print("\r\n");

  serial_print("PWDTY2: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY2]);
  serial_print("\r\n");

  serial_print("PWDTY3: ");
  serial_printdecbyte(_io_ports[M6812_PWDTY3]);
  serial_print("\r\n");
}
