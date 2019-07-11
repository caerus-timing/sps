`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//purely combinational module that takes inputs from gpios 
//splits and concatenates them for each channel
//see TRM for details
//////////////////////////////////////////////////////////////////////////////////


module four_gpio_ch(
    input [31:0] ch_0_1_write_addr,ch_2_3_write_addr,
                          ch_0_1_stop_addr, ch_2_3_stop_addr,
    input [3:0] mode,playback_en,din,write_addr,write_stop_addr,write_ram,write_half_period,loop_pb,
    output [31:0] ch_0_addrs,ch_1_addrs,ch_2_addrs,ch_3_addrs,
    output [7:0] ch_0_ctrl,ch_1_ctrl,ch_2_ctrl,ch_3_ctrl
    );
    
    
    
    gpio_to_ch ch_0_gpio(.ch_wr_addr(ch_0_1_write_addr [15:0]),.ch_stop_addr(ch_0_1_stop_addr[15:0]),
                                  .mode(mode[0]),.playback_en(playback_en[0]),.din(din[0]),.write_addr(write_addr[0]),
                                  .write_stop_addr(write_stop_addr[0]),.write_ram(write_ram[0]),.write_half_period(write_half_period[0]),.loop_pb(loop_pb[0]),
                                  .ch_addrs(ch_0_addrs),.ch_ctrl(ch_0_ctrl)  
                                    );
                                    
    gpio_to_ch ch_1_gpio(.ch_wr_addr(ch_0_1_write_addr [31:16]),.ch_stop_addr(ch_0_1_stop_addr[31:16]),
                              .mode(mode[1]),.playback_en(playback_en[1]),.din(din[1]),.write_addr(write_addr[1]),
                              .write_stop_addr(write_stop_addr[1]),.write_ram(write_ram[1]),.write_half_period(write_half_period[1]),.loop_pb(loop_pb[1]),
                              .ch_addrs(ch_1_addrs),.ch_ctrl(ch_1_ctrl)  
                                );
    
    gpio_to_ch ch_2_gpio(.ch_wr_addr(ch_2_3_write_addr [15:0]),.ch_stop_addr(ch_2_3_stop_addr[15:0]),
                                  .mode(mode[2]),.playback_en(playback_en[2]),.din(din[2]),.write_addr(write_addr[2]),
                                  .write_stop_addr(write_stop_addr[2]),.write_ram(write_ram[2]),.write_half_period(write_half_period[2]),.loop_pb(loop_pb[2]),
                                  .ch_addrs(ch_2_addrs),.ch_ctrl(ch_2_ctrl)  
                                    );

    gpio_to_ch ch_3_gpio(.ch_wr_addr(ch_2_3_write_addr [31:16]),.ch_stop_addr(ch_2_3_stop_addr[31:16]),
                              .mode(mode[3]),.playback_en(playback_en[3]),.din(din[3]),.write_addr(write_addr[3]),
                              .write_stop_addr(write_stop_addr[3]),.write_ram(write_ram[3]),.write_half_period(write_half_period[3]),.loop_pb(loop_pb[3]),
                              .ch_addrs(ch_3_addrs),.ch_ctrl(ch_3_ctrl)  
                                );

    
endmodule
