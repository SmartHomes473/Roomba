CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2553
OBJS=main.o msp430g2553_utils.o software_uart.o hardware_uart.o roomba.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o roomba.elf $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr main.elf $(OBJS)
