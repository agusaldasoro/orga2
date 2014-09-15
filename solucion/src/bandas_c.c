
#include "tp2.h"

int bandas(int i) {
	if (i < 96) {
		return 0;
	} else if (i >= 96 && i < 288) {
		return 64;
	} else if (i >= 288 && i < 480) {
		return 128;
	} else if (i >= 480 && i < 672) {
		return 192;
	}
	return 255;
}

void bandas_c (
	unsigned char *src,
	unsigned char *dst,
	int m,
	int n,
	int src_row_size,
	int dst_row_size
) {
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;

	int x,y;
	for (y = 0; y < m; y++) {
		for (x = 0; x < n; x++) {
			unsigned char b = src_matrix[y][x*4];
			unsigned char g = src_matrix[y][x*4+1];
			unsigned char r = src_matrix[y][x*4+2];
			unsigned char a = src_matrix[y][x*4+3];

			int i = r+g+b;

			int s = bandas(i);
			printf("(%d,%d) => %d+%d+%d = %d\n", x,y,b,g,r,i);
			dst_matrix[y][x*4] = s;
			dst_matrix[y][x*4+1] = s;
			dst_matrix[y][x*4+2] = s;
			dst_matrix[y][x*4+3] = a;
		}
		return;
	} 


}
