/*
 * main.c
 *
 *  Created on: Mar 18, 2019
 *      Author: adam
 */
#include "xuartps_hw.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "cmdline.h"

#define SYS_CLK_HZ 250000000
#define MIN_PB_HZ 60
#define MAX_PB_HZ SYS_CLK_HZ

#define CH_0_ADDR_MASK 0x0000FFFF
#define CH_1_ADDR_MASK 0xFFFF0000
#define CH_2_ADDR_MASK 0x0000FFFF
#define CH_3_ADDR_MASK 0xFFFF0000
#define CH_COUNT 4

#define MEM_LENGTH 65536

/*
 * CHANNEL ADDRESSING GPIO DEVICES
 *
 * axi_gpio 0 and axi_gpio_1 have two 32 bit output channels that transmit playback channel addresses to the PL
 *
 * bits [15:0] are playback channel 0 and playback channel 1 addresses
 * bits [31:16] are playback channel 2 and playback channel 3 addresses
 *
 * axi_gpio_0 stores the playback channel write address; this is the address used when
 * "setting" a channels address during write functionality
 *
 * axi_gpio_1 stores the playback channel stop address; it defaults to the memory depth on
 * reset; if set, channel playback will terminate after this address is reached. addresses are
 * 0 index, so settings a stop address of 1024 stops at address 1023 (for example)
 *
 *CHANNEL PLAYBACK GPIO DEVICES
 *
 *axi_gpio_3
 *  ch1: 4 bit output for playback channel mode (level signal)
 *  ch2:  4 bit output for playback channel enable (level signal)
 *
 *
 *  axi_gpio_4
 **ch1: 4 bit output for playback channel din (level signal)
 *  ch2:  4 bit output for playback channel write stop address (oneshot)
 *
 *  axi_gpio_5
 *  ch1: 4 bit output for playback channel write address (oneshot)
 *  ch2:  4 bit output for playback channel write din to ram(oneshot)
 ***********************************************************************************
 *  PLAYBACK CLOCK GPIO DEVICES
 *
 *  axi_gpio_2
 *  ch 1: 32 bit input of readback address of playback channel 0 (lower 16 bits), channel 1(upper 16 bits)
 *  ch 2: 32 bit output for setting  playback clock frequency
 *
 *  axi_gpio_6
 *  ch1: 4 bit output for playback channel clock enable
 *  ch2:  4 bit output for playback channel clock write frequency (oneshot)
 *
 *
 * *  axi_gpio_7
 *  ch1:  *  ch 1: 32 bit input of readback address of playback channel 2 (lower 16 bits), channel 3(upper 16 bits)
 *  ch2:  4 bit output for enabling loop playback (ie playback doesn't stop at stop address, address wraps to 0)
 *
 *  */
#include "gpio.h"
XGpio Gpio_0, Gpio_1, Gpio_2, Gpio_3, Gpio_4, Gpio_5, Gpio_6, Gpio_7;

void init_gpio();
char input_buffer[256];

void set_addr(u8 ch_num, u16 address);
void clear_input_buffer();
void ch_on();
void ch_off();
u16 get_ch_addr(u8 ch_num);

void set_freq(u8 ch_num, u32 frequency);
void write_din(u8 ch_num, u16 address, u8 din);
void set_stop_addr(u8 ch_num, u16 address);

void play(u8);
void stop();
void load(u8, u8, u32);

u32 ch_frequencies[4] = {2000, 2000, 2000, 2000};
u32 stop_addr[4] 	  = {MEM_LENGTH-1, MEM_LENGTH-1, MEM_LENGTH-1, MEM_LENGTH-1};
u8  ch_repeat_mask    = 0b0000;

