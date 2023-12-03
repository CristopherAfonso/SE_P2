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
 * @file sieteseg.h
 * @brief Donde se aloja la declaración de las funciones del primer punto de
 * la práctica 2, que son 'sieteSeg_init', 'sieteSeg_digitos' y
 * 'sieteSeg_valor'
 *
 * @bug No hay bugs conocidos
 *
 * @version 0.1
 * @copyright Copyright (c) 2023
 * @brief Historial de Revisiones
 * 30/11/23 - Creacion del archivo:
 *            Se ha creado la declaración de todas las funciones
 * 03/12/23 - Se ha documentado este archivo y se ha terminado
 */

#include <types.h>

void sieteSeg_init();
void sieteSeg_digitos(uint8_t* val);
void sieteSeg_valor(uint16_t val);
