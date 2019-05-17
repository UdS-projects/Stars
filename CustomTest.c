#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "star.h"

int main(int argc, char *argv[])
{
    struct image img;
    image_init(&img, 4, 6);
    for(int i=0; i<24; i++)
    {
        img.data[i] = 0;
    }
    img.data[0] = 0x123456;
    img.data[1] = 0xffffff;
    img.data[2] = 0xff0000;
    img.data[23] = 0x00ff00;
    FILE *fp = fopen("CustomTestData", "w");
    image_write_to_file(&img, fp);
    fclose(fp);
    image_destroy(&img);
    printf("yay\n");
    
    ////////////////////////////
    
    FILE* fp2 = fopen("TestStars.txt", "r");
    double x,y,z,magnitude;
    int draper, harvard;
    int fileSize = 0;
	while(fscanf(fp2, "%lf %lf %lf %i %lf %i", &x, &y, &z, &draper, &magnitude, &harvard) == 6)
    {
        fileSize++;
        printf("Last thing: %i\n", harvard);
    }
    rewind(fp2);
    printf("FileSize: %i\n", fileSize);
    fclose(fp2);
    
    return 0;
}
