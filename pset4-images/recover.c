/**
* recover.c
* Charles Williams
* lose311@aol.com
*
* Recovers JPEGs from a corrupted disk called 'card.raw'
*/
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    // confirm no command arguments
    if (argc != 1)
    {
        printf("No input arguments accepted. Please try again.\n");
        return 1;
    }    
    
    // open input file or return error
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open 'card.raw'\n");
        return 2;
    }
        
    // create buffer, title counter, and output file
    BYTE buffer[512]; 
    int title = -1; 
    FILE* outptr;
    
    // read blocks until EOF
    while (fread(buffer, 512, 1, inptr) == 1)
    {
        // check for start of JPEG image file
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // if there is already an output file open, close it
            if (title != -1)
                fclose(outptr);
                
            // otherwise increment title and open an output file    
            // return error 3 if problem with file creation
            title++;
            char outfile[8];
            sprintf(outfile, "%03d.jpg", title);
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create file.\n");
                return 3;
            }
            
            // write block of memory
            fwrite(buffer, sizeof(BYTE), 512, outptr);
        }
        
        // if no new JPEG file is found, continue writing blocks
        else if (title != -1)
        {
            fwrite(buffer, sizeof(BYTE), 512, outptr);
        }
    }

    // close input file and return 0
    fclose(inptr);
    return 0;
}
