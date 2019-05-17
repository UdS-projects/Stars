#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "star.h"
#include "image.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
    {
		fprintf(stderr, "syntax: %s size starlist [lines...]\n", argv[0]);
		return EXIT_FAILURE;
	}

	int size = atoi(argv[1]);
	if (size <= 0)
    {
		fprintf(stderr, "size of image cannot be <= 0\n");
		return EXIT_FAILURE;
	}

	struct image img;
	image_init(&img, size, size);

	// TODO: Read in the stars from the file with name argv[2] 
	// save them in an array in the order they are read in and draw them to the image.
    FILE* fp = fopen(argv[2], "r");
    
    double x,y,z,magnitude;
    int draper, harvard;
    int fileSize = 0;
	while(fscanf(fp, "%le %le %le %i %le %i", &x, &y, &z, &draper, &magnitude, &harvard) != 6)
    {
        fileSize++;
    }
    rewind(fp);
    
    struct star *array = (struct star*) malloc((sizeof(struct star))*fileSize);
    int i = 0;
    while(i < fileSize)
    {
        struct star s;
        if(star_read_from_file(&s, fp) == 1)
        {
            array[i] = s;
        }
        else
        {
            break;
        }
        i++;
    }
    fclose(fp);
    
    for(int i=0; i<fileSize; i++)
    {
        star_plot(&array[i], &img);
    }
    
	// open every constellation file
	for (int i = 3; i < argc; i++)
    {
		FILE* f = fopen(argv[i], "r");
		// if the file could not be opened, skip to the next one.
		if(f == NULL)
        {
			fprintf(stderr, "cannot open line file \"%s\"\n", argv[i]);
			continue;
		}
		
		// TODO draw the constellation to the image
		draw_constellation_from_file(f, &img, array, fileSize);
		
		fclose(f);
	}

	// write the painted image to the output file.
	FILE* image_file = fopen("stars.pbm", "w");
	if (image_file == NULL)
    {
		fprintf(stderr, "cannot open output file.\n");
		return EXIT_FAILURE;
	}

	image_write_to_file(&img, image_file);

	fclose(image_file);
	image_destroy(&img);
}
