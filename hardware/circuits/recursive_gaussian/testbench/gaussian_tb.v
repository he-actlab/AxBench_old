// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014

`timescale 1ns / 1ps

module gaussian_tb;

	reg clk, rst;
	reg [15:0] pixel_in;
	reg [4:0]  pixel_id;

	wire [15:0] pixel_out;

	integer i;

	integer image_in;
	integer image_out;

	initial
	begin
		clk = 0;
		rst = 0;
		pixel_in = 0;
		pixel_id = 0;

		image_in  = $fopen("../inputs/lion_512x512_in.txt", "r");
		image_out = $fopen("../outputs/lion_512x512_out.txt", "w");


		#10;
		rst = 1;
		#10;
		rst = 0;
		#10;

		repeat(262144) // image size = width * height
		begin
			for(i = 0; i<25; i=i+1)
			begin
				$fscanf(image_in, "%b", pixel_in);
				pixel_id = i;
				#10;
			end
			#10;
			$fwrite(image_out, "%b\n", pixel_out);
			#10;
			rst = 1;
			#10;
			rst = 0;
			#10;
		end
		#10;
		$finish;
	end
	
	always
	begin
		#5 clk <= ~clk;
	end

	gaussian u_gaussian (clk, rst, pixel_in, pixel_id, pixel_out);

endmodule