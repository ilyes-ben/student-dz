#include <stdio.h>
#include <stdlib.h>

// insertion, selction, bubble sort using linked list

struct Node {
    int data;
    struct Node* next;
};

// Helper function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Helper function to print the linked list
void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void bubbleSort(struct Node* head) {
    int swapped;
    struct Node *ptr1, *lptr = NULL;

    if (head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data > ptr1->next->data) {
                // Swap the data
                int temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void bubbleSortSimple(struct Node* head) {
    if (head == NULL) return;

    struct Node* current;
    int swapped;

    do {
        swapped = 0;
        current = head;

        while (current->next != NULL) {
            if (current->data > current->next->data) {
                // Swap the data
                int temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}



void selectionSort(struct Node* head) {
    struct Node *temp1 = head;

    while (temp1 != NULL) {
        struct Node *min = temp1;
        struct Node *temp2 = temp1->next;

        while (temp2 != NULL) {
            if (temp2->data < min->data) {
                min = temp2;
            }
            temp2 = temp2->next;
        }

        // Swap the data
        int temp = temp1->data;
        temp1->data = min->data;
        min->data = temp;

        temp1 = temp1->next;
    }
}

struct Node* insertionSort(struct Node* head) {
    struct Node* sorted = NULL;

    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = current->next;

        if (sorted == NULL || sorted->data >= current->data) {
            current->next = sorted;
            sorted = current;
        } else {
            struct Node* temp = sorted;
            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next;
    }
    return sorted;
}


int main() {
    struct Node* head = createNode(4);
    head->next = createNode(3);
    head->next->next = createNode(1);
    head->next->next->next = createNode(5);
    head->next->next->next->next = createNode(2);

    printf("Original List:\n");
    printList(head);

    // Bubble Sort
    bubbleSort(head);
    printf("\nList after Bubble Sort:\n");
    printList(head);

    // Reset list for another sorting example
    head = createNode(4);
    head->next = createNode(3);
    head->next->next = createNode(1);
    head->next->next->next = createNode(5);
    head->next->next->next->next = createNode(2);

    // Selection Sort
    selectionSort(head);
    printf("\nList after Selection Sort:\n");
    printList(head);

    // Reset list for another sorting example
    head = createNode(4);
    head->next = createNode(3);
    head->next->next = createNode(1);
    head->next->next->next = createNode(5);
    head->next->next->next->next = createNode(2);

    // Insertion Sort
    head = insertionSort(head);
    printf("\nList after Insertion Sort:\n");
    printList(head);

    return 0;
}
