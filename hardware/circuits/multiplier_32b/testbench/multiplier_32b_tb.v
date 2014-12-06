// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014

`timescale 1ns/1ps

module multiplier_32b_tb;
wire [63:0] s;
reg [31:0] x;
reg [31:0] y;
reg [63:0] out;
reg clk, rst;
integer count = 1;
integer f;

initial begin
  	f = $fopen("../outputs/multiplier_32b.out","w");

  	rst=1'b1;
  	clk=1'b1;
  	#20 rst=1'b0;

	$fwrite(f, "iterations\t\t\tInput1\tInput2\tResult\tResult_Corr\n");
	repeat(100000) begin
		x = $random;
		y = $random;
		out=$signed(x)*$signed(y);
		#31;
		$fwrite(f, "%d\t\t\t%d\t%d\t%d\t%d\n", count, x, y, s, out);
		count = count+1;
	end
	#10 $finish;
end

always
begin
	#5 clk = ~clk;
end
	
	multiplier_32b U0(s,x,y, clk, rst);

endmodule
