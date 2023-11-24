#ifndef AD_H_
#define AD_H_

#include <sys/interrupts.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/sio.h>
#include <types.h>

/*! @defgroup da Digital to Analog Converter

  Set of functions to work with the analog to digital converter module.

  @{
*/

void ad_print_status();

/**
 * @brief      Initializes the AD module.
 */
void ad_init();

/**
 * @brief      Sets the default modulator.
 *
 * @param[in]  modulator  Specify which modulator to use.
 */
void ad_set_conversor(uint8_t modulator);

/**
 * @brief      Activate or desactivate fifo.
 *
 * @param[in]  fifo  Specify if the AD value should be stored in the FIFO or
 *             not. fifo > 0 ? FIFO : no FIFO
 */
void ad_set_fifo(uint8_t fifo);

/**
 * @brief      Sets the justify.
 *
 * @param[in]  justify  Specify if the AD value should be justified to the right
 *             or not. justify > 0 ? right : left
 */
void ad_justify_right(uint8_t justify);

/**
 * @brief      Sets the increase conversion size to 10.
 *
 * @param[in]  increase  Specify if the AD value should be increased or not.
 *             increase > 0 ? 10 bits : 8 bits
 */
void ad_set_conversion_size_10(uint8_t increase);

/**
 * @brief      Sets the sampling time.
 *
 * @param[in]  size  Specify the sampling time. (1, 2, 4, 16)
 */
void ad_set_sampling_time(uint8_t size);

/**
 * @brief      Sets the sucesive conversions.
 *
 * @param[in]  amount  The amount of sucesive conversions. (1, 4, 8)
 */
void ad_set_sucesive_conversions(uint8_t amount);

/**
 * @brief      Sets the read mode.
 *
 * @param[in]  mult  Specify wther de AD module should do multiple conversions
 *             or not. mult > 0 ? multiple : single
 */
void ad_set_read_mode(uint8_t mult);

/**
 * @brief      Sets the init pin (0-7).
 *
 * @param[in]  pin   The pin
 */
void ad_set_init_pin(uint8_t pin);

/**
 * @brief      Sets the continuous conversions.
 *
 * @param[in]  cont  Specify wether the AD module should do continuous or not.
 *             cont > 0 ? continuous : not continuous
 */
void ad_set_continuous_conversions(uint8_t cont);

/**
 * @brief      Starts the AD module.
 */
void ad_start();

/**
 * @brief      Waits for the AD module to finish.
 */
void ad_wait();

/**
 * @brief      Retrieves the AD value.
 *
 * @param[in]  offset  The offset
 *
 * @return     The read value.
 */
uint16_t ad_retrieve(uint8_t offset);


/**
 * @brief      Retrieves multiple values from the modulator.
 *
 * @param[out] values Array where the result will be stored.
 * @param[in]  amount The size of the array <=> Amount of values to read.
*/
void ad_retrieve_all(uint16_t *values, uint8_t amount);

/**
 * @brief      Installs a callback to be ran after a measurement is done.
 *
 * @param[in]  callback  The callback
 */
void ad_install_callback(void (*callback)());

/*! @} */

#endif /* AD_H_ */
