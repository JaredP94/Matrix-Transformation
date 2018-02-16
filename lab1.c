#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "omp.h"

int main() {

        srand((unsigned int)time(NULL));

        int dimensions[]={4,4,4};

        // Calculate size of required 1D matrix
        int maxSize=1;
        for (int i=0;i<sizeof(dimensions)/sizeof(dimensions[0]);i++)
        {
            maxSize=maxSize*dimensions[i];
           // printf("%i \n",maxSize);
        }

        // Create a 1D matrix containing random data 
        float flatArray[maxSize];
        float a=100;
        for (int i=0; i<maxSize; i++)
        {
            flatArray[i] = ((float)rand()/(float)(RAND_MAX)) * a;
            printf("%f \n",flatArray[i]);
        }



    
}