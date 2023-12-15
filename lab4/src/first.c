#include "lib.h"

// Подсчет интеграла методом прямоугольников.
float SinIntegral(float A, float B, float e){
    float S = 0;
    int n = (B - A) / e;
    for(int i = 0; i < n; i++){
        float x = A + i * e;

        S = S + e * sinf(x);
    }
    return S;
}
// Пузырьковая сортировка.
void swap(int *arr, int i, int j){
    int tmp = arr[i];
    arr[i]=arr[j];
    arr[j]=tmp;
}
void Sort(int* arr, int n){
    for(int i = 0; i < n; ++i){
        int changed = 0;
        for(int j = 0; j < n - 1; ++j){
            if(arr[j] > arr[j+1]){
                swap(arr, j, j+1);
                changed = 1;
            }
        }
        if(!changed) break;
    }
}

// int main(){
//     float A = 0, B = 2, e = 0.1;
//     printf("%f\n", SinIntegral(A, B, e));
//     int arr[12] = {20, 10, 0, 1, 7, 10, 0, 0, 3, 30, 22, 10};
//     Sort(arr, 12);
    // for(int i = 0; i < 12; ++i){
    //     printf("%d ", arr[i]);
    // }
//     return 0;
// }
