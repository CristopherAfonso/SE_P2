#include <ad.h>

uint16_t dir_ad_ = 0;

uint8_t ATD0CTL4 = 0;
uint8_t ATD0CTL5 = 0;

void (*callback_ad_)() = 0;

void ad_print_status() {
  serial_print("\r\ndir_ad_: ");
  serial_printdecword(dir_ad_);
  serial_print("\n\rATD0CTL2: ");
  serial_printbinbyte(_io_ports[M6812_ATD0CTL2 + dir_ad_]);
  serial_print("\n\rATD0CTL3: ");
  serial_printbinbyte(_io_ports[M6812_ATD0CTL3 + dir_ad_]);
  serial_print("\n\rATD0CTL4: ");
  serial_printbinbyte(_io_ports[M6812_ATD0CTL4 + dir_ad_]);
  serial_print("\n\rATD0CTL5: ");
  serial_printbinbyte(_io_ports[M6812_ATD0CTL5 + dir_ad_]);
}

void ad_init() {
  _io_ports[M6812_ATD0CTL2 + dir_ad_] = M6812B_ADPU;
  _io_ports[M6812_ATD0CTL2 + dir_ad_] |= M6812B_AFFC;
}

void ad_set_conversor(uint8_t modulator) {
  dir_ad_ = modulator ? (M6812_ATD1CTL0 - M6812_ATD0CTL0) : 0;
}

void ad_set_fifo(uint8_t fifo) {
  _io_ports[M6812_ATD0CTL3 + dir_ad_] |= (fifo > 0) * M6812B_FIFO;
  _io_ports[M6812_ATD0CTL3 + dir_ad_] &=
      ((fifo == 0) * ~M6812B_FIFO) | ((fifo > 0) * 0xFF);
}

void ad_justify_right(uint8_t justify) {
  _io_ports[M6812_ATD0CTL2 + dir_ad_] |= (justify > 0) * M6812B_DJM;
  _io_ports[M6812_ATD0CTL2 + dir_ad_] &=
      ((justify == 0) * ~M6812B_DJM) | ((justify > 0) * 0xFF);
}

void ad_set_conversion_size_10(uint8_t increase) {
  ATD0CTL4 |= (increase > 0) * M6812B_RES10;
  ATD0CTL4 &= ((increase == 0) * ~M6812B_RES10) | ((increase > 0) * 0xFF);
}

void ad_set_sampling_time(uint8_t size) {
  ATD0CTL4 &= ~(M6812B_SMP0 | M6812B_SMP1);
  // ATD0CTL4 |= (M6812B_SMP0 * !(size & 0x07)) | (M6812B_SMP1 * !(size & 0x0F));
  uint8_t smp = size >> 2;
  smp = smp < 3 ? smp : 3; // 16 se codifica 3
  ATD0CTL4 |= (smp << 5);
}

void ad_set_sucesive_conversions(uint8_t amount) {
  ATD0CTL5 &= ~M6812B_S8C;
  _io_ports[M6812_ATD0CTL3 + dir_ad_] &= ~M6812B_S1C;
  ATD0CTL5 |= (M6812B_S8C * !(amount & 0x07));
  _io_ports[M6812_ATD0CTL3 + dir_ad_] |= (M6812B_S1C * !!(amount & 0x03));
}

void ad_set_read_mode(uint8_t mult) {
  ATD0CTL5 |= (mult > 0) * M6812B_MULT;
  ATD0CTL5 &= ((mult == 0) * ~M6812B_MULT) | ((mult > 0) * 0xFF);
}

void ad_set_init_pin(uint8_t pin) { ATD0CTL5 |= pin & 7; }

void ad_set_continuous_conversions(uint8_t cont) {
  ATD0CTL5 |= (cont > 0) * M6812B_SCAN;
  ATD0CTL5 &= ((cont == 0) * ~M6812B_SCAN) | ((cont > 0) * 0xFF);
}

void ad_start() { /* Guardar los registros que arrancan la conversión */
  _io_ports[M6812_ATD0CTL4 + dir_ad_] = ATD0CTL4;
  _io_ports[M6812_ATD0CTL5 + dir_ad_] = ATD0CTL5;
}

void ad_wait() {
  while (!(_io_ports[M6812_ATD0STAT0 + dir_ad_] & M6812B_SCF));
}

uint16_t ad_retrieve(uint8_t offset) {
  return _IO_PORTS_W(M6812_ADR00H + dir_ad_ + (offset & 0x07) * 2);
}

void ad_retrieve_all(uint16_t *values, uint8_t amount) {
  for (uint8_t i = 0; i < (amount) && i < 8; i++) values[i] = ad_retrieve(i);
}

void ad_install_callback(void (*callback)()) {
//  unlock();
  _io_ports[M6812_ATD0CTL2 + dir_ad_] |= M6812B_ASCIE;
  callback_ad_ = callback;
}

void __attribute__((interrupt)) vi_atd(void) {
//  serial_print("\r\n");
//  serial_printbinbyte(_io_ports[M6812_ATD0CTL2 + dir_ad_]);
  ad_retrieve(0);
  if (callback_ad_) callback_ad_();
}
