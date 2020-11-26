#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    // Check if there are two arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // Open the memory card
    FILE *file = fopen(argv[1], "r");

    // Check if there is something in the file
    if (file == NULL)
    {
        fprintf(stderr, "Invalid file\n");
        return 1;
    }
    
    int jpg_counter = 0;
    // Array of each memory card block
    BYTE buffer[512];
    // File name is stored in 1 byte
    char filename[8];
    FILE *img;
    
    // While it is possible to read inside the file
    while (fread(&buffer, sizeof(buffer), 1, file))
    {
        // If the file is jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If it's the first image
            if (jpg_counter == 0)
            {
                // Creates the formatted image name, indicated by its number
                sprintf(filename, "%03i.jpg", jpg_counter);
                // Open the image to write on it
                img = fopen(filename, "w");
                // Write the image
                fwrite(&buffer, sizeof(buffer), 1, img);
                
                jpg_counter++;
            }
            
            // If you already have another image
            else if (jpg_counter > 0)
            {
                // Close the previous image
                fclose(img);
                
                sprintf(filename, "%03i.jpg", jpg_counter);
                img = fopen(filename, "w");
                fwrite(&buffer, sizeof(buffer), 1, img);
                
                jpg_counter++;
            }
        } 
        else if (jpg_counter > 0)
        {
            // Keep writing, if an image is already being written
            fwrite(&buffer, sizeof(buffer), 1, img);
        }
    }

    fclose(file);
    fclose(img);
}
