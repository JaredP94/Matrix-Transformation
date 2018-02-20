#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "omp.h"

struct arrayContainer
{
    int * _array_ptr;
    int _array_capacity;
    int _number_of_dimensions;
};

struct arrayContainer generateArray(int dimensions[], int dimension_length){
    int _capacity = 1;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < dimension_length; i++)
    {
        _capacity = _capacity * dimensions[i];
    }

    static int * _created_array;
    _created_array =(int*)malloc(_capacity * sizeof(int));

    for (int i = 0; i < _capacity; i++)
    {
        _created_array[i] = rand() % 10;
    }

    struct arrayContainer arrayInfo = {_created_array, _capacity, dimension_length};

    return arrayInfo;
}

struct arrayContainer initializeZero(struct arrayContainer arrayInfo){
    for (int i = 0; i < arrayInfo._array_capacity; i++)
    {
        arrayInfo._array_ptr[i] = 0;
    }

    return arrayInfo;
}

struct arrayContainer uniformOne(struct arrayContainer arrayInfo){
    int _amount_to_set = arrayInfo._array_capacity / 10;
    int _spacing = arrayInfo._array_capacity / _amount_to_set;

    for (int i = 0; i < _amount_to_set; i++)
    {
        arrayInfo._array_ptr[i * _spacing] = 1;
    }

    return arrayInfo;
}

struct arrayContainer uniformRandomOne(struct arrayContainer arrayInfo, int dimensions[]){
    int _amount_to_print = arrayInfo._array_capacity / 20;
    int _spacing = arrayInfo._array_capacity / _amount_to_print;
    int _random_index = rand() % dimensions[0];
    int * _dimension_capacity_array =(int*)malloc(arrayInfo._array_capacity * sizeof(int));
    int * _array_coordinates =(int*)malloc(arrayInfo._array_capacity * sizeof(int));
    int _dimension_capacity = 1;

    // Calculates max indices per each dimension increment
    for (int i = 0; i < arrayInfo._number_of_dimensions; i++)
    {
        _dimension_capacity_array[i] = _dimension_capacity * dimensions[i];
        _dimension_capacity = _dimension_capacity * dimensions[i];
    }

    // Initialise coord array
    for (int i = 0; i < arrayInfo._number_of_dimensions; i++)
    {
        _array_coordinates[i] = 0;
    }

    // Calculate coords based on index from 1D array
    printf("Format = [coords] : [value] \n");
    for (int i = 0; i < _amount_to_print; i++)
    {
        int _target_index = _random_index + i * _spacing;

        for (int j = arrayInfo._number_of_dimensions; j > 0; j--)
        {
            if(_target_index / _dimension_capacity_array[j-1] > 0)
            {
                _array_coordinates[j] = _target_index / _dimension_capacity_array[j-1];
                _target_index = _target_index - (_array_coordinates[j] * _dimension_capacity_array[j-1]);
            }
        }

        _array_coordinates[0] = _target_index;

        printf("[ ");
        for (int k = 0; k < arrayInfo._number_of_dimensions; k++)
        {
            printf("%d ", _array_coordinates[k]);
        }

        printf(" ] : [ %d ]", arrayInfo._array_ptr[_random_index + i * _spacing]);

        for (int l = 0; l <= arrayInfo._number_of_dimensions; l++)
        {
            _array_coordinates[l] = 0;
        }
        printf("\n");
    }

    return arrayInfo;
}

int main() {
        int _dimensions[]={5,5,5};
        struct arrayContainer _generated_array = generateArray(_dimensions, sizeof(_dimensions)/sizeof(_dimensions[0]));
        _generated_array = initializeZero(_generated_array);
        _generated_array = uniformOne(_generated_array);
        _generated_array = uniformRandomOne(_generated_array, _dimensions);
}