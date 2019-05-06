#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "star.h"

/*
 * utils
 */

bool pass(char const* const test_name)
{
	fprintf(stderr, "PASS: '%s'\n", test_name);
	return true;
}

bool fail(char const* const test_name, char const* const message)
{
	fprintf(stderr, "FAIL: '%s' with message '%s'\n", test_name, message);
	return false;
}

#define FAIL(msg) fail(__func__, (msg))
#define PASS(msg) pass(__func__)

/*
 * tests
 */

bool test_round() {
	struct star s;
	int data[100] = {0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0};
	struct image img ={10,10, data};

	s.x = 0.31;
	s.y = -0.31;

	int x, y;
	star_coord_to_pixel(&s, &img, &x, &y);

	if (x != 5 || y != 5) {
		FAIL("incorrect coordinate transformation; be sure to round as specified in the exercise");
		return false;
	}

	return PASS();
}

bool test_read()
{
	FILE* f = fopen("test_data/validstar.txt", "r");

	struct star s;
	fseek(f, 0, SEEK_SET);
	int res = star_read_from_file(&s, f);
	fclose(f);
	if (res != 1)
		return FAIL("wrong result code");
	if (s.draper != 28)
		return FAIL("wrong draper number read");
	if (s.x < 0.994771 || s.x > 0.994773)
		return FAIL("wrong x coord read");
	if (s.y < 0.023163 || s.y > 0.023165)
		return FAIL("wrong y coord read");
	if (s.magnitude < 4.6 || s.magnitude > 4.62)
		return FAIL("wrong magnitude read");

	return PASS();
}

bool test_transform0()
{
	struct star validstar;
	int x;
	int y;
	int data[100] = {0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0};
	struct image img ={10,10, data};
	
	validstar.x = 1.0;
	validstar.y = 1.0;
	star_coord_to_pixel(&validstar, &img, &x, &y);
	if (x != 9 || y != 0) {
		return FAIL("coordinate transformation incorrect");
	}

	return PASS();
}


bool test_image()
{
	struct image img;

	image_init(&img, 10, 10);
	image_draw_pixel(&img, 0x123456, 2, 5);
	image_draw_pixel(&img, 0xffeedd, 4, 9);
	image_draw_pixel(&img, 0xaa55aa, 0, 0);

	FILE* f = fopen("test.pbm", "wt");
	image_write_to_file(&img, f);
	fclose(f);

	char buf[16];
	buf[0] = '\0'; // for the case that image file is empty
	f = fopen("test.pbm", "r");

	// look if first line contains P3
	fgets(buf, sizeof(buf), f);
	if (strcmp(buf, "P3\n") != 0) {
		fclose(f);
		image_destroy(&img);
		return FAIL("expected P3 but found something else");
	}

	// check, if the dimensions match
	int w, h;
	fscanf(f, "%d %d\n", &w, &h);
	if (w != img.w || h != img.h) {
		fclose(f);
		image_destroy(&img);
		return FAIL("width and height differ");
	}

	// read the maximum rgb component value
	int max_col;
	fscanf(f, "%d\n", &max_col);

	// look at all pixels, if they match with written image
	int pixels = 0;
	while (!feof(f)) {
		int r, g, b;
		fscanf(f, "%d", &r);
		if (feof(f)) break;
		fscanf(f, "%d", &g);
		if (feof(f)) break;
		fscanf(f, "%d", &b);
		if (feof(f)) break;
		int c = r << 16 | g << 8 | b;

		if (r > max_col) {
			fclose(f);
			image_destroy(&img);
			return FAIL("red in pixel is is larger than maximum color specified");
		}
		if (g > max_col) {
			fclose(f);
			image_destroy(&img);
			return FAIL("green in pixel is is larger than maximum color specified");
		}
		if (b > max_col) {
			fclose(f);
			image_destroy(&img);
			return FAIL("blue in pixel is is larger than maximum color specified");
		}

		if (c != img.data[pixels]) {
			fclose(f);
			image_destroy(&img);
			return FAIL("pixel color does not match original color");
		}
		pixels++;
	}

	// check if we read precisely the necessary amount of pixels
	if (pixels != w * h) {
		fclose(f);
		image_destroy(&img);
		return FAIL("file contained wrong number of pixels");
	}

	fclose(f);
	image_destroy(&img);
	return PASS();
}

// pass some out of bounds pixel values to image_draw_pixel
// segfaults if test fails
bool test_pixel_out_of_bounds0()
{
	struct image img;
	image_init(&img, 200, 200);

	image_draw_pixel(&img, 0, 20, 200);
	image_destroy(&img);

	return PASS();
}

bool test_draw_pixel_colors0()
{
	struct image img;
	image_init(&img, 200, 200);

	const int yellow = 0xffff00;

	image_draw_pixel(&img, yellow, 100, 100);
	if (img.data[200*100+100] != yellow) {
		image_destroy(&img);
		return FAIL("color yellow was not properly written");
	} else {
		image_destroy(&img);
		return PASS();
	}
}

// Tests valid star coordinate transformation to pixels with a nonquadratic image
bool test_transfrom_valid_coords_nonquadratic0()
{
	struct star validstar;
	int x;
	int y;
	int data[150] = {0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0};
	struct image img ={10,15, data};

	validstar.x = 1.0;
	validstar.y = 1.0;
	star_coord_to_pixel(&validstar, &img, &x, &y);
	if (x != 9 || y != 0) {
		return FAIL("coordinate (1.0|1.0) wrong transformed");
	} else {
		return PASS();
	}
}

/*
 * main
 */

int main(int argc, char *argv[])
{
	// all tests in an array of function pointers
	static bool (*const all_tests[])() = {
		test_round,
		test_read,
		test_transform0,
		test_pixel_out_of_bounds0,
		test_draw_pixel_colors0,
		test_transfrom_valid_coords_nonquadratic0,
		test_image,
		// feel free to add your own tests here
	};

	static const int num_tests = sizeof(all_tests)/sizeof(*all_tests);

	if (argc == 1) {
		int num_failed = 0;
		for (int i = 0; i != num_tests; ++i) {
			if (!all_tests[i]())
				++num_failed;
		}
		printf("ran %i tests; %i failed\n", num_tests, num_failed);

		return num_failed ? EXIT_FAILURE : EXIT_SUCCESS;
	}

	if (argc == 2) {
		int i = atoi(argv[1]);

		if (0 <= i && i < num_tests)
			return all_tests[i]() ? EXIT_SUCCESS : EXIT_FAILURE;

		fprintf(stderr, "test-number must be within the range [0, %i]\n", num_tests-1);
		return EXIT_FAILURE;
	}

	fprintf(stderr, "syntax: %s [test-number]\n", argv[0]);
	return EXIT_FAILURE;
}
