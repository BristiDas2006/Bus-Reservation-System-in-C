#include<stdio.h>    // Standard input-output header
#include<stdlib.h>   // For exit(), remove(), rename()
#include<string.h>   // For string functions like strcpy, strcmp (if needed)

// Structure to store bus details
struct bus {
    int number;            // Bus number (unique ID)
    char destination[70];  // Destination name
    int seats;             // Total or remaining seats
    int price;             // Fare per seat
};

// Function to add a new bus to the file
void add() {
    struct bus s;  // Create a structure variable to hold bus info
    
    // Take user input for bus details
    printf("Enter the bus number: ");
    scanf("%d", &s.number);
    printf("Enter the destination: ");
    scanf("%s", s.destination);
    printf("Enter the number of total seats: ");
    scanf("%d", &s.seats);
    printf("Enter the fare: ");
    scanf("%d", &s.price);
    
    // Open Bus.txt in append mode to add data at the end
    FILE *fp;
    fp = fopen("Bus.txt", "a");
    if(fp == NULL) {   // Error handling if file not found
        printf("Error opening the file\n");
        return;
    }

    // Write the bus details into the file in CSV format
    fprintf(fp, "%d,%s,%d,%d\n", s.number, s.destination, s.seats, s.price);
    fclose(fp);  // Close file after writing
    printf("Bus details added successfully\n\n");
}

// Function to view all buses stored in the file
void viewbusses() {
    FILE *fp;
    fp = fopen("Bus.txt", "r");  // Open file in read mode
    if(fp == NULL) {
        printf("Error opening the file\n");
        return;
    }
    
    struct bus s;  // Structure variable to read data into

    // Table heading
    printf("\n=== ALL BUSES ===\n");
    printf("Bus No | Destination       | Seats | Price\n");
    printf("------------------------------------------\n");
    
    // Read data line by line until EOF (End of File)
    while(fscanf(fp, "%d,%[^,],%d,%d\n", &s.number, s.destination, &s.seats, &s.price) != EOF) {
        // Print data in table format
        printf("%-6d | %-16s | %-5d | %d\n", s.number, s.destination, s.seats, s.price);
    }
    fclose(fp);  // Close file after reading
    printf("\n");
}

// Function to reserve a seat from a selected bus
void reserve() {
    struct bus s;
    FILE *fp, *temp;
    int no, found = 0;
    
    // Show all available buses before choosing one
    viewbusses();
    printf("Choose a bus number to reserve seat: ");
    scanf("%d", &no);
    
    // Open original file in read mode
    fp = fopen("Bus.txt", "r");
    if(fp == NULL) {
        printf("Error opening the file\n");
        return;
    }
    
    // Open temporary file in write mode (used for updating records)
    temp = fopen("temp.txt", "w");
    if(temp == NULL) {
        printf("Error opening temporary file\n");
        fclose(fp);
        return;
    }
    
    // Read each record and check if it matches the selected bus
    while(fscanf(fp, "%d,%[^,],%d,%d\n", &s.number, s.destination, &s.seats, &s.price) != EOF) {
        if(s.number == no) {  // If matching bus found
            if(s.seats > 0) {  // Check if seat available
                s.seats--;      // Decrease available seats by 1
                found = 1;      // Mark as found
                printf("Seat reserved successfully for bus %d\n", s.number);
                printf("Remaining seats: %d\n", s.seats);
            } else {
                // If no seats left
                printf("No seats available for bus %d\n", s.number);
            }
        }
        // Write (possibly updated) record to temp file
        fprintf(temp, "%d,%s,%d,%d\n", s.number, s.destination, s.seats, s.price);
    }
    
    // Close both files
    fclose(fp);
    fclose(temp);
    
    // Replace original file with updated one
    remove("Bus.txt");
    rename("temp.txt", "Bus.txt");
    
    // If no matching bus was found
    if(!found) {
        printf("Bus number %d not found\n", no);
    }
}

