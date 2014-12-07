// Developed by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu


//	inversek.v
// 	l1 = 0.5
// 	l2 = 0.5
//	theta2 = acos(((x * x) + (y * y) - (l1 * l1) - (l2 * l2))/(2 * l1 * l2)) ;
//	theta1 = asin((y * (l1 + l2 * cos(theta2)) - x * l2 * sin(theta2))/(x * x + y * y)) ;
//

`timescale 1ns/1ps

`define BIT_WIDTH 32
`define FRACTIONS 15

module inversek(clk, rst, x_i, y_i, theta1_o, theta2_o);

	input clk, rst;
	input [`BIT_WIDTH-1:0] x_i, y_i;
	output [`BIT_WIDTH-1:0] theta1_o, theta2_o;

	wire [`BIT_WIDTH-1:0] x_sqr_w, y_sqr_w;
	wire [`BIT_WIDTH-1:0] xy_sum_w;
	wire [`BIT_WIDTH-1:0] theta2_num_w;
	wire [`BIT_WIDTH-1:0] theta2_div_w;
	wire [`BIT_WIDTH-1:0] theta2_w;
	wire [`BIT_WIDTH-1:0] cos_theta2_w, sin_theta2_w;
	wire [`BIT_WIDTH-1:0] cos_mult12_w, sin_mult12_w;
	wire [`BIT_WIDTH-1:0] first_op_w;
	wire [`BIT_WIDTH-1:0] part_x_w, part_y_w;
	wire signed_bit;
	wire [`BIT_WIDTH-1:0] part_x_part_y_w;
	wire [`BIT_WIDTH-1:0] theta1_div_w;
	wire [`BIT_WIDTH-1:0] theta1_w;



	reg [`BIT_WIDTH-1:0] x_reg, y_reg;
	reg [`BIT_WIDTH-1:0] x_sqr_reg, y_sqr_reg;
	reg [`BIT_WIDTH-1:0] xy_sum_reg;
	reg [`BIT_WIDTH-1:0] theta2_num_reg;
	reg [`BIT_WIDTH-1:0] theta2_div_reg;
	reg [`BIT_WIDTH-1:0] theta2_reg;
	reg [`BIT_WIDTH-1:0] cos_theta2_reg, sin_theta2_reg;
	reg [`BIT_WIDTH-1:0] cos_mult12_reg, sin_mult12_reg, sin_mult12_reg_2;
	reg [`BIT_WIDTH-1:0] first_op_reg;
	reg [`BIT_WIDTH-1:0] part_x_reg, part_y_reg;
	reg [`BIT_WIDTH-1:0] part_x_part_y_reg;
	reg [`BIT_WIDTH-1:0] theta1_div_reg;
	reg [`BIT_WIDTH-1:0] theta1_reg;

	
	//parameter const_1 = `BIT_WIDTH'b10000000100001001000000000000000;  // -(0.5^2 + 0.5^2)
	//parameter const_2 = `BIT_WIDTH'b00000000100001000000000000000000;  // 11*12*2
	parameter const_1 = `BIT_WIDTH'b10000000000000000100000000000000;  // -(l1^2 + l2^2)
	parameter const_2 = `BIT_WIDTH'b00000000000000000100000000000000;  // l1*l2*2



	always @ (posedge clk) begin
		if(rst)
		begin
			x_reg 		<= 0;
			y_reg 		<= 0;
			x_sqr_reg 	<= 0;
			y_sqr_reg 	<= 0;
			xy_sum_reg	<= 0;
			theta2_num_reg <= 0;
			theta2_div_reg <= 0;
			theta2_reg 	<= 0;
			cos_theta2_reg <= 0;
			sin_theta2_reg <= 0;
			cos_mult12_reg <= 0;
			sin_mult12_reg <= 0;
			first_op_reg <= 0;
			sin_mult12_reg_2 <= 0;
			part_x_reg <= 0;
			part_y_reg <= 0;
			part_x_part_y_reg <= 0;
			theta1_div_reg <= 0;
			theta1_reg <= 0;
		end
		else
		begin
			x_reg 		<= x_i;
			y_reg 		<= y_i;
			x_sqr_reg 	<= x_sqr_w;
			y_sqr_reg 	<= y_sqr_w;
			xy_sum_reg  <= xy_sum_w;
			theta2_num_reg <= theta2_num_w; 
			theta2_div_reg <= theta2_div_w;
			theta2_reg 	<= theta2_w;
			cos_theta2_reg <= cos_theta2_w;
			sin_theta2_reg <= sin_theta2_w;
			cos_mult12_reg <= cos_mult12_w;
			sin_mult12_reg <= sin_mult12_w;
			sin_mult12_reg_2 <= sin_mult12_reg;
			first_op_reg <= first_op_w;
			part_x_reg 	<= part_x_w;
			part_y_reg 	<= part_y_w;
			part_x_part_y_reg <= part_x_part_y_w;
			theta1_div_reg <= theta1_div_w;
			theta1_reg <= theta1_w;
		end		
	end

	// outputs
	assign theta2_o = theta2_reg;
	assign theta1_o = theta1_reg;


	// x^2 -- y^2
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) x_multiplier(.i_multiplicand(x_reg), .i_multiplier(x_reg), .o_result(x_sqr_w), .ovr(overflow_flag), .clk(clk), .rst(rst));
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) y_multiplier(.i_multiplicand(y_reg), .i_multiplier(y_reg), .o_result(y_sqr_w), .ovr(overflow_flag), .clk(clk), .rst(rst));

	// x^2 + y^2
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) xy_adder(.a(x_sqr_reg), .b(y_sqr_reg), .c(xy_sum_w), .clk(clk), .rst(rst));

	// x^2 + y^2 -(l1^2 + l2^2) 		
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) num_adder(.a(const_1), .b(xy_sum_reg), .c(theta2_num_w), .clk(clk), .rst(rst));

	// (x^2 + y^2 -(11^2 + 12^2)) / (2*11*12)
	floating_div_32b #(`FRACTIONS,`BIT_WIDTH) my_divider(.i_dividend(theta2_num_reg), .i_divisor(const_2), .i_start(1'b1), .i_clk(clk), .o_quotient_out(theta2_div_w), .rst(rst), .o_complete(), .o_overflow());

	// acos((x^2 + y^2 -(11^2 + 12^2)) / (2*11*12))
	acos_lut U0 (theta2_div_reg, theta2_w);

	// cos(theta2) -- sin(theta2)
	cos_lut U1 (theta2_reg, cos_theta2_w);
	sin_lut U2 (theta2_reg, sin_theta2_w);
	
	// l2*cos(theta2) -- l2*sin(theta2)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) cos_multiplier(.i_multiplicand(cos_theta2_reg), .i_multiplier(`BIT_WIDTH'b00000000000000000100000000000000), .o_result(cos_mult12_w), .ovr(overflow_flag), .clk(clk), .rst(rst));		
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) sin_multiplier(.i_multiplicand(sin_theta2_reg), .i_multiplier(`BIT_WIDTH'b00000000000000000100000000000000), .o_result(sin_mult12_w), .ovr(overflow_flag), .clk(clk), .rst(rst));
	
	// l1 + l2*cos(theta2)
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) n_adder(.a(cos_mult12_reg), .b(`BIT_WIDTH'b00000000000000000100000000000000), .c(first_op_w), .clk(clk), .rst(rst));	

	// y * (l1 + l2*cos(theta2))
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) multiplier_1(.i_multiplicand(first_op_reg), .i_multiplier(y_reg), .o_result(part_y_w), .ovr(overflow_flag), .clk(clk), .rst(rst));
	// x * l2 * sin(theta2)
	floating_multiplier_32b #(`FRACTIONS,`BIT_WIDTH) multiplier_2(.i_multiplicand(sin_mult12_reg_2), .i_multiplier(x_reg), .o_result(part_x_w), .ovr(overflow_flag), .clk(clk), .rst(rst));
		
	// check the sign
	assign signed_bit = ~part_x_reg[31];		

	// part_x + part_y
	floating_adder_32b #(`FRACTIONS,`BIT_WIDTH) adder_123 (.a({signed_bit,part_x_reg[30:0]}), .b(part_y_reg), .c(part_x_part_y_w), .clk(clk), .rst(rst));

	// (part_x + part_y) / xy_sum
	floating_div_32b #(`FRACTIONS,`BIT_WIDTH) m_divider(.i_dividend(part_x_part_y_reg), .i_divisor(xy_sum_reg), .i_start(1'b1), .i_clk(clk), .rst(rst), .o_quotient_out(theta1_div_w), .o_complete(), .o_overflow());
		
	// theta1
	asin_lut U3 (theta1_div_reg, theta1_w);
	
endmodule
