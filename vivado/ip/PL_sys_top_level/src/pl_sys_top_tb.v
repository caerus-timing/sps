`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//top level tb for testing all PL modules
//simulated inputs from PS gpios
//outputs are channel outputs
//////////////////////////////////////////////////////////////////////////////////


module pl_sys_top_tb();


parameter    N_ADDR_BITS = 16, 
                         DATA_WIDTH = 1,
                         MEM_DEPTH = 65536,
                         MEM_INIT_FILE =  "/home/adam/repos/sig_replay/mem_init_files/65k_10.txt",
                         DEFAULT_HALF_PERIOD = 22'd1;

reg s_axi_clk = 0,s_axi_reset = 0;
reg [31:0] ch_0_1_write_addr = 0,ch_2_3_write_addr = 0,
                              ch_0_1_stop_addr = 0, ch_2_3_stop_addr = 0;
reg [3:0] mode = 0,playback_en = 0,din = 0,write_addr = 0,
                             write_stop_addr = 0,write_ram = 0,write_half_period = 0,
                             loop_pb = 0;
reg [21:0] i_gpio_half_period = 0;             

wire[3:0] pb_done_bus;
wire ch_0,ch_1,ch_2,ch_3;

pl_sys_top #(.N_ADDR_BITS(N_ADDR_BITS),
                         .DATA_WIDTH(DATA_WIDTH),
                           .MEM_DEPTH(MEM_DEPTH),
                           .MEM_INIT_FILE(MEM_INIT_FILE),
                           .DEFAULT_HALF_PERIOD(DEFAULT_HALF_PERIOD)
                         )

uut (
 .s_axi_clk(s_axi_clk),.s_axi_reset(s_axi_reset),
 .ch_0_1_write_addr(ch_0_1_write_addr),.ch_2_3_write_addr(ch_2_3_write_addr),
                 .ch_0_1_stop_addr(ch_0_1_stop_addr),.ch_2_3_stop_addr(ch_2_3_stop_addr),
                 .i_gpio_half_period(i_gpio_half_period),
                 .mode(mode),.playback_en(playback_en),.din(din),.write_addr(write_addr),
                 .write_stop_addr(write_stop_addr),.write_ram(write_ram),.write_half_period(write_half_period),
                 .loop_pb(loop_pb),.pb_done_bus(pb_done_bus),.ch_0(ch_0),
                 .ch_1(ch_1),.ch_2(ch_2),.ch_3(ch_3)
);

initial fork
    #3 s_axi_reset = 1;
    #10 playback_en = 4'b1111;
    #60 i_gpio_half_period = 22'd3;
    #65 write_half_period = 4'b1110;
    #70 write_half_period = 4'b0000;
    #150 playback_en = 4'b0000;
    #160 ch_0_1_stop_addr = 32'hFFF1111F;
    #160 ch_0_1_write_addr = 32'h0000FF00;
    #165 write_stop_addr = 4'b0011;
    #170 write_stop_addr = 4'b0000;
    #175 write_addr = 4'b0011;
    #180 write_addr = 4'b0000;
    #300 $stop;
join

always #1 s_axi_clk = ~s_axi_clk;

endmodule
