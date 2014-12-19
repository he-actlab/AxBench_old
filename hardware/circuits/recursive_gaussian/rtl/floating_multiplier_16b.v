// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014

`timescale 1ns / 1ps

module floating_multiplier_16b #(parameter Q=12, N=16) (i_multiplicand, i_multiplier, o_result, ovr);
	input			[N-1:0]	i_multiplicand;
	input			[N-1:0]	i_multiplier;
	output			[N-1:0]	o_result;
	output			reg		ovr;
	//	Both fixed-point values are of the same length (N,Q)
	
	reg [2*N-1:0]	r_result;		//	Multiplication by 2 values of N bits requires a 
	wire [2*N-1:0]  r_result_tmp;
	reg [N-1:0]		r_RetVal;
	
	// Only handing back the same number of bits as we received 
	// with fixed point in the same location...
	assign o_result = r_RetVal;
	
	always @(r_result_tmp)	
	begin
		r_result <= r_result_tmp;
	end

	always @(r_result) 
	begin
		// update the sign bits
		r_RetVal[N-1] 	<= i_multiplicand[N-1] ^ i_multiplier[N-1];
		r_RetVal[N-2:0] <= r_result[N-2+Q:Q];

		// check for overflow
		if (r_result[2*N-2:N-1+Q] > 0)
			ovr <= 1'b1;
		else
			ovr <= 1'b0;
	end

	// actual multiplication happens here 
	assign r_result_tmp = {1'b0,i_multiplicand[N-2:0]} * {1'b0,i_multiplier[N-2:0]};

endmodule
