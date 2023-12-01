#include <teclado.h>
#include <gpio.h>
#include <timer.h>
#include <sys/sio.h>

#define PUERTO 'H'

#define F1 (1 << 6)
#define F2 (1 << 1)
#define F3 (1 << 2)
#define F4 (1 << 4)
#define C1 (1 << 5)
#define C2 (1 << 7)
#define C3 (1 << 3)

#define FILAS (F1 | F2 | F3 | F4)
#define COLUMNAS (C1 | C2 | C3)

void teclado_init() {
	gpio_setportasinout(PUERTO, FILAS);
	gpio_pullupenable(PUERTO, 1);
	gpio_writeport(PUERTO, 0);
}

char teclado_getch() {
	return teclado_getch_timeout(0);
}

char teclado_getch_timeout(uint32_t milis) {
	gpio_writeport(PUERTO, 0);
	
	// Esperamos a que no haya ninguna tecla pulsada
	// Es decir, mientras que no esten todas las columnas a 1
	//uint32_t start_m = timer_milis();
	while((gpio_readport(PUERTO) & COLUMNAS) != COLUMNAS) {
		//if (milis > 0 && (start_m + milis < timer_milis())) return 'T';
	}
	
	// esperamos 20 milis
	timer_sleep_milis(20);
	
	// Esperamos a que haya una tecla pulsada
	uint32_t start_m = timer_milis();
	while((gpio_readport(PUERTO) & COLUMNAS) == COLUMNAS) {
		if (milis > 0 && (start_m + milis < timer_milis())) return 'T';
	}
	
	// esperamos 20 milis
	timer_sleep_milis(20);
	
	// hacemos un escaneo simple :_(
	gpio_writeport(PUERTO, (F2 | F3 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '1';
		case (C1 | C3): return '2';
		case (C1 | C2): return '3';
		case (C1 | C2 | C3): break;
		/*default: 
			serial_print("err: ");
			serial_printbinword(gpio_readport(PUERTO));
			serial_print("\n");
			break;*/
	}
	
	gpio_writeport(PUERTO, (F1 | F3 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '4';
		case (C1 | C3): return '5';
		case (C1 | C2): return '6';
		case (C1 | C2 | C3): break;
		/*default: 
			serial_print("err: ");
			serial_printbinword(gpio_readport(PUERTO));
			serial_print("\n");
			break;*/
	}
	
	gpio_writeport(PUERTO, (F1 | F2 | F4));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '7';
		case (C1 | C3): return '8';
		case (C1 | C2): return '9';
		case (C1 | C2 | C3): break;
		/*default: 
			serial_print("err: ");
			serial_printbinword(gpio_readport(PUERTO));
			serial_print("\n");
			break;*/
	}
	
	gpio_writeport(PUERTO, (F1 | F2 | F3));
	switch ((gpio_readport(PUERTO) & COLUMNAS)) {
		case (C2 | C3): return '*';
		case (C1 | C3): return '0';
		case (C1 | C2): return '#';
		case (C1 | C2 | C3): break;
		/*default: 
			serial_print("err: ");
			serial_printbinword(gpio_readport(PUERTO));
			serial_print("\n");
			break;*/
	}
	
	return '?';	
}
