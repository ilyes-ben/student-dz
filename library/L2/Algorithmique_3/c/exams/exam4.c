#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Define the structure for triangle list
typedef struct ListTri {
    float seg1, seg2, seg3;  // Three segments
    char color[20];          // Color
    struct ListTri* next;    // Next pointer
} ListTri;

// Function to add triangle at list head
ListTri* Add(ListTri* head) {
    ListTri* newNode = (ListTri*)malloc(sizeof(ListTri));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return head;
    }
    
    printf("Enter triangle details:\n");
    printf("Segment 1: ");
    scanf("%f", &newNode->seg1);
    printf("Segment 2: ");
    scanf("%f", &newNode->seg2);
    printf("Segment 3: ");
    scanf("%f", &newNode->seg3);
    printf("Color: ");
    scanf("%s", newNode->color);
    
    newNode->next = head;
    return newNode;
}

// Function to delete from list head
ListTri* Delete(ListTri* head) {
    if (!head) {
        printf("List is empty!\n");
        return NULL;
    }
    
    ListTri* temp = head;
    head = head->next;
    free(temp);
    printf("Head triangle deleted.\n");
    return head;
}

// Function to display all triangles
void Display(ListTri* head) {
    if (!head) {
        printf("List is empty!\n");
        return;
    }
    
    ListTri* current = head;
    int count = 1;
    printf("\nTriangle List:\n");
    while (current) {
        printf("Triangle %d:\n", count++);
        printf("Segments: %.2f, %.2f, %.2f\n", 
               current->seg1, current->seg2, current->seg3);
        printf("Color: %s\n\n", current->color);
        current = current->next;
    }
}

// Function to count yellow rectangles
int Search(ListTri* head) {
    int count = 0;
    ListTri* current = head;
    
    while (current) {
        // Check if triangle is yellow and rectangle (one 90° angle)
        if (strcmp(current->color, "yellow") == 0) {
            // Using Pythagorean theorem to check if rectangle
            float a2 = current->seg1 * current->seg1;
            float b2 = current->seg2 * current->seg2;
            float c2 = current->seg3 * current->seg3;
            
            if (a2 + b2 == c2 || b2 + c2 == a2 || a2 + c2 == b2) {
                count++;
            }
        }
        current = current->next;
    }
    return count;
}

// Function to display isosceles triangles
void Isosceles_Rectangle(ListTri* head) {
    ListTri* current = head;
    printf("\nIsosceles Triangles:\n");
    
    while (current) {
        // Check if any two sides are equal
        if (current->seg1 == current->seg2 || 
            current->seg2 == current->seg3 || 
            current->seg1 == current->seg3) {
            printf("Segments: %.2f, %.2f, %.2f\n", 
                   current->seg1, current->seg2, current->seg3);
            printf("Color: %s\n\n", current->color);
        }
        current = current->next;
    }
}

// Function to create array of perimeters
int Triangle_Array(ListTri* head, float perimeters[], int maxSize) {
    ListTri* current = head;
    int count = 0;
    
    while (current && count < maxSize) {
        perimeters[count++] = current->seg1 + current->seg2 + current->seg3;
        current = current->next;
    }
    return count;
}

// Function to sort array (using bubble sort)
void Sort_Array(float arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    ListTri* head = NULL;
    int choice;
    float perimeters[100];  // Array for perimeters
    
    do {
        printf("\nTriangle Management System\n");
        printf("1. Add triangle\n");
        printf("2. Delete triangle\n");
        printf("3. Display all\n");
        printf("4. Count yellow rectangles\n");
        printf("5. Show isosceles triangles\n");
        printf("6. Sort perimeters\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                head = Add(head);
                break;
            case 2:
                head = Delete(head);
                break;
            case 3:
                Display(head);
                break;
            case 4:
                printf("Yellow rectangles: %d\n", Search(head));
                break;
            case 5:
                Isosceles_Rectangle(head);
                break;
            case 6: {
                int count = Triangle_Array(head, perimeters, 100);
                if (count > 0) {
                    Sort_Array(perimeters, count);
                    printf("Sorted perimeters: ");
                    for (int i = 0; i < count; i++) {
                        printf("%.2f ", perimeters[i]);
                    }
                    printf("\n");
                }
                break;
            }
            case 7:
                // Free memory before exiting
                while (head) {
                    ListTri* temp = head;
                    head = head->next;
                    free(temp);
                }
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 7);
    
    return 0;
}