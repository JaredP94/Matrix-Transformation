#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "omp.h"

#define CHUNKSIZE 100
struct arrayContainer
{
    int * _array_ptr;
    int _array_capacity;
    int _number_of_dimensions;
};

struct arrayContainer generateArray(int dimensions[], int dimension_length)
{
    int _capacity = 1;

    for (int i = 0; i < dimension_length; i++)
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

    struct arrayContainer arrayInfo = {_created_array, _capacity, dimension_length};

    return arrayInfo;
}

struct arrayContainer initializeZero(struct arrayContainer arrayInfo)
{
    int chunk = CHUNKSIZE;
    #pragma omp parallel shared(arrayInfo,chunk)// Start of parallel region
    {
        #pragma omp for schedule(static,chunk)
        for (int i = 0; i < arrayInfo._array_capacity; i++)
        {
            arrayInfo._array_ptr[i] = 0;
        }
    }

    return arrayInfo;
}

struct arrayContainer uniformOne(struct arrayContainer arrayInfo)
{
    const int _amount_to_set = arrayInfo._array_capacity / 10;
    const int _spacing = 10;
    int chunk = CHUNKSIZE;
    #pragma omp parallel shared(arrayInfo,chunk) // Start of parallel region
    {
        #pragma omp for schedule(static,chunk)
        for (int i = 0; i < _amount_to_set; i++)
        {
            arrayInfo._array_ptr[i * _spacing] = 1;
        }
    }

    return arrayInfo;
}

bool isValueInArray(int array[], int value, int array_size)
{
    for (int i = 0; i < array_size - 1; i++)
    {
        if (array[i] == value){
            return true;
        }
    }
    return false;
}

struct arrayContainer uniformRandom(struct arrayContainer arrayInfo, int dimensions[])
{
    const int _amount_to_print = arrayInfo._array_capacity / 20;
    int _random_indices[_amount_to_print];
    const int _spacing = 20;
    const int _random_index = rand() % dimensions[0];
    int * _dimension_capacity_array = (int*)malloc(arrayInfo._array_capacity * sizeof(int));
    int * _array_coordinates = (int*)malloc(arrayInfo._array_capacity * sizeof(int));
    int _dimension_capacity = 1;
    const int _index_increment = 1;

    // Calculate uniform random indices
    for (int i = 0; i < _amount_to_print; i++)
    {
        int _random_index;
        do
        {
            _random_index = rand() % arrayInfo._array_capacity;
            _random_indices[i] = _random_index;
        }
        while (isValueInArray(_random_indices, _random_index, i) == true);
    }

    // Calculates max indices per each dimension increment
    for (int i = 0; i < arrayInfo._number_of_dimensions; i++)
    {
        _dimension_capacity_array[i] = _dimension_capacity * dimensions[i];
        _dimension_capacity = _dimension_capacity * dimensions[i];
    }

    // Initialise coord array
    memset(_array_coordinates, 0, sizeof arrayInfo._number_of_dimensions);

    // Calculate coords based on index from 1D array
    printf("Format = [coords] : [value] \n");
    for (int i = 0; i < _amount_to_print; i++)
    {
        int _target_index = _random_indices[i];

        for (int j = arrayInfo._number_of_dimensions; j > 0; j--)
        {
            if(_target_index / _dimension_capacity_array[j-1] > 0)
            {
                _array_coordinates[j] = _target_index / _dimension_capacity_array[j-1];
                _target_index = _target_index - (_array_coordinates[j] * _dimension_capacity_array[j-1]);
            }
        }

        _array_coordinates[0] = _target_index;

        printf("Iteration %d: [ ", i+_index_increment);
        for (int k = 0; k < arrayInfo._number_of_dimensions; k++)
        {
            printf("%d ", _array_coordinates[k]);
        }

        printf("] : [ %d ] \n", arrayInfo._array_ptr[_random_indices[i]]);
    }

    return arrayInfo;
}

int main() {
        srand((unsigned int)time(NULL));
        int _dimensions[]={10,10,10};
        struct arrayContainer _generated_array = generateArray(_dimensions, sizeof(_dimensions)/sizeof(int));
        _generated_array = initializeZero(_generated_array);
        _generated_array = uniformOne(_generated_array);
        _generated_array = uniformRandom(_generated_array, _dimensions);
            /* Free the memory we allocated */
        free(_generated_array._array_ptr);

        return 0;
}