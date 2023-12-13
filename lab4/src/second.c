#include "lib.h"

//Подсчет интеграла методом трапеций.
float SinIntegral(float A, float B, float e){
    int n = (B - A) / e;
    float S = 0;
    for(int i = 0; i < n; i++){
        float x1 = A + i * e;
        float x2 = A + (i + 1) * e;

        S += 0.5 * (x2 - x1) * (sinf(x1) + sinf(x2));
    }
    return S;
}
// Сортировка Хоара.
void swap(int *arr, int i, int j){
    int tmp = arr[i];
    arr[i]=arr[j];
    arr[j]=tmp;
}
int hoar_partition(int *arr, int n, int b){
    int i = 0, j = n-1;
    while (i < j){
        while (arr[i] < b) i++;
        while (arr[j] > b) j--;
        if (i < j){
            swap(arr, i, j);
            i++;
            j--;
        }
    }
    return i;
}
void Sort(int *arr, int n) {
    while (n >= 2) {
        double b = floor((arr[0]+arr[n-1])/2);
        int i = hoar_partition(arr, n, b);
        Sort(arr, i);
        arr = &arr[i];
        n = n - i;
    }
}

// int main(){
//     float A = 0, B = 2, e = 0.1;
//     printf("%f\n", SinIntegral(A, B, e));
//     int arr[12] = {20, 10, 0, 1, 7, 10, 0, 0, 3, 30, 22, 10};
//     Sort(arr, 12);
//     for(int i = 0; i < 12; ++i){
//         printf("%d ", arr[i]);
//     }
//     return 0;
// }
