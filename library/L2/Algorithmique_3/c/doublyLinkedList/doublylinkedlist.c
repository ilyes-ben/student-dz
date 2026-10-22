#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    struct node *prev;
    int data;
    struct node *next;
}node;

node* createNode(int value){
node *temp = malloc(sizeof(node));
temp->prev = NULL;
temp->data = value;
temp->next = NULL;
return temp;
}


node *temp = createNode(value);
if (*head == NULL) {  // Handle empty list
        *head = temp;
        return;
    }
temp->next = *head;
(*head)->prev = temp;
*head = temp;
}

void insertAtBeginning(node **head, int value) {
    node *temp = createNode(value);
    if (*head == NULL) {
        *head = temp;
        return;
    }

    temp->next = *head;
    (*head)->prev = temp;
    *head = temp;
}


void insertAtEnd(node *head, int value){
node *th = head;
node *temp = createNode(value);
while(th->next != NULL){
    th = th->next;
}
th->next = temp;
temp->prev = th;
}

void insertAtPosition(node **head, int position, int value){
node *temp = createNode(value);
node *th = *head;

 if (position <= 0) {  // Invalid position
        printf("Invalid position!\n");
        return;
    }


if(position == 1){
    insertAtBeginning(&head, value);
    return;
}

while(position != 1){
    th = th->next;
    position--;
}

if(th->next == NULL){
    th->next = temp;
    temp->prev = th;
    return;
}

temp->prev = th->prev;
temp->next = th;
th->prev = temp;
}


void deleteFirstNode(node **head){
    *head = (*head)->next;
    free(*(head)->prev);
    *(head)->prev = NULL;
}

void deleteLastNode(node *head){
    node *temp = head;
    node *temp2 = NULL;
    while(temp->next != NULL){
        temp2 = temp;
        temp = temp->next;
    }
    temp2->next = NULL;
    free(temp);
    temp = NULL
}

void deleteAtPosition(node *head, int position){
    node *temp = head;

    if(position == 1)deleteFirstNode(head);

    while(position != 1){
        temp = temp->next;
        position--;
    }

    if(temp->next == NULL){
        deleteLastNode(head);
    }

    //method 1
    node *prev = temp->prev;
    node *next = temp->next;
    prev->next = next;
    next->prev = prev;
    free(temp)
    temp = NULL;

    //method 2
    /*
    node *temp2 = temp->prev;
    temp2->next = temp->next;
    temp->next->prev = temp2;
    free(temp);
    temp = NULL;
    */ 
}

void main(){
node *head = createNode(1);
insertAtBeginning(&head,0);
insertAtEnd(head,2);
insertAtPosition(head, 2 , 100);


// Print list
node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

  // Free memory
    while (head != NULL) {
        node *temp = head;
        head = head->next;
        free(temp);
    }

}