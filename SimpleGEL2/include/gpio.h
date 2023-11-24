/**
 * @author Carmen Clara Rocío Machado
 * @author Ángel Javier Díaz Balboa
 * @date 23/11/2023
 *
*/

#ifndef _GPIO_H
#define _GPIO_H

#include <types.h>
#include <sys/param.h>
#include <sys/ports.h>
#include <sys/interrupts.h>

/*! @defgroup gpio General Purpose Input Output
  
  Function set specialized for interacting with ports either in input or output
  mode. Also provides utility functions like gpio_strtobin()

  @{
*/

/** Transform a char pointer string to an integer that corresponds
    to the binary codes specified
    @param str The string. The bit contained by the right-most char is assigned
           to the first bit of the integer
    @return An uint8_t containing the binary equivalent
*/
uint8_t gpio_strtobin(char* str);

/** Set a port pin as input or output
    @param port The wanted port
    @param pin The wanted pin
    @param out_enabled Whether input (0) or output (1) is wanted
*/
void gpio_setportpinasinout(char port, int pin, int out_enabled);

/** Set the pins of a port freely
    @param port The wanted port
    @param pin_values An uint8_t containing an array-like binary number
*/
void gpio_setportasinout(char port, uint8_t pin_values);

/** Set or unset a certain pin of a port
    @param port The wanted port
    @param pin The wanted pin
    @param set Specify if it must be set or unset
*/
void gpio_writepinval(char port, int pin, int set);

/*! Set the entire value of a port
    @param port The wanted port
    @param val An uint_8 containg the value for each pin; may be converted
    using the utility function StrToBin
*/
void gpio_writeport(char port, uint8_t val);

/*! Set whether an entire port in input mode has its pull-up active or inactive
    @param port The wanted port
    @param set Enable/disable pull-up
*/
void gpio_pullupenable(char port, int set);

/** Read the value of a certain pin of a port
    @param port The wanted port
    @param pin The wanted pin
    @return An int containg the pin value
*/
int gpio_readpinval(char port, int pin);

/*! Read the entire value of a port
    @param port The wanted port
    @return An uint_8 containg the port value
*/
uint8_t gpio_readport(char port);

/*! Install interrupt handling functions for a pin 
    @param port The wanted port (G - H)
    @param pin The wanted pin
    @param custom_interrupt_func The funcion to install
    @return Return 1 if the installation is correct
*/
int gpio_installhandler(char port, int pin, void (*custom_interrupt_func)(void));

/**@}*/

#endif /* _GPIO_H */
