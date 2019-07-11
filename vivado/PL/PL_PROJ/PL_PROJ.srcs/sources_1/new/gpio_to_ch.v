`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//////////////////////////////////////////////////////////////////////////////////


module gpio_to_ch(
    input [15:0] ch_wr_addr, ch_stop_addr,
    input mode, playback_en,din,write_addr,write_stop_addr,write_ram,write_half_period,loop_pb,
    output [31:0] ch_addrs,
    output [7:0] ch_ctrl
    );
    
    assign ch_addrs[15:0] = ch_wr_addr;
    assign ch_addrs[31:16] = ch_stop_addr;
    assign ch_ctrl[7] = mode;
    assign ch_ctrl[6] = playback_en;
    assign ch_ctrl[5] =  din;
    assign ch_ctrl[4] = write_addr;
    assign ch_ctrl[3] = write_stop_addr;
    assign ch_ctrl[2] = write_ram;
    assign ch_ctrl[1] =write_half_period;
    assign ch_ctrl[0] =  loop_pb;
    
endmodule
