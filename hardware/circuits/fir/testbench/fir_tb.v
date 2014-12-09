// Modified by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu

`timescale 1ns/1ps
module fir_tb;

	reg 	[7:0]	x;
	reg 			clk,rst;
	wire 	[9:0]	dataout;
	
	reg 	[7:0]  	items [0:4];
	integer f_in, f_out;
	integer cnt_item;

	initial begin
	    clk=0;
	    rst = 1;
	    f_in  = $fopen("../inputs/in.txt", "r");
	    f_out = $fopen("../outputs/out.txt", "w");

		#20 rst = 0;
		#10;

		cnt_item = 0;
		$fscanf(f_in, "%d\n", x);
		items[cnt_item] = x;
		cnt_item = cnt_item + 1;
		#10;
		$fscanf(f_in, "%d\n", x);
		items[cnt_item] = x;
		cnt_item = cnt_item + 1;
		#10;
		$fscanf(f_in, "%d\n", x);
		items[cnt_item] = x;
		cnt_item = cnt_item + 1;
		#10;
		$fscanf(f_in, "%d\n", x);
		items[cnt_item] = x;
		cnt_item = cnt_item + 1;
		#10;
		$fscanf(f_in, "%d\n", x);
		items[cnt_item] = x;
		cnt_item = cnt_item + 1;
		$fwrite(f_out, "%d %d %d %d %d", items[4], items[3], items[2], items[1], items[0]);
		#10;
		$fscanf(f_in, "%d\n", x);
		items[0] = items[1];
		items[1] = items[2];
		items[2] = items[3];
		items[3] = items[4];
		items[4] = x;
		#10;
		

		repeat(999) begin
			$fscanf(f_in, "%d\n", x);
			$fwrite(f_out, " %d\n", dataout);
			$fwrite(f_out, "%d %d %d %d %d", items[4], items[3], items[2], items[1], items[0]);
			items[0] = items[1];
			items[1] = items[2];
			items[2] = items[3];
			items[3] = items[4];
			items[4] = x;
			#10;
       	end
       	$fwrite(f_out, " %d\n", dataout);
		#10;
		$finish;
	end	

	always
   	begin
   	    #5  clk <= ~clk;
   	end

	fir 	u_fir(clk,rst,x,dataout);
endmodule
