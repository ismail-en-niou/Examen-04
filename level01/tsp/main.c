#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
// Swap two integers (used for permutations)
void swap_f(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

float distance(float a[2], float b[2]) {
    return sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}


void backtrack(float (*point)[2] , int size , int index , int *perm , float *min_dis)
{
    if (size == index)
    {
        float min = 0.0f;
        for (int i = 0 ; i < size - 1; i++)
            min+= distance(point[perm[i]] , point[perm[i + 1]]);
        min+= distance(point[perm[0]] , point[perm[size - 1]]);
        if (min < *min_dis)
            *min_dis = min;
        return;
    }
    for (int i = index ; i < size ; i++)
    {
        swap_f(&perm[index] , &perm[i]);
        backtrack(point , size , index + 1 , perm , min_dis);
        swap_f(&perm[index] , &perm[i]);
    }
}

float tsp(float (*points)[2], int size) 
{
    float   min_dist = INFINITY;
    int *perm = malloc(sizeof(int) * size);
    for (int i= 0 ; i < size ;i++)
        perm[i] = i;
    backtrack(points , size , 0 , perm , &min_dist);
    return min_dist;
}

// Count number of valid lines in file
ssize_t file_size(FILE *file) {
    char *buffer = NULL;
    size_t n = 0;
    ssize_t ret = 0;

    errno = 0;
    while (getline(&buffer, &n, file) != -1)
        ret++;

    free(buffer);
    if (errno || fseek(file, 0, SEEK_SET))
        return -1;
    return ret;
}

// Read coordinates from file
int retrieve_file(float (*array)[2], FILE *file) {
    int tmp;
    for (size_t i = 0; (tmp = fscanf(file, "%f, %f\n", &array[i][0], &array[i][1])) != EOF; i++) {
        if (tmp != 2) {
            errno = EINVAL;
            return -1;
        }
    }
    if (ferror(file))
        return -1;
    return 0;
}

int main(int ac, char **av) {
    char *filename = "stdin";
    FILE *file = stdin;

    if (ac > 1) {
        filename = av[1];
        file = fopen(filename, "r");
    }

    if (!file) {
        fprintf(stderr, "Error opening %s: %m\n", filename);
        return 1;
    }

    ssize_t size = file_size(file);
    if (size == -1) {
        fprintf(stderr, "Error reading %s: %m\n", filename);
        fclose(file);
        return 1;
    }

    float (*array)[2] = calloc(size, sizeof(float[2]));
    if (!array) {
        fprintf(stderr, "Error allocating memory: %m\n");
        fclose(file);
        return 1;
    }

    if (retrieve_file(array, file) == -1) {
        fprintf(stderr, "Error reading %s: %m\n", filename);
        free(array);
        if (file != stdin)
            fclose(file);
        return 1;
    }

    if (file != stdin)
        fclose(file);

    printf("%.2f\n", tsp(array, size));
    free(array);
    return 0;
}