void init_gpio() {

	XGpio_Initialize(&Gpio_0, PB_CH_0_1_ADDR_GPIO_ID);
	XGpio_Initialize(&Gpio_1, PB_CH_2_3_ADDR_GPIO_ID);
	XGpio_Initialize(&Gpio_2, PB_CH_0_1_ADDR_READBACK_HP_GPIO_ID);
	XGpio_Initialize(&Gpio_3, PB_CH_MODE_EN_GPIO_ID);
	XGpio_Initialize(&Gpio_4, PB_CH_DIN_WRITE_STOP_ADDR_GPIO_ID);
	XGpio_Initialize(&Gpio_5, PB_CH_WRITE_ADDR_WRITE_DIN_GPIO_ID);
	XGpio_Initialize(&Gpio_6, PB_CH_CLK_CTRL_GPIO_ID);
	XGpio_Initialize(&Gpio_7, PB_CH_2_3_ADDR_READBACK_WRITE_HP_GPIO_ID);

	XGpio_SetDataDirection(&Gpio_0, PB_ADDR_0_1_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_0, PB_STOP_ADDR_0_1, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_1, PB_ADDR_2_3_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_1, PB_STOP_ADDR_2_3, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_2, PB_CH_ADDR_READBACK_GPIO_CH, GPIO_AS_INPUT);
	XGpio_SetDataDirection(&Gpio_2, PB_CH_HP_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_3, PB_CH_MODE_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_3, PB_CH_EN_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_4, PB_CH_DIN_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_5, PB_CH_WRITE_DIN_GPIO_CH, GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_6, PB_CH_0_1_ADDR_READBACK_CH, GPIO_AS_INPUT);
	XGpio_SetDataDirection(&Gpio_6, LOOP_PB_GPIO,GPIO_AS_OUTPUT);
	XGpio_SetDataDirection(&Gpio_7, PB_CH_2_3_ADDR_READBACK_CH, GPIO_AS_INPUT);

	XGpio_SetDataDirection(&Gpio_7, WRITE_HP_CH, GPIO_AS_OUTPUT);

	//double checked above

	//TODO : check functions
}

/*
void write_din(u8 ch_num, u16 address, u8 din) {
	//set mode to write
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_MODE_GPIO_CH, 1);

	set_addr(ch_num, address);
	XGpio_DiscreteWrite(&Gpio_4, PB_CH_DIN_GPIO_CH, din);
	XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_DIN_GPIO_CH, 1);
	XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_DIN_GPIO_CH, 0);

	//set mode back to read
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_MODE_GPIO_CH, 0);
}
*/

void set_stop_addr(u8 ch_num, u16 address) {
//	u16 addr0 = 1023;
//	u16 addr1 = 23113;
//	u16 addr2 = 8221;
//	u16 addr3 = 901;
	static u32 ch_0_1 = 0;
	static u32 ch_2_3 = 0;
//	ch_0_1 = 0;
//	ch_2_3 = 0;
	// xil_printf("ch %d stop address is %d\n", ch_num, get_ch_addr(ch_num));
	// xil_printf("setting ch %d stop address to %d\n", ch_num, address);

	stop_addr[ch_num] = address;

	switch (ch_num) {
		case 0:
			ch_0_1 = (ch_0_1 & CH_1_ADDR_MASK) | address;
			XGpio_DiscreteWrite(&Gpio_0, PB_STOP_ADDR_0_1, ch_0_1);
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0001); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0000); //write addr oneshot off
			break;
		case 1:
			ch_0_1 = (address << 16) | (ch_0_1 & CH_0_ADDR_MASK);
			XGpio_DiscreteWrite(&Gpio_0, PB_STOP_ADDR_0_1, ch_0_1);
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0010); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0000); //write addr oneshot off
			break;
		case 2:
			ch_2_3 = (ch_2_3 & CH_3_ADDR_MASK) | address;
			XGpio_DiscreteWrite(&Gpio_1, PB_STOP_ADDR_2_3, ch_2_3);
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0100); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0000); //write addr oneshot off
			break;
		case 3:
			ch_2_3 = (address << 16) | (ch_2_3 & CH_2_ADDR_MASK);
			XGpio_DiscreteWrite(&Gpio_1, PB_STOP_ADDR_2_3, ch_2_3);
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b1000); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_4, PB_CH_WRITE_STOP_ADDR_CH, 0b0000); //write addr oneshot off
			break;
	}

	xil_printf("ch_0_1_stop: %x\n", ch_0_1);
	xil_printf("ch_2_3_stop: %x\n", ch_2_3);

}

