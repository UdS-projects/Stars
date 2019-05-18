#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "star.h"
#include "image.h"

int star_read_from_file(struct star* s, FILE* f)
{
    double x,y,z,magnitude;
    int draper, harvard;
	if(fscanf(f, "%lf %lf %lf %i %lf %i", &x, &y, &z, &draper, &magnitude, &harvard) == 6 && x >= -1 && x <= 1 && y >= -1 && y <= 1)
    {
        s->x = x;
        s->y = y;
        s->draper = draper;
        s->magnitude = magnitude;
        return 1;
    }
    return 0;
}

void star_coord_to_pixel(struct star const* s, struct image* const img, int* x, int* y)
{
   double transX = ((1+(s->x))/2)*((img->w)-1);
   double transY = ((1-(s->y))/2)*((img->h)-1);
   *x = transX;
   *y = transY;
}

void star_plot(struct star const* s, struct image* img)
{
    int x, y;
	star_coord_to_pixel(s, img, &x, &y);
    int color = 0x00ffffff;
    image_draw_pixel(img, color, x, y);
}

void draw_constellation_from_file(FILE* f, struct image* img, struct star const* stars, int num_stars)
{
    int draperStar1;
    int draperStar2;
    int num_stars2 = num_stars - 1;
    for(int i=0; fscanf(f, "%i,%i", &draperStar1, &draperStar2) != EOF; i++)
    {
        int currentStar1Index = 0;
        while((draperStar1 != (stars[currentStar1Index].draper)) && (currentStar1Index < num_stars))
        {
            currentStar1Index++;
        }
        if(currentStar1Index == num_stars2 && draperStar1 != (stars[currentStar1Index].draper))
        {
            continue;
        }
        
        int currentStar2Index = 0;
        while(draperStar2 != (stars[currentStar2Index].draper) && currentStar2Index < num_stars)
        {
            currentStar2Index++;
        }
        if(currentStar2Index == num_stars2 && draperStar2 != (stars[currentStar2Index].draper))
        {
            continue;
        }
        
        image_draw_line(img, 0xffff00, stars[currentStar1Index].x, stars[currentStar1Index].y, stars[currentStar2Index].x, stars[currentStar2Index].y);
    }
}
