// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014
`timescale 1ns/1ps


module sobel_tb();

	reg [8:0] p0,p1,p2,p3,p5,p6,p7,p8,p4;
	reg clk;
	wire [7:0] out;
	integer in,temp,ot;
	integer count;

	initial begin
		clk = 1'b0;
		count = 0;
		in = $fopen("../inputs/baboon_512x512.txt","r");		//enter input file name containing pixels
		ot = $fopen("../outputs/baboon_512x512.txt","w");	//enter output file name
		#5;
		p0 = 8'b0; 
		p1 = 8'b0; 
		p2 = 8'b0; 
		p3 = 8'b0; 
		p4 = 8'b0; 
		p5 = 8'b0; 
		p6 = 8'b0; 
		p7 = 8'b0;
		#20;
		repeat(262144)
		begin
			temp = $fscanf(in,"%d %d %d %d %d %d %d %d %d\n",p0,p1,p2,p3,p4,p5,p6,p7,p8);
			#5;
			#5;
			count = count +1;
			#6;
			$fwrite(ot,"%d\n",out);
		end
		#100 $finish;
	end

	always
	begin
		#2.5 clk = ~clk;
	end

	sobel U0 (clk,p0,p1,p2,p3,p5,p6,p7,p8,out);

endmodule
