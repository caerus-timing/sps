`timescale 1ns / 1ps


module clk_en_sig  #(parameter DEFAULT_HALF_PERIOD = 22'd125000)(


input s_axi_aclk,s_axi_aresetn,
input [21:0] i_gpio_half_period,
input write_hp,
output reg clk_en_sig

    );
    
    reg [21:0] divisor = 0;
    reg [21:0] count = 0;
pos_oneshot write_hp_os(
     .i_clk(s_axi_aclk),.i_reset(s_axi_aresetn),
                    .input_pulse(write_hp),
            .oneshot(write_os)
        );


    
    always @(posedge s_axi_aclk)
    begin
        if(s_axi_aresetn == 1'b0) 
            begin
                count <= 1'b0;
                clk_en_sig <= 1'b0;
                divisor <=DEFAULT_HALF_PERIOD;
            end
            else if (write_os == 1'b1)
            begin
                divisor <= i_gpio_half_period;
                count <= 1'b0;
            end
            else if(count == divisor)
            begin
            count <= 1'b0;
            clk_en_sig <= 1'b1;
            end
            else begin count <= count + 1'b1; clk_en_sig = 1'b0; end
    end
    
endmodule
