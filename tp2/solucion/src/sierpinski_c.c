
#include "tp2.h"
#include <math.h>

// float sierpinski(int x, int y, int cols, int rows) {

// 	int b = (int) (floor((y*255.0)/cols));
// 	float ret = (a ^ b);
// 	return ret / 255.0;
// }

void sierpinski_c    (
	unsigned char *src,
	unsigned char *dst,
	int cols,
	int filas,
	int src_row_size,
	int dst_row_size)
{
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

	int x, y;
	for (y = 0; y < filas; y++) {
		for (x = 0; x < cols; x++) {
			int i = (int) (floor((y*255.0)/filas));
			int j = (int) (floor((x*255.0)/cols));
			float s = (i^j) / 255.0;
			
			unsigned char r = src_matrix[y][x*4+0] * s;
			unsigned char g = src_matrix[y][x*4+1] * s;
			unsigned char b = src_matrix[y][x*4+2] * s;
			unsigned char a = src_matrix[y][x*4+3] * s;

			dst_matrix[y][x*4] = r;
			dst_matrix[y][x*4+1] = g;
			dst_matrix[y][x*4+2] = b;
			dst_matrix[y][x*4+3] = a;
		}
	} 

}


