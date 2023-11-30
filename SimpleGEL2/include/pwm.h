/**
 *
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Sistemas Empotrados
 * Curso: 4º
 * Práctica 01: Libreria PWM
 * @file pwm.h
 * @author Cheuk Kelly Ng Pante (alu0101364544@ull.edu.es)
 * @author Carlos Pérez Fino (alu0101340333@ull.edu.es)
 * @brief Definición de las funciones de la librería PWM
 * @version 0.2
 * @date 24-11-2023
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _PWM_H_
#define _PWM_H_

#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <types.h>

typedef enum { false, true } bool;

/*! @defgroup pwm Funciones de manejo del módulo PWM
 */

/** @{*/

/**
 * @brief Configurar el reloj base para un determinado canal
 *
 * @param clock_A A partir de la señal de reloj del sistema ECLK se general las
 * señales A y B aplicándole un divisor, independiente para cada señal.
 * @param factor_N Indica los valores válidos para N
 */
void pwm_set_clock(bool clock_A, int8_t factor_N);

/**
 * @brief Configurar la polaridad para un determinado canal
 *
 * @param port Establece el puerto
 * @param up Establece el tipo de polaridad
 */
void pwm_set_polarity(int8_t port, bool up);

/**
 * @brief Configurar el alineamiento para un determinado canal.
 * Este método permite estableces el tipo de alineamiento para poder modificar
 * los pulsos de reloj
 *
 * @param align Define el alineamiento de la señal de salida 1=izquierdo 0=centrado
 */
void pwm_set_alignment(bool left_align);

/**
 * @brief Configurar el número de etapas del periodo para un determinado canal,
 * permite establecer el número de etapas por periodo
 *
 * @param steps Establece el número de etapas del periodo
 * @param channel Determina el canal a configurar
 */
void pwm_set_channel_period(int8_t channel, int8_t steps);

/**
 * @brief Seleccionar el periodo para un determinado canal
 *
 * @param channel Determina el canal a configurar
 */
uint8_t pwm_get_channel_period(int8_t channel);

/**
 * @brief Habilitar un determinado canal
 *
 * @warning Se tiene que invocar antes el métodos pwm_set_clock()
 * para poder habilitar un determinado canal.
 *
 * @param channel Determina el canal a habilitar
 *
 */
void pwm_channel_enable(int8_t channel);

/**
 * @brief Deshabilitar un determinado canal
 *
 * @param channel Determina el canal a deshabilitar
 */
void pwm_channel_disable(int8_t channel);

/**
 * @brief Modificar el ciclo de trabajo (en etapas) para un determinado canal
 * Modifica el número de ciclos según las etapas de trabajo
 *
 * @param steps Define el número de etapas
 * @param channel Determina el canal a configurar
 */
void pwm_modify_channel_duty_steps(int8_t channel, int8_t steps);

/**
 * @brief Modificar el ciclo de trabajo (en porcentaje) para un determinado
 * canal
 *
 * @param percentage Define el porcentaje de trabajo
 * @param channel Determina el canal a configurar
 */
void pwm_modify_duty_percentage(int8_t channel, int8_t percentage);

/**
 * @brief Muestra todos los registros en bytes
 *
 */
void pwm_print_status();

/** @}*/

#endif  // _PWM_H_
