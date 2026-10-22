#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple property structure
typedef struct Property {
    int id;
    int category;    // 1:economic, 2:social, 3:normal
    float price;
    char locality[20];
    struct Property* next;
} Property;

// Display properties of a specific category
void displayByCategory(Property* head, int category) {
    Property* temp = head;
    printf("\nProperties in category %d:\n", category);
    while (temp != NULL) {
        if (temp->category == category) {
            printf("ID: %d, Price: %.2f, Locality: %s\n", 
                   temp->id, temp->price, temp->locality);
        }
        temp = temp->next;
    }
}

// Insert at beginning
Property* insertProperty(Property* head) {
    Property* new = (Property*)malloc(sizeof(Property));
    if (!new) return head;
    
    printf("Enter ID: ");
    scanf("%d", &new->id);
    printf("Enter category (1:economic, 2:social, 3:normal): ");
    scanf("%d", &new->category);
    printf("Enter price: ");
    scanf("%f", &new->price);
    printf("Enter locality: ");
    scanf("%s", new->locality);
    
    new->next = head;
    return new;
}

// Find minimum price in social category
float minSocialPrice(Property* head) {
    float min = -1;
    Property* temp = head;
    
    while (temp != NULL) {
        if (temp->category == 2) {
            if (min == -1 || temp->price < min) {
                min = temp->price;
            }
        }
        temp = temp->next;
    }
    return min;
}

// Count economic properties in BBA
int countEconomicBBA(Property* head) {
    int count = 0;
    Property* temp = head;
    
    while (temp != NULL) {
        if (temp->category == 1 && strcmp(temp->locality, "BBA") == 0) {
            count++;
        }
        temp = temp->next;
    }
    return count;
}

// Find category with most properties
void mostCommonCategory(Property* head) {
    int cat1 = 0, cat2 = 0, cat3 = 0;
    Property* temp = head;
    
    while (temp != NULL) {
        if (temp->category == 1) cat1++;
        else if (temp->category == 2) cat2++;
        else if (temp->category == 3) cat3++;
        temp = temp->next;
    }
    
    if (cat1 >= cat2 && cat1 >= cat3)
        printf("Most common category: Economic (1)\n");
    else if (cat2 >= cat1 && cat2 >= cat3)
        printf("Most common category: Social (2)\n");
    else
        printf("Most common category: Normal (3)\n");
}

// Fill and sort normal category prices
void normalPrices(Property* head) {
    float prices[50];
    int count = 0;
    Property* temp = head;
    
    // Fill array
    while (temp != NULL) {
        if (temp->category == 3) {
            prices[count++] = temp->price;
        }
        temp = temp->next;
    }
    
    // Simple bubble sort
    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-i-1; j++) {
            if (prices[j] > prices[j+1]) {
                float temp = prices[j];
                prices[j] = prices[j+1];
                prices[j+1] = temp;
            }
        }
    }
    
    // Display sorted prices
    printf("Sorted normal category prices:\n");
    for (int i = 0; i < count; i++) {
        printf("%.2f ", prices[i]);
    }
    printf("\n");
}

int main() {
    Property* head = NULL;
    int choice;
    
    do {
        printf("\n1. Insert property");
        printf("\n2. Display by category");
        printf("\n3. Min social price");
        printf("\n4. Count economic in BBA");
        printf("\n5. Most common category");
        printf("\n6. Sort normal prices");
        printf("\n7. Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                head = insertProperty(head);
                break;
            case 2: {
                int cat;
                printf("Enter category (1-3): ");
                scanf("%d", &cat);
                displayByCategory(head, cat);
                break;
            }
            case 3: {
                float min = minSocialPrice(head);
                if (min != -1)
                    printf("Minimum social price: %.2f\n", min);
                else
                    printf("No social properties found\n");
                break;
            }
            case 4:
                printf("Economic properties in BBA: %d\n", 
                       countEconomicBBA(head));
                break;
            case 5:
                mostCommonCategory(head);
                break;
            case 6:
                normalPrices(head);
                break;
            case 7:
                // Free memory
                while (head != NULL) {
                    Property* temp = head;
                    head = head->next;
                    free(temp);
                }
                printf("Goodbye!\n");
                break;
        }
    } while (choice != 7);
    
    return 0;
}