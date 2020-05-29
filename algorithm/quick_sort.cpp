#include <stdio.h>

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) 
{
    int pivot = high;
    int i = (low - 1);

    for (int j = low; j < high; j++) 
    {
        if (arr[j] < arr[pivot]) 
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[pivot]);
    return i + 1;
}

void quick_sort(int arr[], int low, int high) 
{
    if (low < high) 
    {
        int mid = partition(arr, low, high);
        quick_sort(arr, low, mid - 1);
        quick_sort(arr, mid + 1, high);
    }
}

int main() 
{
    int arr[100005];

    int num, stats;
    int count = 0;
    while(1) 
    {
        stats = scanf("%d", &num);
        if (stats == EOF) break;

        arr[count] = num;
        count++;
    }

    quick_sort(arr, 0, count - 1);

    for (int i = 0; i < count; i++) 
    {
        printf("%d", arr[i]);
        if (i != count - 1) printf(" ");
    }
    printf("\n");

    return 0;
}
