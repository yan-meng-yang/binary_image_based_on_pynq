#include "binary.h"
#include <iostream>
using namespace std;


void binary(hls::stream<trans_pkt>& src, hls::stream<trans_pkt>& dst, int rows, int cols)
{
#pragma HLS INTERFACE axis port=src
#pragma HLS INTERFACE axis port=dst
#pragma HLS INTERFACE s_axilite port=rows  bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cols  bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

	trans_pkt data_p;

	PIXEL _sobel;

	ap_uint<13> row, col;
	PIXEL num;

	for(row = 0; row < rows; row++)
	{
#pragma HLS LOOP_TRIPCOUNT min=1 max=720

		for(col = 0; col < cols; col++)
		{
#pragma HLS LOOP_TRIPCOUNT min=1 max=1280
#pragma HLS pipeline
			data_p = src.read();
			num = (PIXEL) data_p.data;
			_sobel = (num>=140?255:0);

			if ((row == rows - 1) && (col == cols - 1))
				data_p.last = 1;
			else
				data_p.last= 0;
			data_p.data = _sobel;
			dst.write(data_p);
		}
	}
}