// Function to cancel a seat reservation (add back one seat)
void cancel() {
    struct bus s;
    FILE *fp, *temp;
    int no, found = 0;
    
    // Show all buses before choosing which one to cancel
    viewbusses();
    printf("Choose a bus number to cancel reservation: ");
    scanf("%d", &no);
    
    // Open main file
    fp = fopen("Bus.txt", "r");
    if(fp == NULL) {
        printf("Error opening the file\n");
        return;
    }
    
    // Open temp file for writing updated data
    temp = fopen("temp.txt", "w");
    if(temp == NULL) {
        printf("Error opening temporary file\n");
        fclose(fp);
        return;
    }
    
    // Read file line by line
    while(fscanf(fp, "%d,%[^,],%d,%d\n", &s.number, s.destination, &s.seats, &s.price) != EOF) {
        if(s.number == no) {  // Match found
            s.seats++;        // Increase seats (cancel means freeing seat)
            found = 1;
            printf("Seat cancellation successful for bus %d\n", s.number);
            printf("Available seats now: %d\n", s.seats);
        }
        // Write updated info into temp file
        fprintf(temp, "%d,%s,%d,%d\n", s.number, s.destination, s.seats, s.price);
    }
    
    // Close both files
    fclose(fp);
    fclose(temp);
    
    // Replace Bus.txt with updated temp file
    remove("Bus.txt");
    rename("temp.txt", "Bus.txt");
    
    if(!found) {
        printf("Bus number %d not found\n", no);
    }
}

// Function to show only buses that have available seats
void availableseat() {
    struct bus s;
    FILE *fp;
    
    // Open file for reading
    fp = fopen("Bus.txt", "r");
    if(fp == NULL) {
        printf("Error opening the file\n");
        return;
    }
    
    // Table heading
    printf("\n=== AVAILABLE SEATS ===\n");
    printf("Bus No | Destination       | Available Seats | Price\n");
    printf("----------------------------------------------------\n");
    
    int availableFound = 0;  // Flag to check if any seat is available

    // Read each line from the file
    while(fscanf(fp, "%d,%[^,],%d,%d\n", &s.number, s.destination, &s.seats, &s.price) != EOF) {
        if(s.seats > 0) {  // Show only buses with seats > 0
            printf("%-6d | %-16s | %-15d | %d\n", s.number, s.destination, s.seats, s.price);
            availableFound = 1;
        }
    }
    
    // If no bus has available seats
    if(!availableFound) {
        printf("No buses with available seats found.\n");
    }
    
    fclose(fp);  // Close file
}

// Function to display the main menu
void showmenu() {
    printf("\n============================\n");
    printf("    BUS RESERVATION SYSTEM\n");
    printf("============================\n");
    printf("1. Add Bus\n");
    printf("2. View All Buses\n");
    printf("3. Reserve a Seat\n");
    printf("4. Cancel Reservation\n");
    printf("5. Show Available Seats\n");
    printf("6. Exit\n");
    printf("============================\n");
    printf("Enter your choice: ");
}

// Main function — program execution starts here
int main() {
    int choice;  // Variable to store user choice
    
    printf("Welcome to Bus Reservation System!\n");
    
    while(1) {  // Infinite loop until user exits manually
        showmenu();        // Display menu
        scanf("%d", &choice);  // Take user input
        
        // Use switch-case to call the right function
        switch(choice) {
            case 1:
                add();  // Add a new bus
                break;
            case 2:
                viewbusses();  // View all buses
                break;
            case 3:
                reserve();  // Reserve seat
                break;
            case 4:
                cancel();  // Cancel reservation
                break;
            case 5:
                availableseat();  // Show only available buses
                break;
            case 6:
                printf("Thank you for using Bus Reservation System!\n");
                exit(0);  // Exit program safely
            default:
                printf("Invalid choice! Please enter 1-6.\n");
        }
    }
    return 0;
}
