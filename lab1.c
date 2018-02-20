#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "omp.h"

#define CHUNKSIZE 100
struct arrayContainer
{
    int * _array_ptr;
    int _array_capacity;
};

struct arrayContainer generateArray(int dimensions[]){
    int _capacity = 1;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < sizeof(dimensions)/sizeof(int); i++)
    {
        _capacity = _capacity * dimensions[i];
    }

    static int * _created_array; // Creates a pointer to a block of memory on the heap
    _created_array =(int*)malloc(_capacity * sizeof(int));

    // If the array cannot be created, exit the program
    if (_created_array == NULL)
    {
        printf("Could not allocate required memory\n");
        exit(1);
    }

    int chunk = CHUNKSIZE;

    //clock_t begin = clock();
    #pragma omp parallel shared(_created_array,chunk)// Start of parallel region
    {
        #pragma omp for schedule(dynamic,chunk)
        for (int i = 0; i < _capacity; i++)
        {
            _created_array[i] = rand() % 10;
              // printf("\n %i ",omp_get_thread_num()); // see the thread ID associated with each loop iteration
        }
    } // End of parallel region

    // Timing the code
    /*
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent %f", time_spent);
    printf("\n");
    */

    struct arrayContainer arrayInfo = {_created_array, _capacity};

    return arrayInfo;
}

struct arrayContainer initializeZero(struct arrayContainer arrayInfo){
    int chunk = CHUNKSIZE;
    #pragma omp parallel shared(arrayInfo,chunk)// Start of parallel region
    {
        #pragma omp for schedule(dynamic,chunk)
        for (int i = 0; i < arrayInfo._array_capacity; i++)
        {
            arrayInfo._array_ptr[i] = 0;
        }
    }

    return arrayInfo;
}

struct arrayContainer uniformOne(struct arrayContainer arrayInfo){
    int _amount_to_set = arrayInfo._array_capacity / 10;
    int _spacing = arrayInfo._array_capacity / _amount_to_set;
    int chunk = CHUNKSIZE;
    #pragma omp parallel shared(arrayInfo,chunk) // Start of parallel region
    {
        #pragma omp for schedule(dynamic,chunk)
        for (int i = 0; i < _amount_to_set; i++)
        {
            arrayInfo._array_ptr[i * _spacing] = 1;
        }
    }

    return arrayInfo;
}

struct arrayContainer uniformRandomOne(struct arrayContainer arrayInfo, int dimensions[]){
    int _amount_to_set = arrayInfo._array_capacity / 20;
    int _spacing = arrayInfo._array_capacity / _amount_to_set;
    int _random_index = rand() % dimensions[0];

    for (int i = 0; i < _amount_to_set; i++)
    {
        arrayInfo._array_ptr[_random_index + i * _spacing] = 1;
    }

    return arrayInfo;
}

int main() {
        int _dimensions[]={500,500,500};
        struct arrayContainer _generated_array = generateArray(_dimensions);
        _generated_array = initializeZero(_generated_array);
        _generated_array = uniformOne(_generated_array);
        _generated_array = uniformRandomOne(_generated_array, _dimensions);
            /* Free the memory we allocated */
        free(_generated_array._array_ptr);

        return 0;
}