/*
 * command_io.c
 *
 * Relies on cmdline.c and cmdline.h provided by T.I. developer
 * API.
 *
 * Contains functions supported terminal i/o for streamlining
 * development. These functions are only for parsing/handling
 * command line input relative to various functions.
 *
 *  	Created on: 1.20.19
 *      Author: Adam Satar
 */

#include "cmdline.h"
#include "command_io.h"
long cmd_val; /*Value passed in on the command line.*/
long cmd_val_2;
long cmd_val_3;

//Table of command string, function, and help string.

tCmdLineEntry g_sCmdTable[] = {
		{ "help", cmd_help, " : command arg1 arg2 arg3 " },
		{ "set_addr", cmd_set_addr," : set_addr ch_num address" },
		{ "set_stop_addr", cmd_set_stop_addr," : set_stop_addr ch_num address" },
		{ "get_ch_addr", cmd_get_ch_addr," : get_ch_addr ch_num" },

		/*
		{ "write_din", cmd_write_din, " : write_din din address" },
		{ "ch_on", cmd_pb_ch_on, " : ch_on" },
		{ "ch_off", cmd_pb_ch_off," : ch_off" },
		*/

		{ "set_freq", cmd_set_freq,	" : (set_freq ch_num freq) ; sets the freq (in Hz) of ch_num" },
		{ "play", cmd_play,	" : (play ch_mask) ; starts playback on the masked channels" },
		{ "stop", cmd_stop,	" : (stop) ; stops playback on all channels" },
		{ "load", cmd_load,	" : (load ch_mask loop_mask sample_count) ; begins loading a raw data stream" },

		{ 0, 0, 0 } //end of table indicator
};

/*
int cmd_write_din(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 10);
	cmd_val_2 = strtol(argv[2], 0, 10);
	write_din(cmd_val, cmd_val_2);

	return 0;
}
*/

int cmd_set_addr(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 10); //ch num
	cmd_val_2 = strtol(argv[2], 0, 10); //addr
	set_addr(cmd_val,cmd_val_2);
	return 0;
}

int cmd_set_freq(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 10); //channel
	cmd_val_2 = strtol(argv[2], 0, 10); //frequency
	set_freq(cmd_val, cmd_val_2);

	return 0;
}


int cmd_get_ch_addr(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 10); //channel
	if (cmd_val < 0 || cmd_val > 3) {
		xil_printf("ch %d invalid. enter 0,1,2,3\n", cmd_val);
	} else {
		xil_printf("channel %d address is %d\n", cmd_val, get_ch_addr(cmd_val));
	}

	return 0;
}

int cmd_play(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 16); // ch_mask
	play(cmd_val);
	return 0;
}

int cmd_stop(int argc, char *argv[]) {
	stop();
	return 0;
}

int cmd_load(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 16); // ch_mask
	cmd_val_2 = strtol(argv[2], 0, 16); // loop_mask
	cmd_val_3 = strtol(argv[3], 0, 10); // ch_mask

	load(cmd_val, cmd_val_2, cmd_val_3);
	return 0;
}

int cmd_set_stop_addr(int argc, char *argv[]) {
	cmd_val = strtol(argv[1], 0, 10); // ch_num
	cmd_val_2 = strtol(argv[2], 0, 10); // stop_addr

	set_stop_addr(cmd_val, cmd_val_2);
	return 0;
}

/*
int cmd_pb_ch_on(int argc, char *argv[]) {
	ch_on();
	return 0;
}

int cmd_pb_ch_off(int argc, char *argv[]) {
	ch_off();
	return 0;
}
*/

int cmd_help(int argc, char *argv[]) {
	tCmdLineEntry *pEntry;

	// Print some header text.
	xil_printf("\nAvailable commands\n");
	xil_printf("------------------\n");

	//
	// Point at the beginning of the command table.
	//
	pEntry = &g_sCmdTable[0];

	//
	// Enter a loop to read each entry from the command table.  The end of the
	// table has been reached when the command name is NULL.
	//
	while (pEntry->pcCmd) {
		//
		// Print the command name and the brief description.
		//
		xil_printf("%s%s\n", pEntry->pcCmd, pEntry->pcHelp);

		//
		// Advance to the next entry in the table.
		//
		pEntry++;
	}

	//
	// Return success.
	//
	return (0);
}
