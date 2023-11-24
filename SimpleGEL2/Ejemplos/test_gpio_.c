/**
 * @author Carmen Clara Rocío Machado
 * @author Ángel Javier Díaz Balboa
 * @date 23/11/2023
*/

#include "gpio.h"
#include <sys/sio.h>

void call_if_interruption(void) {
  serial_print("\r\nINTERRUPCION DETECTADA");
}

int test_gpio_strtobin() {
  int test = gpio_strtobin("10011011") == 0x9C;
  if (test) test = gpio_strtobin("01101101") == 0x6D;
  if (test) test = gpio_strtobin("01010101") == 0x55;
  if (test) test = gpio_strtobin("10101010") == 0xAA; 
  return test; 
}

int main() {
  serial_init();
  
  ///Check gpio_strtobin()
  serial_print("\r\nComprobación de gpio_strtobin: ");
  serial_print((test_gpio_strtobin() == 1) ? "OK" : "BAD");
  
  while (0 == 0) {
    char elec = 'E';
    serial_print("\r\nEntrada (E), salida (S),"
    "interrupciones(I) o cerrar programa (C):");
    while((elec = serial_recv()) != 'E' && elec != 'S' && elec != 'C'
    && elec != 'I');
    serial_send(elec);
    if (elec == 'C') 
      break;
    
    if (elec == 'E') { ///Input
      elec = 'N';
      serial_print("\r\nActivar pull-up (Y/N):");
      while((elec = serial_recv()) != 'Y' && elec != 'N');
      serial_send(elec);
      if (elec == 'Y') { ///Pull-up
        AGAIN:
        elec = 'G';
        serial_print("\r\nPuerto a utilizar (A-B-E-G-H-T-P-S)?:");
        while((elec = serial_recv()) != 'A' && elec != 'B' && elec != 'E'
            && elec != 'G' && elec != 'H' && elec != 'T' && elec != 'P'
            && elec != 'S');
        serial_send(elec);
        gpio_setportasinout(elec, gpio_strtobin("00000000"));
        gpio_pullupenable(elec, 1);
        serial_print("\r\nPull-up activado para puerto "); serial_send(elec);
        
        ///Read
        char choice = 'R';
        int port_val = 0x0;
        int res = 0;
        while (choice != 'S' && choice != 's') {
          if (choice >= '0' && choice <= '7') {
            res = gpio_readpinval(elec, choice - '0');
            if (res > 0)
              port_val |= (1 << (choice - '0'));
            else
              port_val &= ~(1 << (choice - '0'));
          }
          else {          
            switch (choice) {                   
              case 'R':
              case 'r': port_val = gpio_readport(elec);
                      break;
              case 'P':
              case 'p': goto AGAIN;  
            };
          }           
           
          serial_print("\r\nValor ");
          serial_printbinbyte(port_val);
          serial_print(" Volver a leer, actualizar 1 bit, cambiar"
          " de puerto o salir (R/NUM_PIN/P/S):");
          choice = serial_recv();                   
        }
      }
      else { ///Without pull-up
        AGAIN_:
        elec = 'G';
        serial_print("\r\nPuerto a utilizar (A-B-E-G-H-T-P-S)?:");
        while((elec = serial_recv()) != 'A' && elec != 'B' && elec != 'E'
            && elec != 'G' && elec != 'H' && elec != 'T' && elec != 'P'
            && elec != 'S');
        serial_send(elec);
        gpio_setportasinout(elec, gpio_strtobin("00000000"));
        gpio_pullupenable(elec, 0);
        serial_print("\r\nPull-up desactivado para puerto "); serial_send(elec);
        
        ///Read
        char choice = 'R';
        int port_val = 0x0;
        int res = 0;
        while (choice != 'S' && choice != 's') {
          if (choice >= '0' && choice <= '7') {
            res = gpio_readpinval(elec, choice - '0');
            if (res > 0)
              port_val |= (1 << (choice - '0'));
            else
              port_val &= ~(1 << (choice - '0'));
          }
          else {          
            switch (choice) {                   
              case 'R':
              case 'r': port_val = gpio_readport(elec);
                      break;
              case 'P':
              case 'p': goto AGAIN_;  
            };
          }           
           
          serial_print("\r\nValor ");
          serial_printbinbyte(port_val);
          serial_print(" Volver a leer, actualizar 1 bit, cambiar"
          " de puerto o salir (R/NUM_PIN/P/S):");
          choice = serial_recv();                  
        }
      }
    }
    else if (elec == 'S') { ///Output
      elec = 'G';
      serial_print("\r\nPuerto a utilizar (A-B-E-G-H-T-P-S)?:");
      while((elec = serial_recv()) != 'A' && elec != 'B' && elec != 'E'
            && elec != 'G' && elec != 'H' && elec != 'T' && elec != 'P'
            && elec != 'S');
      serial_send(elec);
      gpio_setportasinout(elec, gpio_strtobin("11111111"));
      serial_print("\r\nSeleccionado el puerto "); serial_send(elec);
      uint8_t port_val = 0x0;
      char pin_to_change = '2';
      int write_full = 0; ///0 bit by bit, 1 the entire port
      gpio_writeport(elec, port_val); ///initialize entire port to 0
      
      do {        
        serial_print("\r\nValor actual del puerto "); serial_send(elec);
        serial_print("\r\n");
        serial_printbinbyte(port_val);
        serial_print("\r\nPresione número (7->0) para invertir un pin; "
        "'S' para salir; 'M' para cambiar de modo de "
        "escritura (puerto entero / bit a bit)");
        pin_to_change = serial_recv();
        
        if (pin_to_change >= '0' && pin_to_change <= '7') {
          port_val ^= 1 << (pin_to_change - '0');
          if (!write_full)
            gpio_writepinval(elec, (pin_to_change - '0'),
                            (port_val & (1 << (pin_to_change - '0'))));
        }
        
        else {        
          switch (pin_to_change) {    
            case 'M':
            case 'm': write_full ^= 0x1;
                      serial_print("\r\nCambiado modo de escritura a ");
                      if (write_full)
                        serial_print("puerto entero");
                      else
                        serial_print("bit a bit");
                      break;
          };        
        }
       
        if (write_full)       
          gpio_writeport(elec, port_val);
        
      } while (pin_to_change != 'S' && pin_to_change != 's');
    }
    else { ///Interruptions test
      elec = 'G';
      serial_print("\r\nPuerto a utilizar (G-H)?:");
      while((elec = serial_recv()) != 'G' && elec != 'H');      
      serial_send(elec);
      int pin_int = 4;
      serial_print("\r\nPin a utilizar (0-7)?:");
      while((pin_int = serial_recv()) < '0' || pin_int > '7');      
      serial_send(pin_int);
      int ok = gpio_installhandler(elec, pin_int - '0', call_if_interruption);
      if (ok) serial_print("\r\nInstalada función manejadora");
      else serial_print("\r\nError al instalar la función manejadora");
    }
  }
  
  return 0;
}

