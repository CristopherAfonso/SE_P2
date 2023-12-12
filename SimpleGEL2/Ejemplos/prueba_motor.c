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
 * @brief Archivo que lee por teclado numérico un número de 0-100, lo
 * representa en los 7-segmentos (a medida que el usuario lo va introduciendo)
 * y haga que el motor gire a una velocidad proporcional al número introducido.
 * Se usan las teclas * y # para confirmar y cancelar la introducción del
 * número, respectivamente.
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
  lock(); ///< Deshabilitamos interrupciones
 
  serial_init();
  sieteSeg_init();
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  teclado_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");
  
  /// Realizamos la configuración del motor o PWM
  pwm_set_clock(0, 0);
  pwm_set_polarity(0, 1);
  pwm_set_alignment(1);
  pwm_set_channel_period(0, 200);
  pwm_modify_channel_duty_steps(0, 0);
  pwm_channel_enable(0);
 
  unlock(); ///< Habilitamos interrupciones
  
  /// Valores a pasar al 7 segmentos y al PWM
  uint16_t valSeg = 0;
  uint16_t valMot = 0;

  sieteSeg_valor(valSeg); ///< valor por defecto a mostrar
  
  while (1) {
    // Pedir un valor por teclado
    char val = teclado_getch_timeout(1000);
    
    /// Si el usuario da un valor entre 0-9 se recoge
    if (val >= '0' && val <= '9') {
		  uint16_t newValSeg = valSeg;
		  newValSeg = newValSeg * 10;
		  newValSeg += val - '0';

      /// Siempre que sea menor o igual a 100 se muestra
		  if (newValSeg <= 100) {
			  valSeg = newValSeg;
			  sieteSeg_valor(valSeg);
		  }
	  } else if (val == '#') { 
		  valSeg /= 10;
		  sieteSeg_valor(valSeg);
	  } else if (val == '*') {
		  /// la segunda vez que se pulsa el '*' se limpia el 7seg
		  sieteSeg_valor(valSeg);
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
