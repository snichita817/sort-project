#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cstring>
using namespace std;
using namespace std::chrono;

ifstream fcin("date.in");

int arrMain[100000000], arr_copy[100000000];
int output[100000000], L[50000001], M[50000001], countR[10000001];
const int RUN = 32;

// Function that returns maximum from the array
int getMax(int arr[], int n) {
    int maxi = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > maxi)
            maxi = arr[i];
    return maxi;
}

// We use counting sort to sort the elements of an arr based on the current digit of a number
void countingRadix(int arr[], int size, int place) {
    const int maxi = 10; // numbers 0 to 9

    for (int i = 0; i < maxi; ++i)
        countR[i] = 0;

    // Frequency array for every digit
    for (int i = 0; i < size; i++)
        countR[(arr[i] / place) % 10]++;

    // Calculate cumulative count
    for (int i = 1; i < maxi; i++)
        countR[i] += countR[i - 1];

    // Place the elements in sorted order
    for (int i = size - 1; i >= 0; i--)
    {
        //output[count[arr[i]]-1] = arr[i];
        output[countR[(arr[i] / place) % 10] - 1] = arr[i];
        countR[(arr[i] / place) % 10]--;
    }

    // Copy elements to the original array
    for (int i = 0; i < size; i++)
        arr[i] = output[i];

}

// Main function of RadixSort
void radixsort(int arr[], int size) {

    int maxi = getMax(arr, size);

    // Apply counting sort to sort elements based on place value.
    for (int place = 1; maxi / place > 0; place *= 10)
        countingRadix(arr, size, place);
}

/******                                  MERGE SORT                                  ******/
// Merge two subarrays L and M into arr
void merge(int arr[], int p, int q, int r) {

    int n1 = q - p + 1;
    int n2 = r - q;

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];\

    // i, j indexes of subarrays, k index for the main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Pick the smaller element and insert in the main array
    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    // Insert the remaining elements in the array (if any left)
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = M[j];
        j++;
        k++;
    }
}

// Divide the arr into two subarrays, sort them and merge them
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // m is the point where the arr is divided into two subarrays
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/******                                  SHELL SORT                                  ******/
void shellSort(int arr[], int n) {
    // Rearrange elements at n/2, n/4, n/8 ... intervals (Shell original sequence)
    for (int in = n / 2; in > 0; in /= 2)
    {
        for (int i = in; i < n; i += 1)
        {
            int temp = arr[i];
            int j;
            for (j = i; j >= in && arr[j - in] > temp; j -= in)
            {
                arr[j] = arr[j - in];
            }
            arr[j] = temp;
        }
    }
}
/******                                  HEAP SORT                                   ******/

// Function that transforms heap to a max heap
void heapify(int arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1; // odd elements
    int right = 2 * i + 2; // even elements

    // Find the largest element among the root, left and right child
    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If root is not the largest, swap element with the root and continue heapifying
    if (largest != i) {
        int aux = arr[i];
        arr[i] = arr[largest];
        arr[largest] = aux;
        heapify(arr, n, largest);
    }
}

// Main function to do heap sort
void heapSort(int arr[], int n) {

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        int aux = arr[0];
        arr[0] = arr[i];
        arr[i] = aux;

        // Heapify root element to get highest element at root again
        heapify(arr, i, 0);
    }
}

/******                                  SELECTION SORT                      ******/
void selectionSort(int array[], int size) {
    for (int step = 0; step < size - 1; step++) {
        int mini = step;
        for (int i = step + 1; i < size; i++) {
            // Select the minimum element in each loop.
            if (array[i] < array[mini])
                mini = i;
        }

        // put min at the correct position
        swap(array[mini], array[step]);
    }
}


/******                                   COUNTING SORT           ******/
void countSort(int array[], int size) {
    // The size of count must be at least the (max+1) but
    // we cannot assign declare it as int count(max+1) in C++ as
    // it does not support dynamic memory allocation.
    // So, its size is provided statically.
    //int maxi = array[0];

    // Find the largest element in the array
    int maxi = getMax(array, size);

    //// Find the largest element of the array
    //for (int i = 1; i < size; i++) {
    //    if (array[i] > maxi)
    //        maxi = array[i];
    //}

    // Initialize count array with all zeros.
    for (int i = 0; i <= maxi; ++i) {
        countR[i] = 0;
    }

    // Store the count of each element
    for (int i = 0; i < size; i++) {
        countR[array[i]]++;
    }

    // Store the cummulative count of each array
    for (int i = 1; i <= maxi; i++) {
        countR[i] += countR[i - 1];
    }

    // Find the index of each element of the original array in count array, and
    // place the elements in output array
    for (int i = size - 1; i >= 0; i--) {
        output[countR[array[i]] - 1] = array[i];
        countR[array[i]]--;
    }

    // Copy the sorted elements into original array
    for (int i = 0; i < size; i++) {
        array[i] = output[i];
    }
}


// Print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

// array generator
void arrGen(int n, int t)
{
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        arrMain[i] = rand() % t;
}

