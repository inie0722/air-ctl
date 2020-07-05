#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "in_sort.h"

bool compare(const void *a, const void *b)
{
    return *(int *)a < *(int *)b;
}

int Q_compare(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

#define BUF_SIZE 1000 * 10000

int main(void)
{
    int *in_sort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);
    int *in_stable_sort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);
    int *qsort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);

    for (size_t i = 0; i < BUF_SIZE; i++)
    {
        int data = i;
        in_sort_buf[i] = data;
        in_stable_sort_buf[i] = data;
        qsort_buf[i] = data;
    }

    for (size_t i = BUF_SIZE - BUF_SIZE / 10; i < BUF_SIZE; i++)
    {
        int data = rand() % BUF_SIZE;

        in_sort_buf[i] = data;
        in_stable_sort_buf[i] = data;
        qsort_buf[i] = data;
    }

    clock_t time;
    time = clock();
    in_sort(in_sort_buf, in_sort_buf + BUF_SIZE, sizeof(int), compare);
    printf("in_sort: %ld us\n", clock() - time);

    time = clock();
    in_stable_sort(in_stable_sort_buf, in_stable_sort_buf + BUF_SIZE, sizeof(int), compare);
    printf("in_stable_sort: %ld us\n", clock() - time);

    time = clock();
    qsort(qsort_buf, BUF_SIZE, sizeof(int), Q_compare);
    printf("qsort: %ld us\n", clock() - time);

    for (size_t i = 0; i < BUF_SIZE; i++)
    {
        if (in_sort_buf[i] != qsort_buf[i])
        {
            printf("Error: in_sort\n");
            break;
        }

        if (in_stable_sort_buf[i] != qsort_buf[i])
        {
            printf("Error: in_stable_sort\n");
            break;
        }
    }

    free(in_sort_buf);
    free(in_stable_sort_buf);
    free(qsort_buf);

    return 0;
}