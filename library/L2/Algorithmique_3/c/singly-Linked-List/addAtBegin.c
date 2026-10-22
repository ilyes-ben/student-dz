#include <stdio.h>
#include <stdlib.h>

at_beg(node *head, int data);

typedef struct node {
    int data;
    struct node *link;
}node;

int main(void){
node *head = malloc(sizeof(node));
head->data = 45;
head->link = NULL;

node *ptr = malloc(sizeof(node));
ptr->data = 100;
ptr->link = NULL;

head->link = ptr;

int data = 3;

head = at_beg(head, data);
ptr = head;

while(ptr != NULL){
    printf("%d",ptr->data);
    ptr = ptr->link; 
}

return 0;
}

at_beg(node *head, int data){
    node *ptr = malloc(sizeof(node));
    ptr-data = data;
    ptr-link = NULL;

    ptr->link = head;
    head = ptr;
    return head

}
