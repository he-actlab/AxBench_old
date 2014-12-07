`timescale 1ns/1ps

`define BIT_WIDTH 32
`define FRACTIONS 15


module inversek_tb;

	reg [`BIT_WIDTH-1:0] x,y;
	wire [`BIT_WIDTH-1:0] theta1,theta2;
	reg clock,rst;

	integer x_file;
	integer y_file;
	integer theta_file;

	initial begin
		x_file = $fopen("../inputs/x_input.txt", "r");
		y_file = $fopen("../inputs/y_input.txt", "r");
		theta_file = $fopen("../outputs/theta_output.txt", "w");
		clock = 1'b0;
		rst = 1'b1;
		#100 rst = 1'b0;

		repeat(10)
		begin	
			$fscanf(x_file, "%b\n", x);
			$fscanf(y_file, "%b\n", y);
			#2200;
			$fwrite(theta_file, "%b %b %b %b\n", x, y, theta1, theta2);
		end

	#10 $finish;

	end

	always begin
		#5 clock <= ~clock;
	end

	inversek X1 (clock, rst, x, y, theta1, theta2);

	
endmodule