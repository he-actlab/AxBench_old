// Developed by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu
//
// l1 = 0.5; l2=0.5;
// x = l1 * cos(theta1) + l2 * cos(theta1 + theta2);
// y = l1 * sin(theta1) + l2 * sin(theta1 + theta2);

`timescale 1ns/1ps
`define BIT_WIDTH 32
`define FRACTIONS 15

module forwardk(clk, rst, theta1_i, theta2_i, x_o, y_o);

	input rst, clk;
	input [`BIT_WIDTH-1:0] theta1_i, theta2_i;
	output [`BIT_WIDTH-1:0] x_o, y_o;


	wire [`BIT_WIDTH-1:0] x_w, y_w;
	wire [`BIT_WIDTH-1:0] theta_sum;
	wire [`BIT_WIDTH-1:0] x_1_w, x_2_w, y_1_w, y_2_w, cos_1_w, cos_2_w, sin_1_w, sin_2_w;
	wire overflow_flag;

	reg [`BIT_WIDTH-1:0] x_reg,y_reg;
	reg [`BIT_WIDTH-1:0] x_1_reg, x_2_reg, y_1_reg, y_2_reg, cos_1_reg, cos_2_reg, sin_1_reg, sin_2_reg;
	reg [`BIT_WIDTH-1:0] const_1, const_2;
	reg [`BIT_WIDTH-1:0] theta1_reg,theta2_reg, theta_sum_reg;

	always @(posedge clk)
	begin
		if(rst)
		begin
			theta1_reg 		<= 0;
			theta2_reg 		<= 0;
			theta_sum_reg	<= 0;
			x_reg 			<= 0;
			y_reg 			<= 0;
			cos_1_reg 		<= 0; 
			cos_2_reg 		<= 0;
			sin_1_reg 		<= 0;
			sin_2_reg 		<= 0;
			x_1_reg 		<= 0; 
			x_2_reg 		<= 0;
			y_1_reg 		<= 0;
			y_2_reg 		<= 0;
		end
		else
		begin
			theta1_reg 		<= theta1_i;
			theta2_reg 		<= theta2_i;
			theta_sum_reg 	<= theta_sum;
			x_reg 			<= x_w;
			y_reg 			<= y_w;
			cos_1_reg		<= cos_1_w; 
			cos_2_reg		<= cos_2_w;
			sin_1_reg		<= sin_1_w;
			sin_2_reg		<= sin_2_w;
			x_1_reg			<= x_1_w; 
			x_2_reg 		<= x_2_w;
			y_1_reg 		<= y_1_w;
			y_2_reg 		<= y_2_w;
		end
	end


	assign x_o = x_reg;
	assign y_o = y_reg;

	
	// sum of two thetas
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) theta_adder	(.a(theta1_reg), .b(theta2_reg), .c(theta_sum), .clk(clk), .rst(rst));

	// look-up tables
	cos_lut 						cos_theta_sum 	(theta_sum_reg, cos_2_w);
	sin_lut 						sin_theta_sum 	(theta_sum_reg, sin_2_w);
	cos_lut 						cos_theta1 		(theta1_reg, 	cos_1_w);
	sin_lut 						sin_theta1 		(theta1_reg, 	sin_1_w);


	// performs the multiplications
	// x_1_w = l1 * cos(theta1)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) x1_multiplier(.i_multiplicand(`BIT_WIDTH'b00000000000000000100000000000000), .i_multiplier(cos_1_reg), .o_result(x_1_w), .ovr(overflow_flag), .clk(clk), .rst(rst)); 
	// x_2_w= l2 * cos(theta1 + theta2)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) x2_multiplier(.i_multiplicand(`BIT_WIDTH'b00000000000000000100000000000000), .i_multiplier(cos_2_reg), .o_result(x_2_w), .ovr(overflow_flag), .clk(clk), .rst(rst)); 
	// y_1_w = l1 * sin(theta1)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) y1_multiplier(.i_multiplicand(`BIT_WIDTH'b00000000000000000100000000000000), .i_multiplier(sin_1_reg), .o_result(y_1_w), .ovr(overflow_flag), .clk(clk), .rst(rst)); 
	// y_2_w = l2 * sin(theta1 + theta2)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) y2_multiplier(.i_multiplicand(`BIT_WIDTH'b00000000000000000100000000000000), .i_multiplier(sin_2_reg), .o_result(y_2_w), .ovr(overflow_flag), .clk(clk), .rst(rst));


	// calculates the outputs
	// y_w = l1 * sin(theta1) + l2 * sin(theta1+theta2)
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) 	y_adder			(.a(y_1_reg), .b(y_2_reg), .c(y_w), .clk(clk), .rst(rst));
	// x_w = l1 * cos(theta1) + l2 * cos(theta1+theta2)
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) 	x_adder			(.a(x_1_reg), .b(x_2_reg), .c(x_w), .clk(clk), .rst(rst));

endmodule
