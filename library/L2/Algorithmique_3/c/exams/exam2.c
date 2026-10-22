#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple vehicle structure
typedef struct Vehicle {
    int year;
    char energy[20];
    char brand[20];
    int seats;
    struct Vehicle* next;
} Vehicle;

// Test if vehicle exists with given year
int Test(Vehicle* head, int year) {
    Vehicle* temp = head;
    while (temp != NULL) {
        if (temp->year == year)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// Add new vehicle in sorted order
Vehicle* Add(Vehicle* head) {
    Vehicle* new = (Vehicle*)malloc(sizeof(Vehicle));
    if (!new) return head;
    
    // Get data
    printf("Enter year: ");
    scanf("%d", &new->year);
    printf("Enter energy type: ");
    scanf("%s", new->energy);
    printf("Enter brand: ");
    scanf("%s", new->brand);
    printf("Enter seats: ");
    scanf("%d", &new->seats);
    
    // Insert at beginning if list empty or year is smallest
    if (head == NULL || new->year <= head->year) {
        new->next = head;
        return new;
    }
    
    // Find position to insert
    Vehicle* temp = head;
    while (temp->next != NULL && temp->next->year < new->year) {
        temp = temp->next;
    }
    
    // Insert vehicle
    new->next = temp->next;
    temp->next = new;
    return head;
}

// Display all vehicles
void Display(Vehicle* head) {
    if (head == NULL) {
        printf("No vehicles in list\n");
        return;
    }
    
    Vehicle* temp = head;
    printf("\nVehicle List:\n");
    while (temp != NULL) {
        printf("Year: %d\n", temp->year);
        printf("Energy: %s\n", temp->energy);
        printf("Brand: %s\n", temp->brand);
        printf("Seats: %d\n\n", temp->seats);
        temp = temp->next;
    }
}

// Build sorted list from array
Vehicle* BuildFromArray(int years[], int n) {
    Vehicle* head = NULL;
    
    for (int i = 0; i < n; i++) {
        Vehicle* new = (Vehicle*)malloc(sizeof(Vehicle));
        if (!new) continue;
        
        // Set year from array, other fields default
        new->year = years[i];
        strcpy(new->energy, "Unknown");
        strcpy(new->brand, "Unknown");
        new->seats = 0;
        
        // Insert in sorted position
        if (head == NULL || years[i] <= head->year) {
            new->next = head;
            head = new;
        } else {
            Vehicle* temp = head;
            while (temp->next != NULL && temp->next->year < years[i]) {
                temp = temp->next;
            }
            new->next = temp->next;
            temp->next = new;
        }
    }
    return head;
}

int main() {
    Vehicle* head = NULL;
    int choice;
    
    do {
        printf("\n1. Add vehicle");
        printf("\n2. Test year");
        printf("\n3. Display vehicles");
        printf("\n4. Build from array");
        printf("\n5. Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                head = Add(head);
                break;
                
            case 2: {
                int year;
                printf("Enter year to test: ");
                scanf("%d", &year);
                if (Test(head, year))
                    printf("Vehicle exists\n");
                else
                    printf("Vehicle not found\n");
                break;
            }
                
            case 3:
                Display(head);
                break;
                
            case 4: {
                int n;
                printf("Enter number of years: ");
                scanf("%d", &n);
                int years[50];  // Fixed size for simplicity
                printf("Enter years:\n");
                for (int i = 0; i < n; i++)
                    scanf("%d", &years[i]);
                head = BuildFromArray(years, n);
                break;
            }
                
            case 5:
                // Free memory
                while (head != NULL) {
                    Vehicle* temp = head;
                    head = head->next;
                    free(temp);
                }
                printf("Goodbye!\n");
                break;
        }
    } while (choice != 5);
    
    return 0;
}