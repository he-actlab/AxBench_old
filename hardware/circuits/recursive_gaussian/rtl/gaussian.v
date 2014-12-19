// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014

`timescale 1ns / 1ps

module gaussian (clk, rst, pixel_in, pixel_id, pixel_out);

	parameter bitwidth = 16; 

	input clk;
	input rst;
	input [bitwidth-1:0] pixel_in;
	input [4:0]          pixel_id;

	output [bitwidth-1:0] pixel_out;


	reg [bitwidth-1:0] mul_reg;
	reg [bitwidth-1:0] acc_reg;

	wire [bitwidth-1:0] mul_wire;
	wire [bitwidth-1:0] add_wire;
	wire [bitwidth-1:0] gaussian_entry;

	assign pixel_out = acc_reg;

	always @(posedge clk) begin
		if(rst)
		begin
			mul_reg <= 0;
			acc_reg <= 0;
		end
		else
		begin
			mul_reg <= mul_wire;
			acc_reg <= add_wire;
		end
	end

	gaussian_rom            u_rom (pixel_id, gaussian_entry);
	floating_multiplier_16b u_mlt (pixel_in, gaussian_entry, mul_wire);
	floating_adder_16b      u_add (acc_reg, mul_wire, add_wire);

endmodule
