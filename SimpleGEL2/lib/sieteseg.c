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
 * @file sieteseg.c
 * @brief Donde se aloja el código de las funciones del primer punto de
 * la práctica 2, que son 'sieteSeg_init', 'sieteSeg_digitos' y
 * 'sieteSeg_valor'
 *
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 30/11/23 - Creacion del archivo:
 *            Se han creado todos los cuerpos de cada función que
 *            usará este archivo, se han terminado todas las funciones
 * 03/12/23 - Se ha documentado este archivo y se ha terminado
 */

#include <gpio.h>
#include <sys/interrupts.h>
#include <sys/param.h>
#include <timer.h>
#include <types.h>

/// Definimos el puerto a usar que será el puerto 7 del microcontrolador
#define PUERTO7 'G'

/// Definimos variables globales a usar para el 7 segmentos
uint8_t nums[] = {1, 3, 5, 6}; ///< Array que contiene los números a mostrar
uint8_t currentDisp = 0; ///< Variable que indica el display actual

/**
 * @brief Función que se encarga de mostrar los números en el display
 * 
 * @param params Puntero a los parámetros que se le pasan a la función
 */
void task_segs(void* params) {
  uint8_t final = 1 << (4 + currentDisp);
  final += nums[currentDisp] & 0xF;

  if (nums[currentDisp] < 10) {
    gpio_writeport(PUERTO7, final);
  }
  
  currentDisp = (currentDisp + 1) % 4;
}

/**
 * @brief Función que se encarga de realiza las inicializaciones necesarias.
 * 
 */
void sieteSeg_init() {
  uint8_t param = 0;
  
  /// Configuramos el puerto 7 como salida
  gpio_setportasinout(PUERTO7, 0xff); 

  /// Añadimos la tarea periódica que se encarga de mostrar los números
  timer_add_periodic_task(task_segs, &param, 1000); 
}

/**
 * @brief recibirá puntero a array de al menos 4 bytes, a partir de
 * ese momento se mostrará en cada 7-segmentos valor correspondiente
 * a las primeras 4 posiciones del array pasado.
 * 
 * @param val Array de 4 elementos de tipo uint8_t que contiene los
 * números a mostrar
 */
void sieteSeg_digitos(uint8_t* val) {
  nums[0] = val[0];
  nums[1] = val[1];
  nums[2] = val[2];
  nums[3] = val[3];
}

/**
 * @brief recibirá entero y hará que se muestre su valor en decimal
 * en los 7-segmentos.
 * 
 * @param val Entero que se mostrará en los 7 segmentos
 */
void sieteSeg_valor(uint16_t val) {
  /// Cogemos el decimal menos significativo 4 veces y lo guardamos en el array
  for (uint8_t i = 0; i < 4; i++) {
    nums[i] = val % 10;
    val = val / 10;
  }
  
  /// Desactivamos los display que sean ceros a la izquierda
  for (uint8_t i = 0; i < 3; i++) {
    if (nums[3-i] == 0) {
		  nums[3-i] = 0xF;
	  } else {
		  break;
	  }
  }
}
