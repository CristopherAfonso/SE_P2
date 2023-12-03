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
 * @file prueba_sieteseg.c
 * @brief Archivo que contiene el programa que se encarga de que las funciones
 * definidas en sieteSeg.h y implementadas en sieteSeg.c funcionen correctamente
 *
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 30/11/23 - Creacion del archivo:
 *            Se han creado el programa de pruebas, y se ha verificado que todas
 *            las funciones implementadas en sieteSeg.c funcionan correctamente
 * 03/12/23 - Se ha documentado este archivo y se ha terminado
 */

#include <gpio.h>
#include <sieteseg.h>
#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/sio.h>
#include <timer.h>
#include <types.h>

int main() {
  lock(); ///< Deshabilitamos interrupciones
 
  serial_init();
  sieteSeg_init(); ///< Inicializamos el 7 segmentos
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  serial_print("\r\n" __FILE__ " ==========\r\n");
  sieteSeg_valor(1234); ///< Valor inicial por defecto
 
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
