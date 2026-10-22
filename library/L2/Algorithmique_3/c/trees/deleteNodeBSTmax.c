#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definition of Node for Binary Search Tree
struct BstNode {
    int data;
    struct BstNode* left;
    struct BstNode* right;
};

// Function to create a new node in heap
struct BstNode* GetNewNode(int data) {
    struct BstNode* newNode = (struct BstNode*)malloc(sizeof(struct BstNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// To insert data in BST, returns the address of the root node
struct BstNode* Insert(struct BstNode* root, int data) {
    if (root == NULL) { // empty tree
        root = GetNewNode(data);
    }
    // if data to be inserted is lesser, insert in left subtree
    else if (data <= root->data) {
        root->left = Insert(root->left, data);
    }
    // else, insert in right subtree
    else {
        root->right = Insert(root->right, data);
    }
    return root;
}

// Function to find the maximum value node in the BST
struct BstNode* FindMax(struct BstNode* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

// To search an element in BST, returns true if element is found
bool Search(struct BstNode* root, int data) {
    if (root == NULL) {
        return false;
    } else if (root->data == data) {
        return true;
    } else if (data <= root->data) {
        return Search(root->left, data);
    } else {
        return Search(root->right, data);
    }
}

// Function to delete a node in the BST
struct BstNode* Delete(struct BstNode* root, int data) {
    if (root == NULL) return root;
    
    if (data < root->data) {
        root->left = Delete(root->left, data);
    }
    else if (data > root->data) {
        root->right = Delete(root->right, data);
    }
    else {
        // Node to be deleted is found
        if (root->left == NULL && root->right == NULL) {
            free(root); // No child node
            root = NULL;
        }
        else if (root->left == NULL) {
            struct BstNode* temp = root;
            root = root->right; // One child (right child)
            free(temp);
        }
        else if (root->right == NULL) {
            struct BstNode* temp = root;
            root = root->left; // One child (left child)
            free(temp);
        }
        else {
            // Node with two children, get the maximum in the left subtree
            struct BstNode* temp = FindMax(root->left); // Find the maximum in the left subtree
            root->data = temp->data; // Copy the maximum value
            root->left = Delete(root->left, temp->data); // Delete the maximum node
        }
    }
    return root;
}

int main() {
    struct BstNode* root = NULL;  // Creating an empty tree

    // Code to test the logic
    root = Insert(root, 15);
    root = Insert(root, 10);
    root = Insert(root, 20);
    root = Insert(root, 25);
    root = Insert(root, 8);
    root = Insert(root, 12);

    // Ask user to enter a number
    int number;
    printf("Enter number to be searched: ");
    scanf("%d", &number);

    // If number is found, print "FOUND"
    if (Search(root, number)) {
        printf("Found\n");
    } else {
        printf("Not Found\n");
    }

    // Delete a node and print result (to test FindMax in delete)
    root = Delete(root, 10);
    printf("After deleting node 10:\n");
    
    // Ask user to enter a number again after deletion
    printf("Enter number to be searched: ");
    scanf("%d", &number);

    // If number is found after deletion, print "FOUND"
    if (Search(root, number)) {
        printf("Found\n");
    } else {
        printf("Not Found\n");
    }

    return 0;
}
