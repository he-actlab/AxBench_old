// Modified by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu

// 5-tap FIR filter
// y[n] = 1/32 x[n] + 1/16 x[n-1] + 1/8 x[n-2] + 1/4 x[n-3] + 1/2 x[n-4]

`timescale 1ns/1ps

module shift01(A, O);
	input [7:0] A;
	output [7:0] O;

	assign O = A >> 3'b101;
endmodule


module shift02(A, O);
	input [7:0] A;
	output [7:0] O;

	assign O = A >> 3'b100;
endmodule

module shift03(A, O);
	input [7:0] A;
	output [7:0] O;

	assign O = A >> 3'b011;
endmodule

module shift04(A, O);
	input [7:0] A;
	output [7:0] O;

	assign O = A >> 3'b010;
endmodule

module shift05(A, O);
	input [7:0] A;
	output [7:0] O;

	assign O = A >> 3'b001;
endmodule



// main module FIR
module fir(clk,rst,x,dataout);
input [7:0]x;
input clk,rst;
output [9:0] dataout;


reg  [9:0] dataout_reg;
reg [7:0] x_reg;
wire [9:0] dataout_wire;


wire [7:0]d1,d2,d3;
wire [7:0]m1,m2,m3,m4,m5;
wire [7:0]d11,d12,d13,d14;


assign dataout = dataout_reg;

shift01 SH01(x_reg,m1);

dff u2(clk,rst,x_reg,d11);
shift02 SH02(d11, m2);

assign d1=m1+m2;

dff u4(clk,rst,d11,d12);
shift03 SH03(d12, m3);


assign d2=d1+m3;
dff u6(clk,rst,d12,d13);
shift04 SH04(d13, m4);

assign d3=d2+m4;
dff u8(clk,rst,d13,d14);
shift05 SH05(d14, m5);

assign dataout_wire=d3+m5;

always @(posedge clk) begin
    if(rst) 
    begin
   		dataout_reg <= 0;
   		x_reg  		<= 0;
   	end
    else 
    begin
    	dataout_reg <= dataout_wire;
    	x_reg 		<= x;
    end
end 
endmodule

module dff(clk,rst,d,q);// sub module d flipflop
input clk,rst;
input [7:0]d;
output [7:0]q;
reg [7:0]q;
always@(posedge clk)
begin
	if(rst==1)
	begin
		q=0;
	end
	else
	begin
		q=d;
	end
end

endmodule
