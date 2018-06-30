#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

int main(int argc, char *argv[1])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: argc must = 2\n");
        return 1;
    }

    //remember file name
    char *infile = argv[1];

    //open input file
    FILE *inptr = fopen(infile, "r");
    if(inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 2;
    }

    //create new buffer
    uint8_t buffer[512];

    //create output file
    FILE *img = NULL;

    //filename buffer
    char filename[8];

    int counter = 0;

    //iterate over file 512 times 1 bytes at a time until EOF
    while(fread(buffer, 1, 512, inptr) != 0x00)
    {
        //check to see if we have a jpeg
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if this is the first jpg
            if(img == NULL)
            {

                //create jpeg file
                sprintf(filename, "%03i.jpg", counter);
                counter++;

                //open the new jpg
                img = fopen(filename, "w");

                if(img == NULL)
                {
                    fprintf(stderr, "could not create new jpeg");
                    return 3;
                }

                //write jpeg to newly created img
                fwrite(buffer, 1, 512, img);
            }

            //if not first jpeg
            else
            {
                //close last jpeg
                fclose(img);

                //create jpeg file
                sprintf(filename, "%03i.jpg", counter);
                counter++;

                //open the new jpeg
                img = fopen(filename, "w");

                if(img == NULL)
                {
                    fprintf(stderr, "could not create new jpeg");
                    return 3;
                }

                //write jpeg to newly created img
                fwrite(buffer, 1, 512, img);
            }
        }
        //if jpeg header not found
        else
        {
            if(img != NULL)
            {
                //write jpeg to newly created img
                fwrite(buffer, 1, 512, img);
            }
        }
    }

    fclose(img);
    fclose(inptr);
}