
#include "tp2.h"


void cropflip_c    (
	unsigned char *src,
	unsigned char *dst,
	int cols,
	int filas,
	int src_row_size,
	int dst_row_size,
	int tamx,
	int tamy,
	int offsetx,
	int offsety)
{
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
	
	int x, y, x2, y2;

	y2 = tamy-1;
	for (y = offsety; y < offsety+tamy	; y++) {
		x2 = tamx;
		for (x = offsetx; x < offsetx+tamx; x++) {
						
			unsigned char r = src_matrix[y][x*4+0];
			
			unsigned char g = src_matrix[y][x*4+1];
			unsigned char b = src_matrix[y][x*4+2];
			unsigned char a = src_matrix[y][x*4+3];
			dst_matrix[y2][x2*4-4] = r;
			dst_matrix[y2][x2*4-3] = g;
			dst_matrix[y2][x2*4-2] = b;
			dst_matrix[y2][x2*4-1] = a;
			x2--;
		}
		y2--;
	} 

}
