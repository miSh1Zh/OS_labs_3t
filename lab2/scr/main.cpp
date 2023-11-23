#include <iostream>
#include <vector>

using namespace std;

const int RUN = 32;

vector<int> globe_vector(0);
pthread_mutex_t mut;

using partition = pair<int, int>;
typedef struct{
    int l;
    int m;
    int r;
}triple_partition;


// Utility function to print the Array
void printArray(vector<int>& arr)
{
    for (int elem : arr)
        printf("%d ", elem);
    printf("\n");
}

// This function sorts array from left
// index to to right index which is
// of size atmost RUN
// void* insertionSort(void* param)
void insertionSort(int left, int right)
{
    for (int i = left + 1; i <= right; i++) {
        int temp = globe_vector[i];
        int j = i - 1;
        while (j >= left && globe_vector[j] > temp) {
            globe_vector[j + 1] = globe_vector[j];
            j--;
        }
        globe_vector[j + 1] = temp;
    }
}
void* inS(void* part)
{
    int left = ((partition*)part)->first;
    int right = ((partition*)part)->second;
    insertionSort(left, right);
    pthread_exit(0);
    return 0;
}

// Merge function merges the sorted runs
void merge(int l, int m, int r)
{
    // Original array is broken in two
    // parts left and right array
    int len1 = m - l + 1, len2 = r - m;
    int left[len1], right[len2];
    for (int i = 0; i < len1; i++)
        left[i] = globe_vector[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = globe_vector[m + 1 + i];

    int i = 0;
    int j = 0;
    int k = l;

    // After comparing, we merge those two array in larger sub array
    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) {
            globe_vector[k] = left[i];
            i++;
        }
        else {
            globe_vector[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left, if any
    while (i < len1) {
        globe_vector[k] = left[i];
        k++;
        i++;
    }

    // Copy remaining element of right, if any
    while (j < len2) {
        globe_vector[k] = right[j];
        k++;
        j++;
    }
}

void* mr(void* part){
    int l = ((triple_partition*)part)->l;
    int m = ((triple_partition*)part)->m;
    int r = ((triple_partition*)part)->r;
    merge(l, m, r);
    pthread_exit(0);
    return 0;
}

// Iterative Timsort function to sort the
// array[0...n-1] (similar to merge sort)
void timSort(int thread_count)
{
    int n = globe_vector.size();

    vector<partition> to_take(0);

    // Sort individual subarrays of size RUN
    for (int i = 0; i < n; i += RUN){
        // insertionSort(i, min((i + RUN - 1), (n - 1)));

        to_take.push_back(partition(i, min((i + RUN - 1), (n - 1))));

        // cout << "\nProcess: "; printArray(globe_vector); cout << '\n';
    }

    pthread_t tid[thread_count];
    int count = 0;

    int n_parts = to_take.size();
    for(int i = 0; i < n_parts; ++i){
        if(count == thread_count){
            while(count){
                pthread_join(tid[thread_count - count], NULL);
                count--;
            }
        }
        pthread_create(&tid[count], NULL, inS, &(to_take[i]));
        ++count;
    }
    for(int i = 0; i < count; ++i){
        pthread_join(tid[i], NULL);
    }
    count = 0;

    // Start merging from size RUN (or 32).
    // It will merge
    // to form size 64, then 128, 256
    // and so on ....
    for (int size = RUN; size < n; size = 2 * size) {

        // pick starting point of
        // left sub array. We
        // are going to merge
        // arr[left..left+size-1]
        // and arr[left+size, left+2*size-1]
        // After every merge, we
        // increase left by 2*size
        for (int left = 0; left < n; left += 2 * size) {

            // Find ending point of
            // left sub array
            // mid+1 is starting point
            // of right sub array
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));

            // merge sub array arr[left.....mid] &
            // arr[mid+1....right]
            if (mid < right){
                merge(left, mid, right);
            }
        }
    }
}


int main(int argc, char* argv[])
{
    // check correct key
    if (argc != 2) {
        perror("\nError: no threads to use\n");
        exit(EXIT_FAILURE);
    }
    if(atoi(argv[1]) < 1){
        perror("\nError: Wrong count (try  'integer'>=1)\n");
        exit(EXIT_FAILURE);
    }
    const int thread_count = atoi(argv[1]);

    // Input array
    int tmp;
    printf("Type your array: ");
    while(scanf("%d", &tmp) > 0){
        globe_vector.push_back(tmp);
    }

    // Function Call
    timSort(thread_count);

    // Results
    // printf("\nAfter Sorting globe Array is: ");
    // printArray(globe_vector);
    printf("\nArray size was %d\n", globe_vector.size());
    return 0;
}