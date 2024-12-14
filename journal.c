#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store a journal entry
typedef struct JournalEntry {
    int day;
    char description[255];
    struct JournalEntry *next; // Pointer to the next entry in the list
} JournalEntry;

// Structure to store journal data for a specific month
typedef struct Journal {
    int year;
    int month;
    JournalEntry *head; // Head pointer for the linked list of journal entries
} Journal;

// Global journal data
Journal journal;

// Function prototypes
void initializeJournal(int year, int month);
void printCalendar(int month, int year);
void addEntry(int day, const char *description);
void viewEntry(int day);
void editEntry(int day, const char *description);
void deleteEntry(int day);
void viewAllEntries();

// Initialize the journal
void initializeJournal(int year, int month) {
    journal.year = year;
    journal.month = month;
    journal.head = NULL; // Start with an empty linked list
}

// Print the calendar for the given month and year
void printCalendar(int month, int year) {
    const char *months[] = {
        "", "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    const int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Check for leap year
    int maxDays = daysInMonth[month];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        maxDays = 29;
    }

    // Zeller's Congruence to find the first day of the month
    int q = 1; // Day of the month
    int m = month;
    int y = year;

    if (m == 1 || m == 2) {
        m += 12;
        y -= 1;
    }

    int k = y % 100;
    int j = y / 100;
    int h = (q + (13 * (m + 1)) / 5 + k + (k / 4) + (j / 4) - (2 * j)) % 7;

    int startDay = (h + 5) % 7; // Convert to 0 = Monday, 6 = Sunday

    printf("\nCalendar for %s %d\n", months[month], year);
    printf("Mo Tu We Th Fr Sa Su\n");

    // Print leading spaces for the first week
    for (int i = 0; i < startDay; i++) {
        printf("   ");
    }

    // Print the days of the month
    for (int day = 1; day <= maxDays; day++) {
        printf("%2d ", day);
        if ((startDay + day) % 7 == 0) printf("\n"); // New line after Sunday
    }
    printf("\n");
}

// Add a journal entry
void addEntry(int day, const char *description) {
    // Check if an entry for the given day already exists
    JournalEntry *current = journal.head;
    while (current) {
        if (current->day == day) {
            printf("Error: An entry for day %d already exists. Please delete it first or edit the existing entry.\n", day);
            return;
        }
        current = current->next;
    }

    // Create a new entry
    JournalEntry *newEntry = (JournalEntry *)malloc(sizeof(JournalEntry));
    if (!newEntry) {
        printf("Memory allocation failed!\n");
        return;
    }
    newEntry->day = day;
    strcpy(newEntry->description, description);
    newEntry->next = NULL;

    // Insert the entry into the linked list in sorted order by day
    if (!journal.head || journal.head->day > day) {
        newEntry->next = journal.head;
        journal.head = newEntry;
    } else {
        JournalEntry *prev = journal.head;
        while (prev->next && prev->next->day < day) {
            prev = prev->next;
        }
        newEntry->next = prev->next;
        prev->next = newEntry;
    }

    printf("Entry added for day %d: %s\n", day, description);
}


// View a journal entry
void viewEntry(int day) {
    JournalEntry *current = journal.head;
    while (current) {
        if (current->day == day) {
            printf("Entry for day %d: %s\n", day, current->description);
            return;
        }
        current = current->next;
    }
    printf("No entry for day %d.\n", day);
}

// Edit a journal entry
void editEntry(int day, const char *description) {
    JournalEntry *current = journal.head;
    while (current) {
        if (current->day == day) {
            strcpy(current->description, description);
            printf("Entry for day %d updated to: %s\n", day, description);
            return;
        }
        current = current->next;
    }
    printf("No entry exists for day %d to edit.\n", day);
}

// Delete a journal entry
void deleteEntry(int day) {
    JournalEntry *current = journal.head;
    JournalEntry *prev = NULL;

    while (current) {
        if (current->day == day) {
            if (prev) {
                prev->next = current->next;
            } else {
                journal.head = current->next;
            }
            free(current);
            printf("Entry for day %d deleted.\n", day);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("No entry exists for day %d to delete.\n", day);
}

// View all entries for the month in an aesthetic way
void viewAllEntries() {
    if (!journal.head) {
        printf("No entries for this month.\n");
        return;
    }

    JournalEntry *current = journal.head;
    printf("\n*******************************\n");
    printf("     Journal Entries for %d-%02d\n", journal.year, journal.month);
    printf("*******************************\n\n");

    while (current) {
        printf("Day: %-2d | Description: %s\n", current->day, current->description);
        printf("----------------------------------\n");
        current = current->next;
    }
    printf("\n*******************************\n");
}

int main() {
    int year, month, choice, day;
    char description[255];

    printf("Enter year: ");
    scanf("%d", &year);
    printf("Enter month (1-12): ");
    scanf("%d", &month);

    if (month < 1 || month > 12) {
        printf("Invalid month!\n");
        return 1;
    }

    initializeJournal(year, month);
    printCalendar(month, year);

  
        printf("\nJournal Menu:\n");
        printf("1. Add Entry\n");
        printf("2. View Entry\n");
        printf("3. Edit Entry\n");
        printf("4. Delete Entry\n");
        printf("5. View All Entries\n");
        printf("6. Exit\n");
		while(1){
        printf("Enter your choice: ");
        scanf("%d", &choice);
	

        switch (choice) {
            case 1:
                printf("Enter day: ");
                scanf("%d", &day);
                printf("Enter description: ");
                getchar(); // Consume newline from previous input
                fgets(description, 255, stdin);
                description[strcspn(description, "\n")] = '\0'; // Remove newline character
                addEntry(day, description);
                break;

            case 2:
                printf("Enter day: ");
                scanf("%d", &day);
                viewEntry(day);
                break;

            case 3:
                printf("Enter day: ");
                scanf("%d", &day);
                printf("Enter new description: ");
                getchar(); // Consume newline from previous input
                fgets(description, 255, stdin);
                description[strcspn(description, "\n")] = '\0'; // Remove newline character
                editEntry(day, description);
                break;

            case 4:
                printf("Enter day: ");
                scanf("%d", &day);
                deleteEntry(day);
                break;

            case 5:
                viewAllEntries();
                break;

            case 6:
                printf("Exiting program. Goodbye!\n");
                exit(0);
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    } 

    return 0;
}


