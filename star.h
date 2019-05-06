#ifndef STAR_H
#define STAR_H

#include <stdio.h>

#include "image.h"

struct star {
	double x, y, magnitude;
	int draper;
};

/**
 * Read a star from a file.
 * This function sets the values of s according to the current line in f.
 * @return 1 if the star could be read correctly, 0 otherwise.
 */
int star_read_from_file(struct star* s, FILE* f);

/**
 * Transform star coordinates to image coordinates.
 * The transformed coordinates are stored in the variables pointed-to by x and y.
 */
void star_coord_to_pixel(struct star const* s, struct image* const img, int* x, int* y);

/**
 * Plot a star on the image.
 */
void star_plot(struct star const* s, struct image* img);

/**
 * @brief Draws star constellation from a file.
 *
 * @param f the file being read.
 * @param img the image to draw on.
 * @param s an array of @p star%s.
 * @param num_stars the number of stars in the array @p s.
 */
void draw_constellation_from_file(FILE* f, struct image* img, struct star const* s, int num_stars);

#endif
