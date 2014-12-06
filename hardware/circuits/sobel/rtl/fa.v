`timescale 1ns/1ps
module fa(a, b, c_in, s, c_out);

	input a, b, c_in;
	output c_out, s;

	wire w0, w1, w2, w3;

	xor x0(w0, a, b);
	xor x1(s, w0, c_in);

	and u2(w1, a, b);
	and u3(w2, a, c_in);
	and u4(w3, b, c_in);

	or u5(c_out, w1, w2, w3);
endmodule