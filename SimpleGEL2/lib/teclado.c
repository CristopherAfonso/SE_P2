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
 * @file teclado.c
 * @brief Donde se aloja el código de las funciones del tercer punto de
 * la práctica 2, que son  'teclado_init', 'teclado_getch' y
 * 'teclado_getch_timeout'
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 01/12/23 - Creacion del archivo:
 *            Se han creado todos los cuerpos de cada función que
 *            usará este archivo, se han terminado todas las funciones
 * 03/12/23 - Se ha documentado este archivo y se ha terminado
 */

#include <teclado.h>
#include <gpio.h>
#include <timer.h>
#include <sys/sio.h>

/// Puerto donde se conecta el teclado numerico
#define PUERTO 'H'

/// Máscaras para cada fila y columna del teclado
#define F1 (1 << 6)
#define F2 (1 << 1)
#define F3 (1 << 2)
#define F4 (1 << 4)
#define C1 (1 << 5)
#define C2 (1 << 7)
#define C3 (1 << 3)

/// Mascaras para todas las filas y columnas
#define FILAS (F1 | F2 | F3 | F4)
#define COLUMNAS (C1 | C2 | C3)

/**
 * @brief realiza las inicializaciones necesarias.
 * 
 */
void teclado_init() {
	gpio_setportasinout(PUERTO, FILAS);
	gpio_pullupenable(PUERTO, 1);
	gpio_writeport(PUERTO, 0);
}

/**
 * @brief espera a que se realice una pulsación en el teclado y
 * devuelve código ASCII de la tecla pulsada.
 * 
 * @return char caracter de la tecla pulsada
 */
char teclado_getch() {
	return teclado_getch_timeout(0);
}

/**
 * @brief espera a que se realice una pulsación en el teclado y
 * devuelve código ASCII de la tecla pulsada. Si no hay pulsación
 * en milis milisegundos devuelve el carácter 'T'.
 * 
 * @param milis milisegundos que se esperan a que se pulse una tecla
 * @return char caracter de la tecla pulsada o 'T' si no se ha pulsado
 */
char teclado_getch_timeout(uint32_t milis) {
	gpio_writeport(PUERTO, 0);
	
	// Si todas las columnas no estan a 1 esperamos (hay una tecla pulsada)
	while((gpio_readport(PUERTO) & COLUMNAS) != COLUMNAS);
	
	// esperamos 20 milisegundos que es el tiempo de estabilizacion
	timer_sleep_milis(20);
	
	// Esperamos a que haya una tecla pulsada
	uint32_t start_m = timer_milis();
	while((gpio_readport(PUERTO) & COLUMNAS) == COLUMNAS) {
		// Si se excede el tiempo de espera se devuelve 'T'
		if (milis > 0 && (start_m + milis < timer_milis())) return 'T';
	}
	
	// esperamos 20 milisegundos que es el tiempo de estabilizacion
	timer_sleep_milis(20);
	
	// hacemos un escaneo simple para la fila 1 :_(
	gpio_writeport(PUERTO, (F2 | F3 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '1';
		case (C1 | C3): return '2';
		case (C1 | C2): return '3';
		case (C1 | C2 | C3): break;
	}
	
	// hacemos un escaneo simple para la fila 2 :_(
	gpio_writeport(PUERTO, (F1 | F3 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '4';
		case (C1 | C3): return '5';
		case (C1 | C2): return '6';
		case (C1 | C2 | C3): break;
	}
	
	// hacemos un escaneo simple para la fila 3 :_(
	gpio_writeport(PUERTO, (F1 | F2 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '7';
		case (C1 | C3): return '8';
		case (C1 | C2): return '9';
		case (C1 | C2 | C3): break;
	}
	
	// hacemos un escaneo simple para la fila 4 :_(
	gpio_writeport(PUERTO, (F1 | F2 | F3));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '*';
		case (C1 | C3): return '0';
		case (C1 | C2): return '#';
		case (C1 | C2 | C3): break;
	}
	
	// Si ocurriera algún error se devuelve '?'
	return '?';
}
