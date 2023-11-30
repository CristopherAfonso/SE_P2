#include <teclado.h>
#include <gpio.h>

#define PUERTO 'H'

#define F1 (1 << 6)
#define F2 (1 << 1)
#define F3 (1 << 2)
#define F4 (1 << 4)
#define C1 (1 << 5)
#define C2 (1 << 7)
#define C3 (1 << 3)

void teclado_init() {
	gpio_setportasinout(PUERTO, F1 | F2 | F3 | F4);
	gpio_pullupenable(PUERTO, 1);
	gpio_writeport(PUERTO, 0);
}

char teclado_getch() {
	char resultado;
	
	return resultado;	
}

char teclado_getch_timeout(uint32_t milis) {
	char resultado;
	
	return resultado;	
}
