#include <proc/p32mx320f128h.h>

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

int main(void) {
	return 0;
}

