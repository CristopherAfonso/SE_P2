/**
 * @author Carmen Clara Rocío Machado
 * @author Ángel Javier Díaz Balboa
 * @date 23/11/2023
*/

#include <gpio.h>

void(*handler_function[16])(void) = {0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0};

uint8_t gpio_strtobin(char* str) {
  uint8_t bin_arr = 0x0;
  uint8_t bit = 0x1;
  if (str != 0) {
    for (int i = 0; str[i] != '\0' && i < 8; ++i) {
      bin_arr <<= 1;
      if (str[i] == '1') {
        bin_arr += bit;
      }
    }
  }
  return bin_arr;
}

void gpio_setportpinasinout(char port, int pin, int out_enabled) {
  int dir = 0;
  switch (port) {
    case 'A': dir = M6812_DDRA; break;
    case 'B': dir = M6812_DDRB; break;
    case 'E': dir = M6812_DDRE; break;
    case 'G': dir = M6812_DDRG; break;
    case 'H': dir = M6812_DDRH; break;
    case 'T': dir = M6812_DDRT; break;
    case 'P': dir = M6812_DDRP; break;
    case 'S': dir = M6812_DDRS; break;
  };
  _io_ports[dir] = (out_enabled == 0) ? (_io_ports[dir] & ~(1 << pin))
                                      : (_io_ports[dir] | (1 << pin));
}

void gpio_setportasinout(char port, uint8_t pin_values) {
  switch (port) {
    case 'A': _io_ports[M6812_DDRA] = pin_values; break;
    case 'B': _io_ports[M6812_DDRB] = pin_values; break;
    case 'E': _io_ports[M6812_DDRE] = pin_values; break;
    case 'G': _io_ports[M6812_DDRG] = pin_values; break;
    case 'H': _io_ports[M6812_DDRH] = pin_values; break;
    case 'T': _io_ports[M6812_DDRT] = pin_values; break;
    case 'P': _io_ports[M6812_DDRP] = pin_values; break;
    case 'S': _io_ports[M6812_DDRS] = pin_values; break;
  };

}

void gpio_writepinval(char port, int pin, int set) {
  int dir = 0;
  switch (port) {
    case 'A': dir = M6812_PORTA; break;
    case 'B': dir = M6812_PORTB; break;
    case 'E': dir = M6812_PORTE; break;
    case 'G': dir = M6812_PORTG; break;
    case 'H': dir = M6812_PORTH; break;
    case 'T': dir = M6812_PORTT; break;
    case 'P': dir = M6812_PORTP; break;
    case 'S': dir = M6812_PORTS; break;
  };
  _io_ports[dir] = (set == 0) ? (_io_ports[dir] & ~(1 << pin))
                              : (_io_ports[dir] | (1 << pin));
}

void gpio_writeport(char port, uint8_t val) {
  switch (port) {
    case 'A': _io_ports[M6812_PORTA] = val; break;
    case 'B': _io_ports[M6812_PORTB] = val; break;
    case 'E': _io_ports[M6812_PORTE] = val; break;
    case 'G': _io_ports[M6812_PORTG] = val; break;
    case 'H': _io_ports[M6812_PORTH] = val; break;
    case 'T': _io_ports[M6812_PORTT] = val; break;
    case 'P': _io_ports[M6812_PORTP] = val; break;
    case 'S': _io_ports[M6812_PORTS] = val; break;
  };
}

void gpio_pullupenable(char port, int set) {
  int dir = 0;
  int mask = 0;
  switch (port) {
    case 'A': dir=M6812_PUCR; mask = M6812B_PUPA; break;
    case 'B': dir=M6812_PUCR; mask = M6812B_PUPB; break;
    case 'E': dir=M6812_PUCR; mask = M6812B_PUPE; break;
    case 'G': dir=M6812_PUCR; mask = M6812B_PUPG; break;
    case 'H': dir=M6812_PUCR; mask = M6812B_PUPH; break;
    case 'T': dir=M6812_TMSK2; mask = (M6812B_PUPT); break;
    case 'P': dir=M6812_PWCTL; mask = (M6812B_PUPP); break;
    case 'S': dir=M6812_PURDS; mask = (0x7); break; //3 last bits
  };

  _io_ports[dir] = (set == 0) ? (_io_ports[dir] & ~(mask))
                              : (_io_ports[dir] | (mask));
}

int gpio_readpinval(char port, int pin) {
  int dir = 0;
  switch (port) {
    case 'A': dir = M6812_PORTA; break;
    case 'B': dir = M6812_PORTB; break;
    case 'E': dir = M6812_PORTE; break;
    case 'G': dir = M6812_PORTG; break;
    case 'H': dir = M6812_PORTH; break;
    case 'T': dir = M6812_PORTT; break;
    case 'P': dir = M6812_PORTP; break;
    case 'S': dir = M6812_PORTS; break;
  };
  return ((_io_ports[dir] & (1 << pin)) ? 0x01 : 0x00);
}

uint8_t gpio_readport(char port) {
  uint8_t read_val = 0x0;

  switch (port) {
    case 'A': read_val = _io_ports[M6812_PORTA]; break;
    case 'B': read_val = _io_ports[M6812_PORTB]; break;
    case 'E': read_val = _io_ports[M6812_PORTE]; break;
    case 'G': read_val = _io_ports[M6812_PORTG]; break;
    case 'H': read_val = _io_ports[M6812_PORTH]; break;
    case 'T': read_val = _io_ports[M6812_PORTT]; break;
    case 'P': read_val = _io_ports[M6812_PORTP]; break;
    case 'S': read_val = _io_ports[M6812_PORTS]; break;
  };

  return read_val;
}

int gpio_installhandler(char port, int pin, void (*custom_interrupt_func)
(void)) {
  int done = 0;
  if (pin >= 0 && pin <= 7) {
    if (port == 'G') {
      handler_function[pin] = custom_interrupt_func;
      _io_ports[M6812_KWIFG] = (1 << pin);
      _io_ports[M6812_KWIEG] |= (1 << pin);
      done = 1;
    }
    else if (port == 'H') {
      handler_function[8 + pin] = custom_interrupt_func;
      _io_ports[M6812_KWIFH] = (1 << pin);
      _io_ports[M6812_KWIEH] |= 1 << pin;
      done = 1;
    }
  }
  return done;
}

void __attribute__((interrupt)) vi_kwgh(void) {
  int enabled_flag = -1;

  for (int pin = 0; pin < 16; ++pin) {
    if (pin < 8) {
      if ((_io_ports[M6812_KWIFG] & (1 << pin)) && (_io_ports[M6812_KWIEG]
          & (1 << pin))) {
        enabled_flag = pin;
        _io_ports[M6812_KWIFG] = (1 << pin);
        break;
      }
    }
    else {
      if ((_io_ports[M6812_KWIFH] & (1 << (pin - 8))) &&
          (_io_ports[M6812_KWIEH] & (1 <<(pin - 8)))) {
        enabled_flag = pin;
        _io_ports[M6812_KWIFH] = (1 << (pin - 8));
        break;
      }
    }
  }

  if (enabled_flag != -1 && handler_function[enabled_flag] != 0) {
    handler_function[enabled_flag]();
  }
}
