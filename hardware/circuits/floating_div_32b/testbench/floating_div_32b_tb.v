// Developed by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu
`timescale 1ns / 1ps
module floating_div_32b_tb();

	reg clk, rst, start;
	reg [31:0] 	a, b;
	wire [31:0] c;
	wire ov, complete;

	integer a_file;
	integer b_file;
	integer c_file;

	initial
	begin
		clk <= 1'b0;
		rst <= 1'b1;

		a_file = $fopen("../inputs/a_input.txt", "r");
		b_file = $fopen("../inputs/b_input.txt", "r");
		c_file = $fopen("../outputs/c_output.txt", "w");

		#20;
		rst <= 1'b0;
		start <= 1'b1;

		repeat(10)
		begin
			$fscanf(a_file, "%b", a);
			$fscanf(b_file, "%b", b);
			#800;
			$fwrite(c_file, "%b %b %b\n", a, b, c);
		end
		#10 $finish;

	end

	always
	begin
		#5 clk <= ~clk;
	end

	floating_div_32b u0(a, b, start, clk, rst, c, complete, ov);

endmodule