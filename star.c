#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "star.h"
#include "image.h"

int star_read_from_file(struct star* s, FILE* f)
{
    double x,y,z,magnitude;
    int draper, harvard;
	if(fscanf(f, "%lf %lf %lf %i %lf %i", &x, &y, &z, &draper, &magnitude, &harvard) == 6)
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
    int pruf1 = 0; 
    int pruf2 = 0;
   // int num_stars2 = num_stars - 1;
    while(fscanf(f, "%i,%i", &draperStar1, &draperStar2) != EOF)
    {
        int currentStar1Index = 0;
       // printf("%i %i\n", num_stars, stars[1].draper);
        for(int i = 0; i < num_stars ; i++)
        {
            if ( draperStar1 != stars[currentStar1Index].draper)
            {
            currentStar1Index++;
            }
            else 
            {
                pruf1 = 1;
                break;
            }
        }
        if (pruf1 != 1 )
        {
            continue;
        }
        
        int currentStar2Index = 0;
        for(int i = 0; i < num_stars ; i++)
        {
            if ( draperStar2 != stars[currentStar2Index].draper)
            {
            currentStar2Index++;
            }
            else 
            {
                pruf2 = 1;
                break;
            }
        }
        if (pruf2 != 1)
        {
            continue;
        }
        
            if (currentStar2Index > currentStar1Index)
            {
                int  x1,y1,x2,y2;
                star_coord_to_pixel(&stars[currentStar1Index], img, &x1,&y1);
                star_coord_to_pixel(&stars[currentStar2Index], img, &x2,&y2);
                image_draw_line(img, 0xffff00, x1, y1, x2, y2);
                }
            else
            {
                int  x1,y1,x2,y2;
                star_coord_to_pixel(&stars[currentStar1Index], img, &x1,&y1);
                star_coord_to_pixel(&stars[currentStar2Index], img, &x2,&y2);
                image_draw_line(img, 0xffff00, x2, y2, x1, y1);
            }
        
    }
}
