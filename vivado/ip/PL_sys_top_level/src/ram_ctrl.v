`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//handles ram addressing
//////////////////////////////////////////////////////////////////////////////////



module ram_ctrl #(parameter N_ADDR_BITS = 16,
                 MEM_DEPTH = 65536)
                (input 		 s_axi_clk,
                 input 		 playback_clk,
                 input 		 s_axi_reset,
                 input playback_en,
                 input write_addr, //level signal from gpio
                 input  ram_write, //level signal that needs to toggle for one shot from gpio
                 input write_stop_addr,
                 input loop_playback,
                 input [N_ADDR_BITS - 1:0] set_ram_addr,
                 output reg [N_ADDR_BITS -1:0] ram_addr,
                 input [N_ADDR_BITS - 1: 0] stop_addr,
                 output 		 write_oneshot,
                 output  	reg	 playback_done,
                 output 	reg	 wen,

                 input mode); //level signal from gpio
          
    localparam READ_MODE = 1'b0, WRITE_MODE = 1'b1;
  
    
    pos_oneshot set_addr_oneshot_unit(
    .i_clk(s_axi_clk),
    .i_reset(s_axi_reset),
    .input_pulse(write_addr),
    .oneshot(write_addr_oneshot)
    );
    
    
    pos_oneshot write_stop_addr_oneshot_unit(
    .i_clk(s_axi_clk),
    .i_reset(s_axi_reset),
    .input_pulse(write_stop_addr),
    .oneshot(write_stop_addr_oneshot)
    );
    
    pos_oneshot write_oneshot_unit(
    .i_clk(s_axi_clk),
    .i_reset(s_axi_reset),
    .input_pulse(ram_write),
    .oneshot(write_oneshot)
    );
    
    reg [N_ADDR_BITS - 1:0] read_stop_addr;

    
    always @(posedge s_axi_clk or negedge s_axi_reset or posedge write_stop_addr_oneshot)
        if(s_axi_reset == 1'b0) read_stop_addr <= MEM_DEPTH - 1;
        else if(write_stop_addr_oneshot == 1'b1) read_stop_addr <= stop_addr;
        else read_stop_addr <= read_stop_addr;

always @(negedge s_axi_reset or posedge write_addr_oneshot or posedge playback_clk) begin    

    begin
    if(s_axi_reset == 1'b0) 
        begin ram_addr <= 1'b0; playback_done <= 1'b0; end
   
   else if(write_addr_oneshot == 1'b1)
     ram_addr <= set_ram_addr;
    
   
   else begin
                    case(mode)
                    
                        WRITE_MODE:  wen<= 1'b1;  
                        
                        READ_MODE: 
                                    begin
                                               wen<= 1'b0;
                                                if( playback_clk == 1'b1 && playback_en == 1'b1)
                                                 begin
                                                    if(ram_addr == read_stop_addr && loop_playback == 1'b0) playback_done <= 1'b1;
                                                    else if (ram_addr == MEM_DEPTH - 1'b1) ram_addr <= 1'b0;
                                                    else ram_addr <= ram_addr + 1'b1;
                                                 end
                                    end
                          default: begin wen <= 1'b0; ram_addr <= ram_addr; end
                    endcase 
    end
    end
    
    end


    
    
endmodule
