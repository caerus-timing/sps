`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//Adam Satar
//winter 1819
//ece team17
//parameterized ram module with output enable
//////////////////////////////////////////////////////////////////////////////////

module ram #(parameter DATA_WIDTH = 1, DATA_DEPTH = 65536,  
MEM_INIT_FILE =  "/home/adam/repos/sig_replay/mem_init_files/65k_10.txt", 
ADDR_BITS = 16)
(
        input s_axi_clk, 

                        wen,
                        write_oneshot,

                         [DATA_WIDTH - 1'b1:0] din,
        [ADDR_BITS -  1'b1 : 0] addr,
        output dout
    );
reg [DATA_WIDTH - 1'b1:0] ram [0:DATA_DEPTH - 1'b1];

   
initial begin
  if (MEM_INIT_FILE != "") begin
    $readmemh(MEM_INIT_FILE, ram);
  end
end
assign dout = (wen == 1'b0) ? ram[addr] : 1'bZ;


always @(posedge s_axi_clk) begin
    if(wen == 1'b1 && write_oneshot == 1'b1)
       
            ram[addr] <= din;
        else 
            ram[addr] <= ram[addr];
end


endmodule