void set_addr(u8 ch_num, u16 address) {

	static u32 ch_0_1;
	static u32 ch_2_3;
	ch_0_1 = XGpio_DiscreteRead(&Gpio_6, PB_CH_0_1_ADDR_READBACK_CH);
	ch_2_3 = XGpio_DiscreteRead(&Gpio_7, PB_CH_2_3_ADDR_READBACK_CH);

//	ch_0_1 = 0;
//	ch_2_3 = 0;
	//xil_printf("ch %d address is %d\n", ch_num, get_ch_addr(ch_num));
	//xil_printf("setting ch %d address to %d\n", ch_num, address);
	switch (ch_num) {
		case 0:
			ch_0_1 = (ch_0_1 & CH_1_ADDR_MASK) | address;
			XGpio_DiscreteWrite(&Gpio_0, PB_ADDR_0_1_GPIO_CH, ch_0_1);
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0001); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0000); //write addr oneshot off
			break;
		case 1:
			ch_0_1 = (ch_0_1 & CH_0_ADDR_MASK) |  (address << 16);
			XGpio_DiscreteWrite(&Gpio_0, PB_ADDR_0_1_GPIO_CH, ch_0_1);
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0010); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0000); //write addr oneshot off
			break;
		case 2:
			ch_2_3 = (ch_2_3 & CH_3_ADDR_MASK) | address;
			XGpio_DiscreteWrite(&Gpio_1, PB_ADDR_2_3_GPIO_CH, ch_2_3);
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0100); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0000); //write addr oneshot off
			break;
		case 3:
			ch_2_3 = (ch_2_3 & CH_2_ADDR_MASK) | (address << 16);
			XGpio_DiscreteWrite(&Gpio_1, PB_ADDR_2_3_GPIO_CH, ch_2_3);
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b1000); //write addr oneshot on
			XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_ADDR_GPIO_CH, 0b0000); //write addr oneshot off
			break;
	}

	//xil_printf("ch %d address set to %d\n", ch_num, get_ch_addr(ch_num));
}

/*
void ch_on() {

	//turns all on for now
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_EN_GPIO_CH, 0b1111);
	xil_printf("ch on \n");
}

void ch_off() {
//turns all off for now
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_EN_GPIO_CH, 0b0000); //playback en = 0
	xil_printf("ch off \n");
}
*/

u16 get_ch_addr(u8 ch_num) {
	static u32 ch_0_1 = 0;
	static u32 ch_2_3 = 0;

	if (ch_num < 0 || ch_num > 3) {
		xil_printf("ERROR: ch %d invalid. enter 0,1,2,3\n", ch_num);

	} else {
		ch_0_1 = XGpio_DiscreteRead(&Gpio_2, PB_CH_0_1_ADDR_READBACK_CH);
		ch_2_3 = XGpio_DiscreteRead(&Gpio_7, PB_CH_2_3_ADDR_READBACK_CH);

		switch (ch_num) {
		case 0:
			return ch_0_1 & CH_0_ADDR_MASK;
		case 1:
			return (ch_0_1 & CH_1_ADDR_MASK) >> 16;
		case 2:
			return ch_2_3 & CH_2_ADDR_MASK;
		case 3:
			return (ch_2_3 & CH_3_ADDR_MASK) >> 16;
		default:
			xil_printf("ERROR: ch %d invalid. enter 0,1,2,3\n", ch_num);
		}
	}
	return -1;
}

void set_freq(u8 ch_num, u32 frequency) {

	/* period bus is 22 bits which allows minimum frequency of 30Hz*/

	if (frequency < MIN_PB_HZ || frequency > MAX_PB_HZ) {
		xil_printf(
				"ERROR: %d Hz out of range. Frequency must be between %d and %d. Ignoring command.\n",
				frequency, MIN_PB_HZ, MAX_PB_HZ);
	} else {
		// Period is the duration a single sample value is held on the output
		ch_frequencies[ch_num] = frequency;
		xil_printf("Using period duration: %d\n", SYS_CLK_HZ / frequency);

		XGpio_DiscreteWrite(&Gpio_2, PB_CH_HP_GPIO_CH, SYS_CLK_HZ / frequency);

		XGpio_DiscreteWrite(&Gpio_7, WRITE_HP_CH, 1 << ch_num);
		XGpio_DiscreteWrite(&Gpio_7, WRITE_HP_CH, 0);
	}

}

