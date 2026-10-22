#include <stdio.h>

// Function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Bubble Sort
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++) {
            // Compare adjacent elements
            if (arr[j] > arr[j+1]) {
                // Swap if they are in wrong order
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Selection Sort
void selectionSort(int arr[], int n) {
    int i, j, min_idx, temp;
    
    for (i = 0; i < n-1; i++) {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        // Swap the found minimum element with the first element
        temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

// Insertion Sort
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        
        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}



// Function to perform binary search
int binarySearch(int arr[], int size, int target) {
    int low = 0, high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Calculate the mid index

        // Check if the target is at mid
        if (arr[mid] == target) {
            return mid; // Target found
        }

        // If target is smaller, it must be in the left half
        if (arr[mid] > target) {
            high = mid - 1;
        }
        // If target is larger, it must be in the right half
        else {
            low = mid + 1;
        }
    }

    return -1; // Target not found
}

int main() {
    int arr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23}; // Sorted array
    int size = sizeof(arr) / sizeof(arr[0]);
    int target;

    printf("Enter the number to search: ");
    scanf("%d", &target);

    int result = binarySearch(arr, size, target);

    if (result != -1) {
        printf("Element found at index %d\n", result);
    } else {
        printf("Element not found in the


int main() {
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr1) / sizeof(arr1[0]);
    
    // Bubble Sort
    printf("Original array: ");
    printArray(arr1, n);
    bubbleSort(arr1, n);
    printf("Bubble sorted array: ");
    printArray(arr1, n);
    
    // Selection Sort
    printf("\nOriginal array: ");
    printArray(arr2, n);
    selectionSort(arr2, n);
    printf("Selection sorted array: ");
    printArray(arr2, n);
    
    // Insertion Sort
    printf("\nOriginal array: ");
    printArray(arr3, n);
    insertionSort(arr3, n);
    printf("Insertion sorted array: ");
    printArray(arr3, n);
    
    return 0;
}