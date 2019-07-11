`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////


module four_ch #(parameter N_ADDR_BITS = 16, 
                 DATA_WIDTH = 1,
                 MEM_DEPTH = 65536,
                 MEM_INIT_FILE =  "/home/adam/repos/sig_replay/mem_init_files/65k_10.txt",
                 DEFAULT_HALF_PERIOD = 22'd125000)
                

(      input s_axi_clk,s_axi_reset,
        input [2*N_ADDR_BITS - 1 : 0] ch_0_addrs,ch_1_addrs,ch_2_addrs,ch_3_addrs,
        input [7:0] ch_0_ctrl,ch_1_ctrl,ch_2_ctrl,ch_3_ctrl,
        //input pb_clk_0,pb_clk_1,pb_clk_2,pb_clk_3,
        input [21:0] i_gpio_half_period,
        output o_ch_0_gpio_playback_done,ch_0_out,
                     o_ch_1_gpio_playback_done,ch_1_out,
                     o_ch_2_gpio_playback_done,ch_2_out,
                     o_ch_3_gpio_playback_done,ch_3_out

);
    
    ch_unit #(.DATA_WIDTH(DATA_WIDTH),.N_ADDR_BITS(N_ADDR_BITS),.MEM_DEPTH(MEM_DEPTH),.MEM_INIT_FILE(MEM_INIT_FILE),.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
            ch_0_unit 
                    (
                    .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),//.playback_clk(pb_clk_0),
                    .ch_addrs(ch_0_addrs),.ch_ctrl(ch_0_ctrl),.i_gpio_half_period(i_gpio_half_period),
                    .o_gpio_playback_done(o_ch_0_gpio_playback_done),.ch_out(ch_0_out)
                    );
                    
    ch_unit #(.DATA_WIDTH(DATA_WIDTH),.N_ADDR_BITS(N_ADDR_BITS),.MEM_DEPTH(MEM_DEPTH),.MEM_INIT_FILE(MEM_INIT_FILE),.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
           ch_1_unit 
                    (
                   .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),//.playback_clk(pb_clk_1),
                   .ch_addrs(ch_1_addrs),.ch_ctrl(ch_1_ctrl),.i_gpio_half_period(i_gpio_half_period),
                   .o_gpio_playback_done(o_ch_1_gpio_playback_done),.ch_out(ch_1_out)
                   ); 

    ch_unit #(.DATA_WIDTH(DATA_WIDTH),.N_ADDR_BITS(N_ADDR_BITS),.MEM_DEPTH(MEM_DEPTH),.MEM_INIT_FILE(MEM_INIT_FILE),.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
           ch_2_unit 
                    (
                   .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),//.playback_clk(pb_clk_2),
                   .ch_addrs(ch_2_addrs),.ch_ctrl(ch_2_ctrl),.i_gpio_half_period(i_gpio_half_period),
                   .o_gpio_playback_done(o_ch_2_gpio_playback_done),.ch_out(ch_2_out)
                   ); 
  
      ch_unit #(.DATA_WIDTH(DATA_WIDTH),.N_ADDR_BITS(N_ADDR_BITS),.MEM_DEPTH(MEM_DEPTH),.MEM_INIT_FILE(MEM_INIT_FILE),.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
            ch_3_unit 
                    (
                    .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),//.playback_clk(pb_clk_3),
                    .ch_addrs(ch_3_addrs),.ch_ctrl(ch_3_ctrl),.i_gpio_half_period(i_gpio_half_period),
                    .o_gpio_playback_done(o_ch_3_gpio_playback_done),.ch_out(ch_3_out)
                    ); 
 
                    
    

endmodule
