#include "basic_library.h"


void blinkLD2(unsigned int delay, int *output);

int main(void) {
	systick_init();
	int output = 1;

	while (1) {
		blinkLD2(500, &output);
	}
}

void blinkLD2(unsigned int delay, int *output) {
	setLD2(*output);
	delay_ms(500);
	*output = !(*output);
}



