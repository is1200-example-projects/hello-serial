#include <proc/p32mx320f128h.h>

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

int calculate_baudrate_divider(int sysclk, int baudrate, int highspeed) {
	int pbclk, uxbrg, divmult;
	
	divmult = (highspeed) ? 4 : 16;
	/* Periphial Bus Clock is divided by PBDIV in OSCCON */
	pbclk = sysclk >> (OSCCONbits.PBDIV);
	
	/* Multiply by two, this way we can round the divider up if needed */
	uxbrg = ((pbclk * 2) / (divmult * baudrate)) - 2;
	/* We'll get closer if we round up */
	if (uxbrg & 1)
		uxbrg >>= 1, uxbrg++;
	else
		uxbrg >>= 1;
	return uxbrg;
}

void init() {
	/* On Uno32, we're assuming we're running with sysclk == 80 MHz */
	/* Periphial bust can run at a maximum of 40 MHz, setting PBDIV to 1 divides sysclk with 2 */
	OSCCONbits.PBDIV = 1;
}

int main(void) {
	unsigned char tmp;
	delay(10000000);
	ODCE = 0;
	TRISECLR = 0xFF;

	init();
	
	/* Configure UART1 for 115200 baud, no interrupts */
	U1BRG = calculate_baudrate_divider(80000000, 115200, U1MODEbits.BRGH);
	U1STA = 0;
	/* 8-bit data, no parity, 1 stop bit */
	U1MODE = 0x8000;
	/* Enable transmit and recieve */
	U1STASET = 0x1400;

	for (;;) {
		while (!U1STAbits.URXDA);
		tmp = U1RXREG & 0xFF;
		U1TXREG = tmp;
		PORTE = tmp;
		//U1TXREG = 'U';
	}

	return 0;
}

