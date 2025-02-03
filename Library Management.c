#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_AUTHOR 50

// Structure to store book details
typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int available; // 1 = Available, 0 = Issued
} Book;

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void issueBook();
void deleteBook();
void saveBook(Book book);
void loadBooks();

FILE *file; // File pointer

int main() {
    int choice;
    
    while (1) {
        printf("\n==== Library Management System ====\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume newline character

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: deleteBook(); break;
            case 6: printf("Exiting the program.\n"); exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to add a new book
void addBook() {
    Book book;
    
    printf("Enter Book ID: ");
    scanf("%d", &book.id);
    getchar(); // Consume newline character

    printf("Enter Book Title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = '\0'; // Remove newline

    printf("Enter Author Name: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = '\0'; // Remove newline

    book.available = 1; // Book is available

    saveBook(book);
    printf("Book added successfully!\n");
}

// Function to save book details to file
void saveBook(Book book) {
    file = fopen("library.dat", "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
}

// Function to display all books
void displayBooks() {
    Book book;
    file = fopen("library.dat", "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }
    
    printf("\n==== Book List ====\n");
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("ID: %d | Title: %s | Author: %s | Status: %s\n",
               book.id, book.title, book.author, book.available ? "Available" : "Issued");
    }
    fclose(file);
}

// Function to search for a book by ID or Title
void searchBook() {
    Book book;
    int id, found = 0;
    char title[MAX_TITLE];

    file = fopen("library.dat", "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }

    printf("Search by:\n1. ID\n2. Title\nEnter choice: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("Enter Book ID: ");
        scanf("%d", &id);
        while (fread(&book, sizeof(Book), 1, file)) {
            if (book.id == id) {
                printf("Book Found - ID: %d | Title: %s | Author: %s | Status: %s\n",
                       book.id, book.title, book.author, book.available ? "Available" : "Issued");
                found = 1;
                break;
            }
        }
    } else if (choice == 2) {
        printf("Enter Book Title: ");
        getchar();
        fgets(title, MAX_TITLE, stdin);
        title[strcspn(title, "\n")] = '\0';

        while (fread(&book, sizeof(Book), 1, file)) {
            if (strcmp(book.title, title) == 0) {
                printf("Book Found - ID: %d | Title: %s | Author: %s | Status: %s\n",
                       book.id, book.title, book.author, book.available ? "Available" : "Issued");
                found = 1;
                break;
            }
        }
    } else {
        printf("Invalid choice!\n");
    }

    if (!found) {
        printf("Book not found!\n");
    }

    fclose(file);
}

// Function to issue a book
void issueBook() {
    Book book;
    int id, found = 0;
    
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    file = fopen("library.dat", "rb+");
    if (!file) {
        printf("No books found!\n");
        return;
    }

    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id && book.available == 1) {
            book.available = 0;
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("Book issued successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not available or not found!\n");
    }

    fclose(file);
}

// Function to delete a book by ID
void deleteBook() {
    Book book;
    int id, found = 0;

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    FILE *tempFile = fopen("temp.dat", "wb");
    file = fopen("library.dat", "rb");

    if (!file || !tempFile) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            found = 1;
            printf("Book deleted successfully!\n");
            continue;
        }
        fwrite(&book, sizeof(Book), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);
    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!found) {
        printf("Book not found!\n");
    }
}
