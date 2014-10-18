
#include <stdio.h>
#include <string.h>

#include "tp2.h"

void sierpinski_asm    (unsigned char* src, unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

void sierpinski_c    (unsigned char *src,unsigned char *dst, int cols, int filas,
                      int src_row_size, int dst_row_size);

typedef void (sierpinski_fn_t) ( unsigned char*, unsigned char*, int, int, int, int);

void leer_params_sierpinski(configuracion_t *config, int argc, char *argv[]) {
}

void aplicar_sierpinski(configuracion_t *config)
{
	unsigned long a,b;
	a = 0;
	b = 0;
	MEDIR_TIEMPO_START(a);
	sierpinski_fn_t *sierpinski = SWITCH_C_ASM ( config, sierpinski_c, sierpinski_asm ) ;
	buffer_info_t info = config->src;
	MEDIR_TIEMPO_START(a);	
	sierpinski(info.bytes, config->dst.bytes, info.width, info.height, info.width_with_padding, config->dst.width_with_padding);
        MEDIR_TIEMPO_STOP(b);
        fprintf(config->archivo_mediciones, "%lu\n", b-a);
}

void ayuda_sierpinski()
{
	printf ( "       * sierpinski\n" );
	printf ( "           Parámetros     : \n"
	         "                         ninguno\n");
	printf ( "           Ejemplo de uso : \n"
	         "                         sierpinski -i c imagen.bmp\n" );
}


