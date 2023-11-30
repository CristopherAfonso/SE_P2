#include <gpio.h>
#include <sys/interrupts.h>
#include <sys/param.h>
#include <timer.h>
#include <types.h>

#define PUERTO7 'G'

uint8_t nums[] = {1, 3, 5, 6};
uint8_t currentDisp = 0;

void task_segs(void* params) {
  uint8_t final = 1 << (4 + currentDisp);
  final += nums[currentDisp] & 0xF;

  if (nums[currentDisp] < 10) {
	gpio_writeport(PUERTO7, final);
  }
  
  currentDisp = (currentDisp + 1) % 4;
}

void sieteSeg_init() {
  gpio_setportasinout(PUERTO7, 0xff);

  uint8_t param = 0;
  timer_add_periodic_task(task_segs, &param, 1000);
}

void sieteSeg_digitos(uint8_t* val) {
  nums[0] = val[0];
  nums[1] = val[1];
  nums[2] = val[2];
  nums[3] = val[3];
}

void sieteSeg_valor(uint16_t val) {
  for (uint8_t i = 0; i < 4; i++) {
    nums[i] = val % 10;
    val = val / 10;
  }
  
  for (uint8_t i = 0; i < 3; i++) {
    if (nums[3-i] == 0) {
		nums[3-i] = 0xF;
	} else {
		break;
	}
  }
}
