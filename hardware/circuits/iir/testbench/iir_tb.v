// Modified by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu

`timescale 1ns/1ps
module iir_tb;

	reg 	[7:0]	datain;
	reg 			clk,rst;
	wire 	[16:0]	dataout;
	
	reg 	[7:0]  	items [0:8];
	integer f_in, f_out;
	integer i;

	initial begin
	    clk   = 0;
	    rst   = 0;
	    f_in  = $fopen("../inputs/in.txt", "r");
	    f_out = $fopen("../outputs/out.txt", "w");

	    $monitor("%d(%d), %d(%d), %d(%d), %d", u_iir.ff_mul0, u_iir.samples_in[0], 
	    									   u_iir.ff_mul1, u_iir.samples_in[1],
	    									    u_iir.ff_mul2, u_iir.samples_in[2],
	    									    u_iir.data_feedforward);

	    for(i = 0 ; i < 9; i = i + 1)
	    begin
	    	items[i] = 0;
	    end

		#20 rst = 1;
		#20 rst = 0;

		for(i = 0; i < 9; i = i + 1)
		begin
			$fwrite(f_out, "%d ", items[i]);
		end

		$fscanf(f_in, "%d\n", datain);
		items[0] = datain;
		#10;

		repeat(99) begin
			$fscanf(f_in, "%d\n", datain);

			#10;
			$fwrite(f_out, "%d %d %d\n", dataout, u_iir.data_feedforward, u_iir.data_feedback);

			for(i = 0; i < 9; i = i + 1)
			begin
				$fwrite(f_out, "%d ", items[i]);
			end

			for(i = 1; i < 9; i = i + 1)
			begin
				items[i] <= items[i-1];
			end
			items[0] = datain;
       	end
		#10;
		$fwrite(f_out, "%d %d %d\n", dataout, u_iir.data_feedforward, u_iir.data_feedback);
		$finish;
	end	

	always
   	begin
   	    #5  clk <= ~clk;
   	end

	iir 	u_iir(clk, rst, datain, dataout);

endmodule
