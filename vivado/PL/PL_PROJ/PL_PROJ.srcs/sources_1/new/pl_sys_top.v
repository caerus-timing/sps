`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//top level PL system module 
//inputs are PS GPIOs and its outputs are PMOD pins
//////////////////////////////////////////////////////////////////////////////////


module pl_sys_top#(parameter N_ADDR_BITS = 16, 
                         DATA_WIDTH = 1,
                         MEM_DEPTH = 65536,
                         MEM_INIT_FILE =  "/home/adam/repos/sig_replay/mem_init_files/65k_10.txt",
                         DEFAULT_HALF_PERIOD = 22'd125000)
(       
        input s_axi_clk,s_axi_reset,
        input [31:0] ch_0_1_write_addr,ch_2_3_write_addr,
                              ch_0_1_stop_addr, ch_2_3_stop_addr,
         input [3:0] mode,playback_en,din,write_addr,
                             write_stop_addr,write_ram,write_half_period,
                             loop_pb,
         input [21:0] i_gpio_half_period,                 
         output[3:0] pb_done_bus,
         output ch_0,ch_1,ch_2,ch_3
    );
    
        wire [31:0] ch_0_addrs,ch_1_addrs,ch_2_addrs,ch_3_addrs;
        wire [7:0] ch_0_ctrl,ch_1_ctrl,ch_2_ctrl,ch_3_ctrl;
        
        assign pb_done_bus = {o_ch_3_gpio_playback_done,o_ch_2_gpio_playback_done,o_ch_1_gpio_playback_done,o_ch_0_gpio_playback_done};
        
        four_gpio_ch four_gpio_ch_unit
        (      
                .ch_0_1_write_addr(ch_0_1_write_addr),.ch_2_3_write_addr(ch_2_3_write_addr),
                .ch_0_1_stop_addr(ch_0_1_stop_addr),.ch_2_3_stop_addr(ch_2_3_stop_addr),
                .mode(mode),.playback_en(playback_en),.din(din),.write_addr(write_addr),
                .write_stop_addr(write_stop_addr),.write_ram(write_ram),.write_half_period(write_half_period),
                .loop_pb(loop_pb),.ch_0_addrs(ch_0_addrs),.ch_1_addrs(ch_1_addrs),.ch_2_addrs(ch_2_addrs),
                .ch_3_addrs(ch_3_addrs),.ch_0_ctrl(ch_0_ctrl),.ch_1_ctrl(ch_1_ctrl),.ch_2_ctrl(ch_2_ctrl),.ch_3_ctrl(ch_3_ctrl)
        );
        
        

        
        four_ch #(.N_ADDR_BITS(N_ADDR_BITS),.DATA_WIDTH(DATA_WIDTH),.MEM_DEPTH(MEM_DEPTH),.MEM_INIT_FILE(MEM_INIT_FILE),.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
        
        four_ch_unit
        (       .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),
                .i_gpio_half_period(i_gpio_half_period),
                .ch_0_ctrl(ch_0_ctrl),.ch_1_ctrl(ch_1_ctrl),.ch_2_ctrl(ch_2_ctrl),.ch_3_ctrl(ch_3_ctrl),
                .ch_0_addrs(ch_0_addrs), .ch_1_addrs(ch_1_addrs), .ch_2_addrs(ch_2_addrs), .ch_3_addrs(ch_3_addrs),
                .ch_0_out(ch_0),.o_ch_0_gpio_playback_done(o_ch_0_gpio_playback_done),
                .ch_1_out(ch_1),.o_ch_1_gpio_playback_done(o_ch_1_gpio_playback_done),
                .ch_2_out(ch_2),.o_ch_2_gpio_playback_done(o_ch_2_gpio_playback_done),
                .ch_3_out(ch_3),.o_ch_3_gpio_playback_done(o_ch_3_gpio_playback_done)
        );
        
        
endmodule
