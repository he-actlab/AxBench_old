`timescale 1ns/1ps
module exoor(in1,in2,out);
	input in1, in2;
	output out;

	assign out = in1 ^ in2;
endmodule