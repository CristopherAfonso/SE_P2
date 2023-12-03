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
 * @file prueba_teclado.c
 * @brief Archivo que contiene el programa que se encarga de que las funciones
 * definidas en teclado.h y implementadas en teclado.c funcionen correctamente
 *
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 01/12/23 - Creacion del archivo:
 *            Se han creado el programa de pruebas, y se ha verificado que todas
 *            las funciones implementadas en teclado.c funcionan correctamente
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

int main() {
  lock(); ///< Deshabilitamos interrupciones
 
  serial_init();
  sieteSeg_init();
  timer_init(3);  ///< factor escalado a 3 = un milisegundo
  teclado_init();
  serial_print("\r\n" __FILE__ " ==========\r\n");
 
  unlock();  ///< Habilitamos interrupciones
  
  uint16_t valSeg = 0;
  sieteSeg_valor(valSeg); ///< El decodificador tiene valor por defecto 0
  
  while (1) {
    serial_print("\r\nValor puesto: ");
    char val = teclado_getch_timeout(1000); ///< Pedimos valor por teclado
    serial_send(val);
    
    /// Antes de seguir con la siguiente iteracion, comprobamos que el
    /// valor siguiente no pueda ser mayor que 65535 (2^16) porque eso daría
    /// un error de overflow en el decodificador
    if (val >= '0' && val <= '9') {
		  valSeg = valSeg - ((valSeg/1000) * 1000); 
		  valSeg = valSeg * 10;
		  valSeg += val - '0';
		  sieteSeg_valor(valSeg);
	  }
  }

  return 0;
}
