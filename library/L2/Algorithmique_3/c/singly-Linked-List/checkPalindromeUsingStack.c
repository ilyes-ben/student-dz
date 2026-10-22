#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define the node structure
typedef struct node {
    char data;
    struct node *link;
} node;

// Function to create a new node
node *createNode(char data) {
    node *newNode = malloc(sizeof(node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->link = NULL;
    return newNode;
}

// Function to push an element onto the stack
void push(node **top, char data) {
    node *temp = createNode(data);
    temp->link = *top;
    *top = temp;
}

// Function to pop an element from the stack
char pop(node **top) {
    if (*top == NULL) {
        printf("Stack underflow\n");
        exit(1);
    }
    node *temp = *top;
    char value = (*top)->data;
    *top = (*top)->link;
    free(temp);
    return value;
}

void printStack(node *top) {
    node *temp = top;
    while (temp != NULL) {
        printf("%c\n", temp->data);
        temp = temp->link;
    }
}

bool checkPalindrome(node **top) {
    node *stack1 = NULL;
    node *stack2 = NULL;
    node *temp = *top;
    
    // Push elements to stack1 until 'X' is encountered
    while (temp != NULL && temp->data != 'X') {
        push(&stack1, temp->data);
        temp = temp->link;
    }

    // Move past 'X'
    if (temp != NULL) {
        temp = temp->link;
    }

    // Push remaining elements to stack2
    while (temp != NULL) {
        push(&stack2, temp->data);
        temp = temp->link;
    }

    // Check if both stacks are equal
    while (stack1 != NULL && stack2 != NULL) {
        char value1 = pop(&stack1);
        char value2 = pop(&stack2);
        if (value1 != value2) {
            return false;
        }
    }

    // If both stacks are empty, it's a palindrome
    return true;
}



int main() {
    node *top = NULL;

    // Push elements onto the stack to form "ababXbaba"
    push(&top, 'a');
    push(&top, 'b');
    push(&top, 'a');
    push(&top, 'b');
    push(&top, 'X');
    push(&top, 'a');
    push(&top, 'b');
    push(&top, 'a');
    push(&top, 'b');

    printf("Original stack:\n");
    printStack(top);

    bool isPalindrome = checkPalindrome(top);

    if (isPalindrome) {
        printf("The string is a palindrome\n");
    } else {
        printf("The string is not a palindrome\n");
    }

    return 0;
}