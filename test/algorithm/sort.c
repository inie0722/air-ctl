#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "CTL/algorithm/intro_sort.h"
#include "CTL/algorithm/tim_sort.h"

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
    int *intro_sort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);
    int *tim_sort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);
    int *qsort_buf = (int *)malloc(sizeof(int) * BUF_SIZE);

    for (size_t i = 0; i < BUF_SIZE; i++)
    {
        int data = i;
        intro_sort_buf[i] = data;
        tim_sort_buf[i] = data;
        qsort_buf[i] = data;
    }

    for (size_t i = BUF_SIZE - BUF_SIZE / 10; i < BUF_SIZE; i++)
    {
        int data = rand() % BUF_SIZE;

        intro_sort_buf[i] = data;
        tim_sort_buf[i] = data;
        qsort_buf[i] = data;
    }

    clock_t time;
    time = clock();
    CTL_intro_sort(intro_sort_buf, intro_sort_buf + BUF_SIZE, sizeof(int), compare);
    printf("intro_sort: %ld us\n", clock() - time);

    time = clock();
    CTL_tim_sort(tim_sort_buf, tim_sort_buf + BUF_SIZE, sizeof(int), compare);
    printf("tim_sort: %ld us\n", clock() - time);

    time = clock();
    qsort(qsort_buf, BUF_SIZE, sizeof(int), Q_compare);
    printf("qsort: %ld us\n", clock() - time);

    for (size_t i = 0; i < BUF_SIZE; i++)
    {
        if (intro_sort_buf[i] != qsort_buf[i])
        {
            printf("Error: intro_sort\n");
            break;
        }

        if (tim_sort_buf[i] != qsort_buf[i])
        {
            printf("Error: tim_sort\n");
            break;
        }
    }

    free(intro_sort_buf);
    free(tim_sort_buf);
    free(qsort_buf);

    return 0;
}