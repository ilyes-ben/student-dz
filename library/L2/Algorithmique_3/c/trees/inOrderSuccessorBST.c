#include <stdio.h>
#include <stdlib.h>

// Define the structure for a BST node
struct Node {
    int data;
    struct Node *left, *right;
};

// Function to create a new BST node
struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

// Function to find a node with a given value in the BST
struct Node* Find(struct Node* root, int data) {
    if (root == NULL) return NULL;
    if (root->data == data) return root;
    if (root->data < data) return Find(root->right, data);
    return Find(root->left, data);
}

// Function to find the node with the minimum value in a BST
struct Node* FindMin(struct Node* root) {
    if (root == NULL) return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Function to find the inorder successor of a given node in the BST
struct Node* GetSuccessor(struct Node* root, int data) {
    // Search the node - O(h)
    struct Node* current = Find(root, data);
    if (current == NULL) return NULL;

    // Case 1: Node has a right subtree
    if (current->right != NULL) {
        return FindMin(current->right); // O(h)
    }

    // Case 2: No right subtree - O(h)
    struct Node* successor = NULL;
    struct Node* ancestor = root;
    while (ancestor != current) {
        if (current->data < ancestor->data) {
            successor = ancestor; // So far, this is the deepest node for which current is in left
            ancestor = ancestor->left;
        } else {
            ancestor = ancestor->right;
        }
    }
    return successor;
}

// Function to perform an inorder traversal of the BST
void Inorder(struct Node* root) {
    if (root == NULL) return;

    Inorder(root->left);       // Visit left subtree
    printf("%d ", root->data); // Print data
    Inorder(root->right);      // Visit right subtree
}

// Function to insert a node into the BST
struct Node* Insert(struct Node* root, int data) {
    if (root == NULL) {
        root = newNode(data);
    } else if (data <= root->data) {
        root->left = Insert(root->left, data);
    } else {
        root->right = Insert(root->right, data);
    }
    return root;
}

int main() {
    /* Example tree
          5
         / \\
        3   10
       / \\    \\
      1   4     11
    */
    struct Node* root = NULL;
    root = Insert(root, 5);
    root = Insert(root, 10);
    root = Insert(root, 3);
    root = Insert(root, 4);
    root = Insert(root, 1);
    root = Insert(root, 11);

    // Print nodes in Inorder
    printf("Inorder Traversal: ");
    Inorder(root);
    printf("\\n");

    // Find inorder successor of a node
    struct Node* successor = GetSuccessor(root, 1);
    if (successor == NULL) {
        printf("No successor found\\n");
    } else {
        printf("Successor is %d\\n", successor->data);
    }

    return 0;
}
