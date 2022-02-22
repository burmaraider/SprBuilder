#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

const char blankData[] =
{
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

bool PrintHelp();
bool ParseCommandLine(int argc, char *argv[]);
void SaveSpriteFile();

char* fileName;
int frameCount;
int frameRate;
char* basePath;

int main(int argc, char *argv[])
{

    if (!ParseCommandLine(argc, argv))
        return 0;

    SaveSpriteFile();

    return 0;
}

bool PrintHelp()
{
    printf("SprBuilder will create a sprite for you based on the number of frames you input.\n\n");
    printf("Usage: -name <base file name> -frames <number of frames> -framerate <frames per second> -basepath <base folder path>\n\n");

    printf("-name        Base filename of the textures to use ex: mysprite[0].dtx\n");
    printf("-frames      How many frames is your animation?\n");
    printf("-framerate   How many frames per second is this animation?\n");
    printf("-basepath    Where are your textures located? ex: WORLDTEXTURES\\CUSTOM\\MYFOLDER \n\n");

    printf("Example: -name newsprite -frames 12 -framerate 12 -basepath WorldTextures\\custom\\folder\n");

    return false;
}

bool ParseCommandLine(int argc, char *argv[])
{
    bool returnStatus = true;
    if (argc <= 1)
    {
        returnStatus = PrintHelp();
    }
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-name") == 0)
        {
            i++;
            if(i >= argc)
                returnStatus = PrintHelp();
            else
                fileName = argv[i];
        }
        else if(strcmp(argv[i], "-frames") == 0)
        {
            i++;
            if(i >= argc)
                returnStatus = PrintHelp();
            else
                frameCount = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-framerate") == 0)
        {
            i++;
            if(i >= argc)
                returnStatus = PrintHelp();
            else
                frameRate = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-basepath") == 0)
        {
            i++;
            if(i >= argc)
                returnStatus = PrintHelp();
            else
                basePath = argv[i];
        }
        else
            returnStatus = PrintHelp();
    }
    return returnStatus;
}

void SaveSpriteFile()
{
    FILE *filePtr;
    char fileExt[256];
    memset(fileExt, 0, 256);

    sprintf(fileExt, "%s.spr", fileName);

    filePtr = fopen(fileExt, "w+b");

    fwrite(&frameCount, sizeof(uint32_t), 1, filePtr);
    fwrite(&frameRate, sizeof(uint32_t), 1, filePtr);
    fwrite(blankData, sizeof(blankData), 1, filePtr);

    for (size_t i = 0; i < (size_t)frameCount; i++)
    {
        char fileBuffer[256];
        memset(fileBuffer, 0, sizeof(fileBuffer));
        sprintf(fileBuffer, "%s\\%s%d.dtx", basePath, fileName, i);

        int length = strlen(fileBuffer);

        fwrite(&length, sizeof(uint16_t), 1, filePtr);
        fwrite(fileBuffer, strlen(fileBuffer), 1, filePtr);
    }

    fclose(filePtr);
    free(filePtr);
}