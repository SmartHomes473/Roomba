/******************************************************************************
* Initialize UART at 9600 Baud
******************************************************************************/
void UART_init_9600()
{
	/* Set P1.1 = RX and P1.2 = TX */
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;

	/* Set USCI to use the SMCLK */
	UCA0CTL1 |= UCSSEL_2;

	/* Set Baud rate to 9600 */
	UCA0BR0 = BAUD_9600;
	UCA0BR1 = 0;

	/* Set modulation */
	UCA0MCTL = UCBRS0;

	/* Reset UART state machine */
	UCA0CTL1 &= ~UCSWRST;

	/* Enable interrupt on UART channel 1 receive */
	IE2 |= UCA0RXIE;
}

/******************************************************************************
* Initialize UART at 57600 Baud
******************************************************************************/
void UART_init_57600()
{
	/* Set P1.1 = RX and P1.2 = TX */
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;

	/* Set USCI to use the SMCLK */
	UCA0CTL1 |= UCSSEL_2;

	/* Set Baud rate to 9600 */
	UCA0BR0 = BAUD_57600;
	UCA0BR1 = 0;

	/* Set modulation */
	UCA0MCTL = 0x0F;

	/* Reset UART state machine */
	UCA0CTL1 &= ~UCSWRST;

	/* Enable interrupt on UART channel 1 receive */
	IE2 |= UCA0RXIE;
}

/******************************************************************************
* Send byte over UART
******************************************************************************/
void UART_send_byte(uint8_t byte)
{
	while(!(IFG2 & UCA0TXIFG))
	{
		/* Wait for TX buffer to be ready */
	}

	UCA0TXBUF = byte;
}

/******************************************************************************
* Send array of bytes over UART
******************************************************************************/
void UART_send_array(uint8_t* array, uint32_t array_length)
{
	while (array_length--)
	{
		UART_send_byte(*array);
		array++;
	}
}


/******************************************************************************
* ISR for USCI A0 Receive
******************************************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	volatile uint8_t data = UCA0RXBUF;

	printf("%d \n", data);
}
