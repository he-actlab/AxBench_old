// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu

`timescale 1ns/1ps

module exp_lut_tb();

	reg [31:0] d_in;
	wire [31:0] d_out;

	integer input_file;
	integer output_file;

	initial
	begin
		input_file = $fopen("../inputs/data_in.txt", "r");
		output_file = $fopen("../outputs/data_out.txt", "w");
		repeat(10)
		begin
			$fscanf(input_file, "%b\n", d_in);
			#10;
			$fwrite(output_file, "%b\t%b\n", d_in, d_out);
		end
		#10;
		$fclose(output_file);
		$finish;
	end

	exp_lut u_exp_lut (d_in, d_out);

endmodule
