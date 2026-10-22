#include <stdio.h>
#include <stdlib.h>

// Define the structure for a BST node
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

// Function to find the minimum value node in a tree
struct Node* FindMin(struct Node* root) {
    while (root != NULL && root->left != NULL)
        root = root->left;
    return root;
}

// Function to delete a value from the tree
struct Node* Delete(struct Node* root, int data) {
    if (root == NULL) return root;  // If tree is empty
    else if (data < root->data) root->left = Delete(root->left, data);  // Search left subtree
    else if (data > root->data) root->right = Delete(root->right, data);  // Search right subtree
    else {
        // Node to be deleted found
        // Case 1: No child
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
        // Case 2: One child
        else if (root->left == NULL) {
            struct Node* temp = root;
            root = root->right;
            free(temp);
        }
        else if (root->right == NULL) {
            struct Node* temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Two children
        else {
            struct Node* temp = FindMin(root->right);
            root->data = temp->data;  // Copy the in-order successor's value
            root->right = Delete(root->right, temp->data);  // Delete the in-order successor
        }
    }
    return root;
}

// Function to visit nodes in Inorder
void Inorder(struct Node* root) {
    if (root == NULL) return;
    Inorder(root->left);  // Visit left subtree
    printf("%d ", root->data);  // Print data
    Inorder(root->right);  // Visit right subtree
}

// Function to insert a node in a Binary Search Tree
struct Node* Insert(struct Node* root, int data) {
    if (root == NULL) {
        root = (struct Node*)malloc(sizeof(struct Node));
        root->data = data;
        root->left = root->right = NULL;
    }
    else if (data <= root->data) {
        root->left = Insert(root->left, data);
    }
    else {
        root->right = Insert(root->right, data);
    }
    return root;
}

int main() {
    struct Node* root = NULL;

/*Code To Test the logic
	  Creating an example tree
	                    5
			   / \
			  3   10
			 / \   \
			1   4   11
    */
   
    // Create the BST
    root = Insert(root, 5);
    root = Insert(root, 10);
    root = Insert(root, 3);
    root = Insert(root, 4);
    root = Insert(root, 1);
    root = Insert(root, 11);

    // Delete the node with value 5 (change the value to test other cases)
    root = Delete(root, 5);

    // Print nodes in Inorder
    printf("Inorder: ");
    Inorder(root);
    printf("\n");

    return 0;
}
