// Developed by: 	Amir Yazdanbakhsh
// Email: 			a.yazdanbakhsh@gatech.edu
// Date:			December 5, 2014
`timescale 1ns/1ps


module sobel( clk, p0, p1, p2, p3, p5, p6, p7, p8, out);

	input  [8:0] p0,p1,p2,p3,p5,p6,p7,p8;	
	input clk;

	output [7:0] out;					
	
	wire [8:0] p2_p0, p8_p6, p0_p6, p2_p8, p5_p3, p1_p7, center_rm, center_tm;
	wire [9:0] center_1, center_2, center_3, center_4, gx_temp, gy_temp;
	reg [8:0] p0_reg,p1_reg,p2_reg,p3_reg,p5_reg,p6_reg,p7_reg,p8_reg;
	wire or_out, add_out_1, add_out_2, add_out_3, add_out_4;
	wire [10:0] gx , gy, gx_comp, gy_comp;
	wire [10:0] gx_2s_comp, gy_2s_comp;   
	wire [7:0] out_temp;
	wire [10:0] abs_gx,abs_gy;	
	wire [10:0] sum;


	reg [7:0] out_reg;


	reg [10:0] gx_reg, gy_reg;		

	//Horizontal Gradient
	sobel_add_nb #(9) S0 (.a(p2_reg),.b(p0_reg),.ans_out(p2_p0),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	sobel_add_nb #(9) S1 (.a(p5_reg),.b(p3_reg),.ans_out(p5_p3),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	assign center_1 = p5_p3 << 1;
	sobel_add_nb #(9) S2 (.a(p8_reg),.b(p6_reg),.ans_out(p8_p6),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	sobel_add_nb #(9) S3 (.a(p2_p0),.b(p8_p6),.ans_out(center_rm),.subtract(1'b0),.cout(add_out_1)); 	//9-bit Adder
	assign center_2 = {add_out_1,center_rm};
	sobel_add_nb #(10) S4 (.a(center_1),.b(center_2),.ans_out(gx_temp),.subtract(1'b0),.cout(add_out_2)); 	//10-bit Adder
	assign gx = {add_out_2,gx_temp};


	//Vertical Gradient
	sobel_add_nb #(9) S5 (.a(p0_reg),.b(p6_reg),.ans_out(p0_p6),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	sobel_add_nb #(9) S6 (.a(p1_reg),.b(p7_reg),.ans_out(p1_p7),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	assign center_3 = p1_p7 << 1; 
	sobel_add_nb #(9) S7 (.a(p2_reg),.b(p8_reg),.ans_out(p2_p8),.subtract(1'b1),.cout()); 		//9-bit Subtractor
	sobel_add_nb #(9) S8 (.a(p2_p8),.b(p0_p6),.ans_out(center_tm),.subtract(1'b0),.cout(add_out_3));  	//9-bit Adder
	assign center_4 = {add_out_3,center_tm};
	sobel_add_nb #(10) S9 (.a(center_3),.b(center_4),.ans_out(gy_temp),.subtract(1'b0),.cout(add_out_4));  //10-bit Adder
	assign gy = {add_out_4,gy_temp};

	assign out = out_reg;
	
	always @(posedge clk)
	begin
		gx_reg <= gx;
		gy_reg <= gy;
		out_reg <= out_temp;
		p0_reg <= p0;
		p1_reg <= p1;
		p2_reg <= p2;
		p3_reg <= p3;
		p5_reg <= p5;
		p6_reg <= p6;
		p7_reg <= p7;
		p8_reg <= p8;

	end


	not_11b N0 (gx_reg,gx_comp);
	not_11b N1 (gy_reg,gy_comp);
	
	sobel_add_nb #(11) S10 (.a(11'b1), .b(gx_comp), .ans_out(gx_2s_comp), .subtract(1'b0), .cout());	//11-bit Adder
	sobel_add_nb #(11) S11 (.a(11'b1), .b(gy_comp), .ans_out(gy_2s_comp), .subtract(1'b0), .cout());	//11-bit Adder

	mux_11b M0 (gx_reg, gx_2s_comp, gx_reg[8], abs_gx);
	mux_11b M1 (gy_reg, gy_2s_comp, gy_reg[8], abs_gy);

	sobel_add_nb #(11) S12 (.a(abs_gx), .b(abs_gy), .ans_out(sum), .subtract(1'b0), .cout());	//11-bit Adder

	mux_8b M2 (sum[7:0], 8'hff, sum[8], out_temp); 

endmodule