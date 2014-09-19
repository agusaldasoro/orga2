
#include "tp2.h"

#define MIN(x,y) ( x < y ? x : y )
#define MAX(x,y) ( x > y ? x : y )

#define P 2

void mblur_c    (
    unsigned char *src,
    unsigned char *dst,
    int cols,
    int filas,
    int src_row_size,
    int dst_row_size)
{
    unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
    unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;
	
	for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i < 2 || j < 2) {
                dst_matrix[i][j*4+0] = 0;
                dst_matrix[i][j*4+1] = 0;
                dst_matrix[i][j*4+2] = 0;
                dst_matrix[i][j*4+3] = 0;
                continue;
            }

            bgra_t *a = &src_matrix[i][j*4];
            bgra_t *c = &src_matrix[i+1][(j+1)*4];
            bgra_t *b = &src_matrix[i-1][(j-1)*4];
            bgra_t *d = &src_matrix[i+2][(j+2)*4];
            bgra_t *e = &src_matrix[i-2][(j+2)*4];

            bgra_t dst;

            dst.r = 0.2 * (a->r + b->r + c->r + d->r + e->r);
            dst.g = 0.2 * (a->g + b->g + c->g + d->g + e->g);
            dst.b = 0.2 * (a->b + b->b + c->b + d->b + e->b);
            dst.a = 0.2 * (a->a + b->a + c->a + d->a + e->a);

            dst_matrix[i][j*4+0] = dst.b;
            dst_matrix[i][j*4+1] = dst.g;
            dst_matrix[i][j*4+2] = dst.r;
            dst_matrix[i][j*4+3] = dst.a;
        }
    }
 
}


