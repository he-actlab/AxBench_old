// Modified by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu


// y[n] = 	x[n] 	+ 2*x[n-1] + 4*x[n-2] + 8*x[n-3] + 16*x[n-4] + 8 *x[n-5] + 4*x[n-6] + 2*x[n-7] +   x[n-8] +
//						y[n-1] + 2*y[n-2] + 4*y[n-3] + 8 *y[n-4] + 16*y[n-5] + 8*y[n-6] + 4*y[n-7] + 2*y[n-8]

`timescale 1ns/1ps
module iir(clk, rst, datain, dataout);//,data_feedback,data_feedforward);


// Filter Parameters
parameter 		order			=	8;
parameter 		word_size_in 	=	8;
parameter 		word_size_out 	= 	2*word_size_in + 1; // 17 bits
parameter 		mul_size 		= 	2*word_size_in; // 16 bits
parameter 		add_size 		= 	2*word_size_in + 1; // 17 bits

// Feedforward Coefficients (max = 16)
parameter b0=8'd1;
parameter b1=8'd2;
parameter b2=8'd4;
parameter b3=8'd8;
parameter b4=8'd16;
parameter b5=8'd8;
parameter b6=8'd4;
parameter b7=8'd2;
parameter b8=8'd1;

// Feedback Coefficients (max = 32)
parameter a1=8'd1;
parameter a2=8'd2;
parameter a3=8'd4;
parameter a4=8'd8;
parameter a5=8'd16;
parameter a6=8'd8;
parameter a7=8'd4;
parameter a8=8'd2;


// Inputs
input 							clk, rst;
input 		[word_size_in-1:0]	datain;

// Ouput
output 		[word_size_out-1:0]	dataout;


// Input/Output Buffer
reg 		[word_size_in-1:0]	samples_in 	[0:order];
reg 		[word_size_in-1:0]	samples_out	[1:order];

// Temporary Registers
reg 		[word_size_in-1:0]	datain_reg;
reg 		[word_size_out-1:0] dataout_reg;

// Signals
wire 		[word_size_out-1:0] data_out;

// Signals for feedforward calculations
wire 		[mul_size-1 :0] 	ff_mul0, ff_mul1, ff_mul2, ff_mul3, ff_mul4;
wire 		[mul_size-1 :0] 	ff_mul5, ff_mul6, ff_mul7, ff_mul8;
wire 		[add_size-1 :0] 	ff_add0, ff_add1, ff_add2, ff_add3, ff_add4;
wire 		[add_size-1 :0] 	ff_add5, ff_add6, ff_add7, ff_add8;

// Signals for feedforward result
wire		[word_size_out-1:0] data_feedforward; // 18 bits

// Signals for feedback calculations
wire 		[mul_size-1 :0] 	fb_mul0, fb_mul1, fb_mul2, fb_mul3, fb_mul4;
wire 		[mul_size-1 :0] 	fb_mul5, fb_mul6, fb_mul7;
wire 		[add_size-1 :0] 	fb_add0, fb_add1, fb_add2, fb_add3, fb_add4;
wire 		[add_size-1 :0] 	fb_add5;

// Signals for feedback result
wire		[word_size_out-1:0] data_feedback; // 18 bits


// variable for loop
integer k;

assign dataout = dataout_reg;


always@(posedge clk) begin
	if(rst)
	begin
		samples_in[0] 	<= 	0;
		dataout_reg	<=	0;
		datain_reg 	<= 	0;

		for(k=1;k<=order;k=k+1)
		begin
			samples_in[k]<=0;
			samples_out[k]<=0;
		end
	end
	else
	begin
		datain_reg 		<= 	datain;
		samples_in[0]	<=	datain_reg;
		samples_in[1]	<=	samples_in[0];
		samples_out[1]	<=	dataout_reg >> 9; // divide by 512, max result will be 255
		dataout_reg 	<= 	data_out;

		for(k=2;k<=order;k=k+1)
		begin
			samples_in[k]  <= samples_in[k-1];
			samples_out[k] <= samples_out[k-1];
		end
	end
end

	// Feedforward Multiplications
	assign ff_mul0  =  b0 * samples_in[0];
	assign ff_mul1  =  b1 * samples_in[1];
	assign ff_mul2  =  b2 * samples_in[2];
	assign ff_mul3  =  b3 * samples_in[3];
	assign ff_mul4  =  b4 * samples_in[4];
	assign ff_mul5  =  b5 * samples_in[5];
	assign ff_mul6  =  b6 * samples_in[6];
	assign ff_mul7  =  b7 * samples_in[7];
	assign ff_mul8  =  b8 * samples_in[8];
	
	// Feedforward Additions
	kogge_stone_16b 	 ff_a1 (ff_add0,  				ff_mul0, 		ff_mul1);
	kogge_stone_16b 	 ff_a2 (ff_add1,  				ff_mul2, 		ff_mul3);
	kogge_stone_16b 	 ff_a3 (ff_add2,  				ff_mul4, 		ff_mul5);
	kogge_stone_16b 	 ff_a4 (ff_add3,  				ff_mul6, 		ff_mul7);
	kogge_stone_16b 	 ff_a5 (ff_add4,  				ff_add0[15:0],	ff_add1[15:0]);
	kogge_stone_16b 	 ff_a6 (ff_add5,  				ff_add2[15:0],	ff_add3[15:0]);
	kogge_stone_16b 	 ff_a7 (ff_add6,  				ff_add4[15:0],	ff_add5[15:0]);
	kogge_stone_16b 	 ff_a8 (data_feedforward[16:0],	ff_add6[15:0],	ff_mul8);
	

	// Feedback Multiplications 
	assign fb_mul0 = a1 * samples_out[1];
	assign fb_mul1 = a2 * samples_out[2];
	assign fb_mul2 = a3 * samples_out[3];
	assign fb_mul3 = a4 * samples_out[4];
	assign fb_mul4 = a5 * samples_out[5];
	assign fb_mul5 = a6 * samples_out[6];
	assign fb_mul6 = a7 * samples_out[7];
	assign fb_mul7 = a8 * samples_out[8];
	
	kogge_stone_16b 	 fb_a0 (fb_add0,  				fb_mul0, 		fb_mul1);
	kogge_stone_16b 	 fb_a1 (fb_add1,  				fb_mul2, 		fb_mul3);
	kogge_stone_16b 	 fb_a2 (fb_add2,  				fb_mul4, 		fb_mul5);
	kogge_stone_16b 	 fb_a3 (fb_add3,  				fb_mul6, 		fb_mul7);
	kogge_stone_16b 	 fb_a4 (fb_add4,  				fb_add0[15:0], 	fb_add1[15:0]);
	kogge_stone_16b 	 fb_a5 (fb_add5,  				fb_add2[15:0], 	fb_add3[15:0]);
	kogge_stone_16b 	 fb_a6 (data_feedback[16:0], 	fb_add4[15:0],	fb_add5[15:0]);


	kogge_stone_16b 	fb_fa_add (data_out[16:0], data_feedforward[15:0], data_feedback[15:0]);


endmodule
