// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014

`timescale 1ns/1ps

module kogge_stone_32b_tb;
wire [32:0] s;
reg [31:0] x;
reg [31:0] y;
reg [32:0] out;
reg clk, rst;
integer count = 1;
integer f;

initial begin
  	f = $fopen("../outputs/kogge_stone_32b.out","w");

  	rst=1'b1;
  	clk=1'b1;
  	#20 rst=1'b0;

	$fwrite(f, "iterations\t\t\tInput1\tInput2\tSum\tSum_Corr\n");
	repeat(100000) begin
		x = $random;
		y = $random;
		out=x+y;
		#21;
		$fwrite(f, "%d\t\t\t%d\t%d\t%d\t%d\n", count, x, y, s, out);
		count = count+1;
	end
	#10 $finish;
end

always
begin
	#5 clk = ~clk;
end
	
	kogge_stone_32b U0(s,x,y, clk, rst);

endmodule
