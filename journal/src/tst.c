#include <stdio.h>

int
main(int argc, char **argv)
{
    int i, j;
    char *cp;

    unsigned int   d1;
    unsigned short d2;
    unsigned short d3;
    unsigned short d4;

    unsigned long long d5;

    for(i = 1; i < argc; ++i)
    {
        fprintf(stderr,"converting string %s to decimal\n",argv[i]);

        sscanf(argv[i],"%x%*c%hx%*c%hx%*c%hx%*c%llx",&d1, &d2, &d3, &d4, &d5);
        printf("d1: %x\td2: %hx\td3: %hx\td4: %hx\td5: %llx\n",
            d1, d2, d3, d4, d5);
    }
    return 0;
}
