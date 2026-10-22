#include <stdio.h>

// Part 1 & 2: Algorithm Analysis
/* 
After first iteration [6,1,4,5,2,3] → [1,6,4,5,2,3]
This must be Selection Sort because:
- Selection sort finds minimum (1) and swaps with first position
- Answer to Part 2 would be: a) [1,2,6,4,5,3]
*/

// Part 3: Implementation

// Function to check if array is sorted in descending order
int Tab_Sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            return 0;  // Not sorted in descending order
        }
    }
    return 1;  // Sorted in descending order
}

// Selection sort in descending order
void Selection_Sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        // Find maximum element in unsorted portion
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[max_idx]) {
                max_idx = j;
            }
        }
        // Swap with first element of unsorted portion
        if (max_idx != i) {
            int temp = arr[i];
            arr[i] = arr[max_idx];
            arr[max_idx] = temp;
        }
    }
}

// Binary search for descending-sorted array
int Binary_Search(int arr[], int n, int value) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        if (arr[mid] == value) {
            return 1;  // Value found
        }
        if (arr[mid] > value) {
            left = mid + 1;  // Search right half
        } else {
            right = mid - 1;  // Search left half
        }
    }
    return 0;  // Value not found
}

// Main program
int main() {
    int n;
    
    // Get array size
    printf("Enter array size: ");
    scanf("%d", &n);
    
    // Create and fill array
    int arr[100];  // Assuming maximum size of 100
    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Check if array is sorted in descending order
    if (!Tab_Sort(arr, n)) {
        printf("Array is not sorted. Sorting now...\n");
        Selection_Sort(arr, n);
        
        printf("Sorted array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    
    // Perform binary search
    int value;
    printf("Enter value to search: ");
    scanf("%d", &value);
    
    if (Binary_Search(arr, n, value)) {
        printf("%d is found in the array\n", value);
    } else {
        printf("%d is not found in the array\n", value);
    }
    
    return 0;
}