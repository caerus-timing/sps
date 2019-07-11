`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//contains clk enable unit which handles playback clock
//contains ram control unit that handles addressing
//contains ram unit for storage
//////////////////////////////////////////////////////////////////////////////////

module ch_unit #(parameter N_ADDR_BITS = 16, 
                 DATA_WIDTH = 1,
                 MEM_DEPTH = 65536,
                 MEM_INIT_FILE =  "/home/adam/repos/sig_replay/mem_init_files/65k_10.txt",
                 DEFAULT_HALF_PERIOD = 22'd1250000)
                 
(
            input s_axi_clk, s_axi_reset,//playback_clk,
            input [2*N_ADDR_BITS - 1 : 0] ch_addrs,
            input [7:0] ch_ctrl,
            input [21:0] i_gpio_half_period,
            
            output o_gpio_playback_done,
            output ch_out

);

 
 

    clk_en_sig #(.DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD))
    pb_clk(
        .s_axi_aclk(s_axi_clk),
        .s_axi_aresetn(s_axi_reset),
        .i_gpio_half_period(i_gpio_half_period),
        .write_hp(ch_ctrl[1]),
        .clk_en_sig(playback_clk)
    );
      wire  [N_ADDR_BITS - 1 : 0] ram_addr;
    
    ram_ctrl #(.N_ADDR_BITS(N_ADDR_BITS),.MEM_DEPTH(MEM_DEPTH))
    ram_ctrl_unit (
      
             .s_axi_clk(s_axi_clk),
             .s_axi_reset(s_axi_reset),
            .playback_clk(playback_clk),
            .set_ram_addr(ch_addrs[15:0]),
            .ram_addr(ram_addr),
            .write_oneshot(write_oneshot), //to ram
            .playback_done(o_gpio_playback_done),
            .wen(wen), //to ram
            .mode(ch_ctrl[7]),
            .ram_write(ch_ctrl[2]),
            .write_stop_addr(ch_ctrl[3]),
            .write_addr(ch_ctrl[4]),
            .stop_addr(ch_addrs[31:16]),
            .playback_en(ch_ctrl[6]),
            .loop_playback(ch_ctrl[0])
      );


ram #(.DATA_WIDTH(DATA_WIDTH), .DATA_DEPTH(MEM_DEPTH),  
            .MEM_INIT_FILE(MEM_INIT_FILE), 
            .ADDR_BITS(N_ADDR_BITS))
ram_unit
 (.s_axi_clk(s_axi_clk),.write_oneshot(write_oneshot),  .wen(wen), .addr(ram_addr), .din(ch_ctrl[5]), .dout(ch_out));
    
endmodule
