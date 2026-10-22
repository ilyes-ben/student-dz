#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
int data;
struct Node* left;
struct Node* right;
} Node;

// Helper function to create a new node (as given in the PDF)
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
    printf("Erreur d'allocation mémoire\n");
    exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
// 1. Check if BST is valid
int isBSTUtil(Node* root, int min, int max) {
    if (root == NULL)
    return 1;
    if (root->data <= min || root->data >= max)
    return 0;
    return isBSTUtil(root->left, min, root->data) &&
    isBSTUtil(root->right, root->data, max);
    }
    int isBST(Node* root) {
    return isBSTUtil(root, INT_MIN, INT_MAX);
}

// 2. Find sibling value
int sibling(Node* root, int value) {
    if (root == NULL || root->left == NULL || root->right == NULL)
        return -1;  // No sibling found
        
    if (root->left->data == value && root->right != NULL)
        return root->right->data;
    if (root->right->data == value && root->left != NULL)
        return root->left->data;
        
    int left = sibling(root->left, value);
    if (left != -1) return left;
    return sibling(root->right, value);
}

// 3. Search for a value
int search(Node* root, int val) {
    if (root == NULL)
        return 0;
    if (root->data == val)
        return 1;
    if (val < root->data)
        return search(root->left, val);
    return search(root->right, val);
}

// 4. Find minimum value
int findMin(Node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return -1;
    }
    while (root->left != NULL)
        root = root->left;
    return root->data;
}

// 5. Find successor
Node* findSuccessor(Node* root, int value) {
    Node* successor = NULL;
    Node* current = root;
    
    while (current != NULL) {
        if (value < current->data) {
            successor = current;
            current = current->left;
        }
        else if (value > current->data)
            current = current->right;
        else {
            if (current->right != NULL) {
                successor = current->right;
                while (successor->left != NULL)
                    successor = successor->left;
            }
            break;
        }
    }
    return successor;
}

