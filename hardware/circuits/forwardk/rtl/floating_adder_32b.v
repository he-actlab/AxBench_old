// Modified by: Amir Yazdanbakhsh
// Email: a.yazdanbakhsh@gatech.edu

`timescale 1ns / 1ps
module floating_adder_32b #(parameter Q = 15, N = 32)(a, b, c, clk, rst);

	input 	[N-1:0] a;
	input 	[N-1:0] b;
	output 	[N-1:0] c;
	input 			clk, rst;


	reg 	[N-1:0] x_temp, y_temp;
	wire	[N:0]   add_res;

	reg [N-1:0] res;


	// calculate the final result
	assign c = {res[N-1], add_res[N-2:0]};

	always @(a,b) 
	begin
		// both negative or both positive
		if(a[N-1] == b[N-1]) begin
			x_temp <= {1'b0, a[N-2:0]};
			y_temp <= {1'b0, b[N-2:0]};
			//res[N-2:0] = a[N-2:0] + b[N-2:0];		
			res[N-1] = a[N-1];							 
		end
		//	one of them is negative...
		else if(a[N-1] == 0 && b[N-1] == 1) 			//	subtract a-b
		begin		
			if( a[N-2:0] > b[N-2:0] )					//	if a is greater than b,
			begin	
				x_temp <= {1'b0, a[N-2:0]};
				y_temp <= {1'b1, ~b[N-2:0]} + 1;        // 2's complement			
				//res[N-2:0] = a[N-2:0] - b[N-2:0];		//	then just subtract b from a
				res[N-1] = 0;							//	and manually set the sign to positive
			end
			else 
			begin										//	if a is less than b,
				//res[N-2:0] = b[N-2:0] - a[N-2:0];		//	we'll actually subtract a from b to avoid a 2's complement answer
				x_temp <= {1'b1, ~a[N-2:0]} + 1;		//  2's complement
				y_temp <= {1'b0, b[N-2:0]};      
				if (res[N-2:0] == 0)
					res[N-1] = 0;										
				else
					res[N-1] = 1;
			end
		end
		else 
		begin										//	subtract b-a (a negative, b positive)
			if( a[N-2:0] > b[N-2:0] ) 				//	if a is greater than b,
			begin				
				//res[N-2:0] = a[N-2:0] - b[N-2:0];
				x_temp <= {1'b0, a[N-2:0]};
				y_temp <= {1'b1, ~b[N-2:0]} + 1;        // 2's complement
				if (res[N-2:0] == 0)
					res[N-1] = 0;
				else
					res[N-1] = 1;
			end
			else 
			begin										//	if a is less than b,
				//res[N-2:0] = b[N-2:0] - a[N-2:0];		//  then just subtract a from b
				x_temp <= {1'b1, ~a[N-2:0]} + 1;		// 2's complement
				y_temp <= {1'b0, b[N-2:0]};
				res[N-1] = 0;							//	and manually set the sign to positive
			end
		end
	end

	kogge_stone_32b u0(add_res, x_temp, y_temp, clk, rst);

endmodule