// make a copy for the array
void arrCopy(int n)
{
    for (int i = 0; i < n; i++)
        arr_copy[i] = arrMain[i];
}

bool verifySort(int n)
{
    for (int i = 0; i < n - 1; i++)
        if (arr_copy[i] > arr_copy[i + 1])
            return false;
    return true;
}

// Used for lambda expression to sort array in reverse order
bool isGreater(int i, int j)
{
    return i > j;
}

// Function that contains all the sorting algorithms I chose.
void sortAlg(int n)
{
    arrCopy(n);
    ///*******                                  RADIX SORT                                  *******///
    auto wcts = system_clock::now();
    radixsort(arr_copy, n);
    duration<double> wctduration = (system_clock::now() - wcts);
    double dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    radixsort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    radixsort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    //double dur = wctduration.count();

    if (verifySort(n))
        cout << "RadixSort: " << fixed << setprecision(8) << dur / 3 << " seconds." << endl;
    else
    {
        cout << "RadixSort failed. Exiting application...";
        //break;
    }
    ///*******                                  #########                                   *******///
    arrCopy(n);
    ///*******                                  MERGE SORT                                  *******///
    wcts = system_clock::now();
    mergeSort(arr_copy, 0, n - 1);
    wctduration = (system_clock::now() - wcts);
    dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    mergeSort(arr_copy, 0, n - 1);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    mergeSort(arr_copy, 0, n - 1);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    if (verifySort(n))
        cout << "MergeSort: " << fixed << setprecision(8) << dur / 3 << " seconds." << endl;
    else
    {
        cout << "MergeSort failed. Exiting application...";
        //break;
    }

    ///*******                                  #########                                   *******///

    arrCopy(n);
    ///*******                                  SHELL SORT                                  *******///
    wcts = system_clock::now();
    shellSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    shellSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    shellSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();


    if (verifySort(n))
        cout << "ShellSort: " << fixed << setprecision(8) << dur / 3 << " seconds." << endl;
    else
    {
        cout << "ShellSort failed. Exiting application...";
        //break;
    }
    ///*******                                  #########                                   *******///

    arrCopy(n);
    ///*******                                  HEAP SORT                                   *******///
    wcts = system_clock::now();
    heapSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    heapSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    heapSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();


    if (verifySort(n))
        cout << "HeapSort: " << fixed << setprecision(8) << dur / 3 << " seconds." << endl;
    else
    {
        cout << "HeapSort failed. Exiting application...";
        //break;
    }
    ///*******                                  #########                                   *******///

    arrCopy(n);
    ///*******                                  COUNTING SORT                                    *******///
    wcts = system_clock::now();
    countSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    countSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    countSort(arr_copy, n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    if (verifySort(n))
        cout << "CountingSort: " << fixed << setprecision(8) << dur/3 << " seconds." << endl;
    else
    {
        cout << "CountingSort failed. Exiting application...";
        //break;
    }
    ///*******                                  #########                                   *******///

    arrCopy(n);
    ///*******                                  C++ SORT                                    *******///
    wcts = system_clock::now();
    sort(arr_copy, arr_copy + n);
    wctduration = (system_clock::now() - wcts);
    dur = wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    sort(arr_copy, arr_copy + n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();

    arrCopy(n);
    wcts = system_clock::now();
    sort(arr_copy, arr_copy + n);
    wctduration = (system_clock::now() - wcts);
    dur += wctduration.count();


    if (verifySort(n))
        cout << "C++ sorting algorithm: " << fixed << setprecision(8) << dur/3 << " seconds." << endl;
    else
    {
        cout << "C++ sorting algorithm failed. Exiting application...";
        //break;
    }
}

int main()
{
    char line[256];
    int n = 0, t = 0;

    while (fcin >> line)
    {
        int i = 0;
        if (n == 0)
        {
            while (line[i] >= '0' && line[i] <= '9' && i < strlen(line))
            {
                n = n * 10 + (line[i] - '0');
                i += 1;
            }
        }
        else {
            if (n != 0 && t == 0)
            {
                while (line[i] >= '0' && line[i] <= '9' && i < strlen(line))
                {
                    t = t * 10 + (line[i] - '0');
                    i += 1;
                }
            }
            if (n != 0 && t != 0)
            {
                cout << "Tests for an array of length " << n << " with elements between 0 and " << t << "." << endl;
                ///*******                                RANDOMIZED ARRAY                              *******///
                arrGen(n, t);

                sortAlg(n);
                ///*******                                  #########                                   *******///

                cout << endl;

                cout << "Tests for partially sorted array:" << endl;
                partial_sort(arrMain, arrMain + (n / 2), arrMain + n);
                sortAlg(n);

                cout << endl;

                cout << "Tests for fully sorted array:" << endl;
                sort(arrMain, arrMain + n);
                sortAlg(n);

                cout << endl;

                cout << "Tests for reverse sorted array:" << endl;
                sort(arrMain, arrMain + n, isGreater);
                sortAlg(n);

                cout << endl << endl;
                cout << "__________________________________________________________________________________________________________" << endl << endl;
                n = t = 0;
            }
        }
    }

    return 0;
}