void load(u8 ch_mask, u8 loop_mask, u32 length) {
	char c;

	XGpio_DiscreteWrite(&Gpio_6, LOOP_PB_GPIO, loop_mask);
	ch_repeat_mask = loop_mask;

	// Set write mode
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_MODE_GPIO_CH, ch_mask);

	for (u32 i=0; i<length; ++i) {
		c = XUartPs_RecvByte(STDIN_BASEADDRESS);

		// TODO: Optimize this set address to use simultaneous capabilities
		for (u8 ch=0; ch<CH_COUNT; ++ch)
		{
			if ((ch_mask >> ch) & 1){
				set_addr(ch, i);
			}
		}

		XGpio_DiscreteWrite(&Gpio_4, PB_CH_DIN_GPIO_CH, (c & ch_mask));
		XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_DIN_GPIO_CH, ch_mask);
		XGpio_DiscreteWrite(&Gpio_5, PB_CH_WRITE_DIN_GPIO_CH, 0);
	}

	//set mode back to read
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_MODE_GPIO_CH, 0);

}

void play(u8 ch_mask)
{
	// Reset address to zero and find which channel is the "stop channel"
	u8 stop_ch = 0;
	u64 min_duration = -1;

	for (u8 ch=0; ch<CH_COUNT; ++ch)
	{
		if ((ch_mask >> ch) & 1)
		{
			set_addr(ch, 0);
			xil_printf("ch %d address set to %d\n", ch, get_ch_addr(ch));

			if (((ch_repeat_mask >> ch) & 1) == 0)  // Don't trigger on repeat signal
			{
				u64 duration = stop_addr[ch] * (SYS_CLK_HZ / ch_frequencies[ch]);
				xil_printf("ch=%d Not repeat, duration=%d\n", ch, duration);
				if (duration < min_duration)
				{
					xil_printf("ch=%d Is min duration\n", ch);
					min_duration = duration;
					stop_ch = ch;
				}
			}
		}
	}

	// Start playback
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_EN_GPIO_CH, ch_mask);

	// Wait for stop channel to reach stop address
	xil_printf("Waiting for addr=%d on ch=%d\n", stop_addr[stop_ch], stop_ch);


	while (get_ch_addr(stop_ch) < stop_addr[stop_ch]);

	/*
	u32 last_seen_addr;
	u32 curr_addr;

	do {
		curr_addr = get_ch_addr(stop_ch);
		if (last_seen_addr != curr_addr)
		{
			last_seen_addr = curr_addr;
			xil_printf("CA: %d\n", last_seen_addr);
		}
	}
	while (last_seen_addr < stop_addr[stop_ch]);
	xil_printf("Done.\n");
	*/

	XGpio_DiscreteWrite(&Gpio_3, PB_CH_EN_GPIO_CH, 0b0000);
}

void stop()
{
	XGpio_DiscreteWrite(&Gpio_3, PB_CH_EN_GPIO_CH, 0b0000);
}

int main(void) {

	init_gpio();
//	XGpio_DiscreteWrite(&Gpio_7,WRITE_HP_CH,0b1111);
//	XGpio_DiscreteWrite(&Gpio_7,LOOP_PB_GPIO_CH,0b0000);
	char a;

	//set_stop_addr(0,232);
	u8 buffer_index = 0;
	while (1) {
		a = XUartPs_RecvByte(STDIN_BASEADDRESS);
		if (a == '\n') {
			CmdLineProcess(input_buffer);
			xil_printf("OK\n");

			buffer_index = 0;
			clear_input_buffer();

		} else {
			input_buffer[buffer_index] = a;
			buffer_index++;
		}
	}

	return 0;
}
void clear_input_buffer(void) {
	u16 i = 0;

	for (i = 0; i < 256; i++) {
		input_buffer[i] = 0;
	}
}
