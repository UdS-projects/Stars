#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "star.h"

int main()
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
    
    return 0;
}
