`timescale 1ns/1ps

`define BIT_WIDTH 32
`define FRACTIONS 15


module forwardk_tb;

	wire  [`BIT_WIDTH-1:0] x, y;
	reg   [`BIT_WIDTH-1:0] theta1, theta2;
	reg   clock,rst;

	integer theta1_file;
	integer theta2_file;
	integer out_file;

	initial begin
		theta1_file = $fopen("../inputs/theta1_input.txt", "r");
		theta2_file = $fopen("../inputs/theta2_input.txt", "r");
		out_file 	= $fopen("../outputs/output.txt", "w");
		clock = 1'b0;
		rst = 1'b1;
		#100 rst = 1'b0;

		repeat(100)
		begin	
			$fscanf(theta1_file, "%b\n", theta1);
			$fscanf(theta2_file, "%b\n", theta2);
			#2200;
			$fwrite(out_file, "%b %b %b %b\n", theta1, theta2, x, y);
		end

	#10 $finish;

	end

	always begin
		#5 clock <= ~clock;
	end

	forwardk X1 (clock, rst, theta1, theta2, x, y);

	
endmodule