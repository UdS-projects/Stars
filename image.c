#include <stdlib.h>
#include <string.h>

#include "image.h"

void image_init(struct image* img, int w, int h)
{
	if(w>0 && h>0)
	{
		img->w = w;
		img->h = h;
		img->data = (int*) calloc(w*h, sizeof(int));
	}
}

void image_destroy(struct image* img)
{
	free(img->data);
}

void image_draw_pixel(struct image* img, int color, int x, int y)
{
	if(x>=0 && y>=0 && x<(img->w) && y<(img->h))
	{
		int spot = (img->w)*y+x;
		img->data[spot] = color;
	}
}

/* Helper function for image_draw_line */
void swap(int* a, int* b)
{
	int tmp = *a; 
	*a = *b;
	*b = tmp;
}

/**
 * Draw a line using Bresenham's algorithm.
 * @see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
void image_draw_line(struct image* img, int color, int x0, int y0, int x1, int y1)
{

	int dx    = x1 > x0 ? x1 - x0 : x0 - x1;
	int dy    = y1 > y0 ? y1 - y0 : y0 - y1;
	int sx    = x1 > x0 ? 1 : -1;
	int sy    = y1 > y0 ? 1 : -1;
	int not_steep = dy > dx;

	if (not_steep) {
		swap(&x0, &y0);
		swap(&dx, &dy);
		swap(&sx, &sy);
	}

	int e = 2 * dy - dx;
	for (int i = 0; i < dx; i++) {
		if (not_steep)
			image_draw_pixel(img, color, y0, x0);
		else
			image_draw_pixel(img, color, x0, y0);

		if (e >= 0) {
			int d  = 1 + e / (2 * dx);
			y0    += sy < 0 ? -d : d;
			e     -= d * 2 * dx;
		}

		x0 += sx;
		e += 2 * dy;
	}
}

void image_write_to_file(struct image* img, FILE* f)
{
    fprintf(f, "P3\n");
    fprintf(f, "%i %i\n", img->w, img->h);
    fprintf(f, "255\n");
    
    for(int i=0; i<(img->h); i++)
    {
        for(int j=0; j<(img->w); j++)
        {
            int red = (((img->data[j]) >> 16) & 0xFF);
            int green = (((img->data[j]) >> 8) & 0xFF);
            int blue = (((img->data[j])) & 0xFF);
            if (0 <= red && red <= 255 && 0 <= green && green <= 255 && 0 <= blue && blue <= 255 )
            fprintf(f, "%i %i %i ", red, green, blue);
            
        }

        fprintf(f, "\n");
    }
}