// 6. Calculate height
int height(Node* root) {
    if (root == NULL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

Node* getMin(Node* root) {
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

// Exercise 3 - Q3: Get pointer to greatest value
Node* getMax(Node* root) {
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

// Exercise 3 - Q5: Get rank of a node
int getRank(Node* root, int key) {
    if (root == NULL)
        return 0;
    
    if (root->data < key)
        return 1 + getRank(root->left, key) + getRank(root->right, key);
    else
        return getRank(root->left, key);
}



// Exercise 4.1: Insert into BST
Node* insert(Node* root, int data) {
    if (root == NULL)
        return createNode(data);
        
    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);
        
    return root;
}


void Preorder(struct Node *root) {
	// base condition for recursion
	// if tree/sub-tree is empty, return and exit
	if(root == NULL) return;

	printf("%c ",root->data); // Print data
	Preorder(root->left);     // Visit left subtree
	Preorder(root->right);    // Visit right subtree
}

//Function to visit nodes in Inorder
void Inorder(Node *root) {
	if(root == NULL) return;

	Inorder(root->left);       //Visit left subtree
	printf("%c ",root->data);  //Print data
	Inorder(root->right);      // Visit right subtree
}

//Function to visit nodes in Postorder
void Postorder(Node *root) {
	if(root == NULL) return;

	Postorder(root->left);    // Visit left subtree
	Postorder(root->right);   // Visit right subtree
	printf("%c ",root->data); // Print data
}

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


// Exercise 4.2: Display values in descending order
void displayDescending(Node* root) {
    if (root != NULL) {
        displayDescending(root->right);  // Visit right subtree
        printf("%d ", root->data);       // Print current node
        displayDescending(root->left);   // Visit left subtree
    }
}

// Exercise 4.3: Delete greatest element
Node* deleteMax(Node* root) {
    if (root == NULL)
        return NULL;
        
    // If no right child, current node is maximum
    if (root->right == NULL) {
        Node* temp = root->left;
        free(root);
        return temp;
    }
    
    // Keep going right until we find the maximum
    root->right = deleteMax(root->right);
    return root;
}

// Helper function to demonstrate Exercise 4.1
Node* buildTreeFromList() {
    int values[] = {11, 15, 5, 2, 3, 9, 17, 21, 22, 13, 19, 4, 12};
    int n = sizeof(values) / sizeof(values[0]);
    
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }
    return root;
}



// Helper structure for largest BST subtree
typedef struct {
    int min;
    int max;
    int size;
    int isBST;
} BSTInfo;

// Function to find largest BST subtree
BSTInfo largestBSTSubtreeUtil(Node* root) {
    if (root == NULL) {
        return (BSTInfo){INT_MAX, INT_MIN, 0, 1};
    }
    
    BSTInfo left = largestBSTSubtreeUtil(root->left);
    BSTInfo right = largestBSTSubtreeUtil(root->right);
    
    BSTInfo current;
    current.min = left.min < root->data ? left.min : root->data;
    current.max = right.max > root->data ? right.max : root->data;
    
    // Check if current subtree is BST
    if (left.isBST && right.isBST && 
        left.max < root->data && right.min > root->data) {
        current.size = left.size + right.size + 1;
        current.isBST = 1;
    } else {
        current.size = left.size > right.size ? left.size : right.size;
        current.isBST = 0;
    }
    
    return current;
}

int findLargestBSTSubtree(Node* root) {
    return largestBSTSubtreeUtil(root).size;
}

// Function to find lowest common ancestor (helper for min distance)
Node* findLCA(Node* root, int n1, int n2) {
    if (root == NULL || root->data == n1 || root->data == n2) {
        return root;
    }
    
    if (n1 < root->data && n2 < root->data) {
        return findLCA(root->left, n1, n2);
    }
    if (n1 > root->data && n2 > root->data) {
        return findLCA(root->right, n1, n2);
    }
    
    return root;
}

// Function to find distance from root to a node
int findDistance(Node* root, int value, int distance) {
    if (root == NULL) return -1;
    if (root->data == value) return distance;
    
    if (value < root->data) {
        return findDistance(root->left, value, distance + 1);
    }
    return findDistance(root->right, value, distance + 1);
}

// Function to find minimum distance between two nodes
int findMinDistance(Node* root, int n1, int n2) {
    Node* lca = findLCA(root, n1, n2);
    int d1 = findDistance(lca, n1, 0);
    int d2 = findDistance(lca, n2, 0);
    return d1 + d2;
}

// Function to find sum in range
int rangeSumBST(Node* root, int low, int high) {
    if (root == NULL) return 0;
    
    // If current node is in range, include it and recurse both sides
    if (root->data >= low && root->data <= high) {
        return root->data + 
               rangeSumBST(root->left, low, high) + 
               rangeSumBST(root->right, low, high);
    }
    // If current node is less than low, only check right subtree
    else if (root->data < low) {
        return rangeSumBST(root->right, low, high);
    }
    // If current node is greater than high, only check left subtree
    else {
        return rangeSumBST(root->left, low, high);
    }
}


int main(void){
    // Create a sample BST
    Node* root = buildTreeFromList();
    
    // Test isBST
    printf("Is BST valid? %s\n", isBST(root) ? "Yes" : "No");
    
    // Test sibling
    printf("Sibling of 5: %d\n", sibling(root, 5));
    
    // Test search
    printf("Is 13 present? %s\n", search(root, 13) ? "Yes" : "No");
    printf("Is 100 present? %s\n", search(root, 100) ? "Yes" : "No");
    
    // Test findMin
    printf("Minimum value: %d\n", findMin(root));
    
    // Test successor
    Node* succ = findSuccessor(root, 13);
    printf("Successor of 13: %d\n", succ ? succ->data : -1);
    
    // Test height
    printf("Height of tree: %d\n", height(root));
    
    // Test getMin and getMax
    printf("Min value (pointer): %d\n", getMin(root)->data);
    printf("Max value (pointer): %d\n", getMax(root)->data);
    
    // Test getRank
    printf("Rank of 15: %d\n", getRank(root, 15));
    
    // Test displayDescending
    printf("Values in descending order: ");
    displayDescending(root);
    printf("\n");
    
    // Test deleteMax
    printf("Deleting maximum value...\n");
    root = deleteMax(root);
    printf("New maximum: %d\n", getMax(root)->data);
    
    // Test largest BST subtree
    printf("Size of largest BST subtree: %d\n", findLargestBSTSubtree(root));
    
    // Test min distance
    printf("Min distance between 5 and 15: %d\n", findMinDistance(root, 5, 15));
    
    // Test range sum
    printf("Sum of values between 5 and 15: %d\n", rangeSumBST(root, 5, 15));

// Create a BST
Node* root = NULL;
root = insert(root, 50);
insert(root, 30);
insert(root, 70);
insert(root, 20);
insert(root, 40);
insert(root, 60);
insert(root, 80);

// Test traversals
printf("\nPreorder traversal: ");
Preorder(root);
printf("\nInorder traversal: ");
Inorder(root);
printf("\nPostorder traversal: ");
Postorder(root);

// Test BST operations
printf("\n\nIs BST? %d", isBST(root));
printf("\nHeight: %d", height(root));
printf("\nMinimum value: %d", findMin(root));
printf("\nSearch for 30: %d", search(root, 30));
printf("\nSibling of 20: %d", sibling(root, 20));

// Test deletion
printf("\n\nDeleting 30...");
root = Delete(root, 30);
printf("\nInorder after deletion: ");
Inorder(root);

// Test range operations
printf("\n\nSum between 20 and 60: %d", rangeSumBST(root, 20, 60));
printf("\nRank of 50: %d", getRank(root, 50));

// Clean up memory before exit
while(root != NULL) {
    root = deleteMax(root);
}
return 0;
}


