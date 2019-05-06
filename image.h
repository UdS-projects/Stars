#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

/**
 * An image data structure with width @p w and height @p h.
 * Use @p image_init to allocate the @p data field and @p image_destroy to deallocate it again.
 */
struct image {
	int w, h;
	int* data;
};

/**
 * Initialize an image structure with a width and a height.
 * We also allocate w * h ints to hold the pixels of the image.
 * Every pixel is an int that contains the red, green, and blue value of the pixel.
 * red is in bits 16-23, green in bits 8-15, blue in bits 0-7.
 */
void image_init(struct image* img, int w, int h);

/**
 * Destroys the image @p img by freeing its data field.
 * Don't use @p img afterwards.
 */
void image_destroy(struct image* img);

/**
 * Draw a pixel to the image.
 * The color is encoded as described in image_init().
 * This function *needs* to clip the pixel, that is, if x and y are outside the image's boundary,
 * the pixel must not be drawn.
 */
void image_draw_pixel(struct image* img, int color, int x, int y);

/**
 * Draws a line from (x0,y0) to (x1,y1).
 */
void image_draw_line(struct image* img, int color, int x0, int y0, int x1, int y1);

/**
 * Writes an image to a portable pixmap (P3) file.
 * See http://en.wikipedia.org/wiki/Netpbm_format for details on the file format.
 */
void image_write_to_file(struct image* img, FILE* f);

#endif
