#include <stdio.h>
#include <stdlib.h>

// Define the node structure
typedef struct node {
    int data;
    struct node *link;
} node;

// Function to create a new node
node *createNode(int data) {
    node *newNode = malloc(sizeof(node)); // Corrected type and malloc usage
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->link = NULL; // Use NULL instead of Null
    return newNode;
}

// Function to push an element onto the stack
void push(node **top, int data) {
    node *temp = createNode(data);
    if(*top == NULL){
        *top = temp;
        return;
    }
    temp->link = *top;
    *top = temp;
}

// Function to pop an element from the stack
int pop(node **top) {
    if (*top == NULL) { // Check if stack is empty
        printf("Stack underflow\n");
        exit(1);
    }
    node *temp = *top;
    int value = (*top)->data; // Store data to return
    *top = (*top)->link; // Update the top
    free(temp); // Free the memory of the popped node
    temp = NULL;
    return value; // Return the popped value
}

void printStack(node *top) {
    if (top == NULL) { // Check if stack is empty
        printf("Stack underflow\n");
        exit(1);
    }
    node *temp = top;

    while(temp != NULL){
        printf("%d\n",temp->data);
        temp = temp->link;
    }
}

// Function to return the top element of the stack
int peek(node *top){
     if (top == NULL) { // Check if stack is empty
        printf("Stack is empty\n");
        exit(1);
    }
    return top->data;
}


void reverseStackUsingTwoStacks(node **top) {
    node *stack1 = NULL;
    node *stack2 = NULL;

    // Transfer elements from the original stack to stack1
    while (*top != NULL) {
        int value = pop(top); // Pop from the original stack
        push(&stack1, value); // Push to stack1
    }

    // Transfer elements from stack1 to stack2
    while (stack1 != NULL) {
        int value = pop(&stack1); // Pop from stack1
        push(&stack2, value);     // Push to stack2
    }

    // Transfer elements back from stack2 to the original stack
    while (stack2 != NULL) {
        int value = pop(&stack2); // Pop from stack2
        push(top, value);         // Push to the original stack
    }
}

void reverseStack(node **top) {
    if (*top == NULL || (*top)->link == NULL) {
        return; // Base case: stack is empty or has one element
    }

    int value = pop(top);           // Step 1: Pop the top element
    reverseStack(top);               // Step 2: Recursively reverse the rest of the stack
    push(top, value);              // Step 3: Insert the popped element at the bottom
}

int main() {
    node *top = NULL;

    // Push some elements onto the stack
    push(&top, 45);
    push(&top, 2);
    push(&top, 8);

    printf("Original stack:\n");
    printStack(top);

    reverseStackUsingTwoStacks(&top);

    printf("Reversed stack:\n");
    printStack(top);

    reverseStack(&top);

    printf("Reversed stack using recursion:\n");
    printStack(top);

    return 0;
}
