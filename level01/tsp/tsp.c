#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Swap two integers (used for permutations)
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Compute the Euclidean distance between two points
float distance(float a[2], float b[2]) {
    return sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}

// Recursive permutation-based TSP
void tsp_recursive(float (*points)[2], int *tab, int start, int size, float *min_dist) 
{
    if (start == size)
    {
        float dist = 0.0;
        for (int i = 0; i < size - 1; i++) {
            dist += distance(points[tab[i]], points[tab[i + 1]]);
        }
        dist += distance(points[tab[size - 1]], points[tab[0]]); // return to start
        if (dist < *min_dist)
            *min_dist = dist;
        return;
    }
    for (int i = start ; i < size ; i++)
    {
        swap(&tab[start] , &tab[i]);
        tsp_recursive(points ,tab , start + 1 , size , min_dist);
        swap(&tab[start] , &tab[i]);
    }
}


// Entry point for TSP algorithm
float tsp(float (*points)[2], int size) {
    int *perm = malloc(sizeof(int) * size);
    if (!perm) return -1.0f;
    for (int i = 0; i < size; i++)
        perm[i] = i;
    float min_dist = INFINITY;
    tsp_recursive(points, perm, 0, size, &min_dist);
    free(perm);
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
