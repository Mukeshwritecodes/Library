#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define PAGE_SIZE 10

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

struct book {
    char title[50];
    char author[50];
    int id;
    int count;
    float price;
};

struct user {
    char name[50];
    long long int phone;
    char book[50];
    int id;
    float price;
    char status[50];
};

struct waitingNode {
    char name[50];
    long long int phone;
    int bookId;
    struct waitingNode* next;
};

struct waitingQueue {
    struct waitingNode* front;
    struct waitingNode* rear;
};

void displayBook();
void addBook();
void issue_sellBook();
void returnBook();
void displayLog();
int searchBook(int id);
void updateWaitingList();
void displayWaitingList();
void countUpdate(int id);
void sortBook(struct book books[], int bookCount);
struct waitingQueue* createQueue();
void enqueue(struct waitingQueue* q, char name[], long long int phone, int bookId);
struct waitingNode* dequeue(struct waitingQueue* q);
int isQueueEmpty(struct waitingQueue* q);

struct waitingQueue* waitingList = NULL;

void showMenu() {
    printf(GREEN "\n===== Library Management System =====\n" RESET);
    printf(YELLOW "1. Display Books\n2. Add a Book\n3. Borrow a Book\n4. Sell a Book\n"
           "5. Return a Book\n6. Display Transaction Log\n7. Search Book\n"
           "8. Update waiting list\n9. Display waiting list\n" RESET);
    printf(RED "10. Exit\n" RESET);
    printf(BLUE "Enter your choice: " RESET);
}

int main() {
    waitingList = createQueue();
    int choice;
    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input! Please enter a number.\n" RESET);
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: displayBook(); break;
            case 2: addBook(); break;
            case 3:
            case 4: issue_sellBook(); break;
            case 5: returnBook(); break;
            case 6: displayLog(); break;
            case 7: {
                int id;
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                int result = searchBook(id);
                if (result >= 0)
                    printf("Book with ID %d has %d copies available.\n", id, result);
                else
                    printf("Book not found.\n");
                break;
            }
            case 8: updateWaitingList(); break;
            case 9: displayWaitingList(); break;
            case 10: 
                printf(RED "Exiting the program. Thank you!\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice! Please select a valid option.\n" RESET);
        }
    } while (choice != 10);
    return 0;
}

struct waitingQueue* createQueue() {
    struct waitingQueue* q = (struct waitingQueue*)malloc(sizeof(struct waitingQueue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(struct waitingQueue* q, char name[], long long int phone, int bookId) {
    struct waitingNode* newNode = (struct waitingNode*)malloc(sizeof(struct waitingNode));
    strcpy(newNode->name, name);
    newNode->phone = phone;
    newNode->bookId = bookId;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    
    q->rear->next = newNode;
    q->rear = newNode;
}

struct waitingNode* dequeue(struct waitingQueue* q) {
    if (q->front == NULL) return NULL;
    
    struct waitingNode* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) q->rear = NULL;
    
    return temp;
}

int isQueueEmpty(struct waitingQueue* q) {
    return q->front == NULL;
}

void displayBook() {
    struct book books[MAX];
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("Error opening file! Make sure 'books.txt' exists.\n");
        return;
    }

    int bookCount = 0;
    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        bookCount++;
    }
    fclose(file);

    if (bookCount == 0) {
        printf("No books found!\n");
        return;
    }

    int page = 0;
    char choice;
    do {
        int start = page * PAGE_SIZE;
        int end = (start + PAGE_SIZE < bookCount) ? start + PAGE_SIZE : bookCount;

        printf("\n Displaying books %d to %d out of %d:\n", start + 1, end, bookCount);
        printf("--------------------------------------------------\n");

        for (int i = start; i < end; i++) {
            printf("Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: Rs %.2f\n--------------------------\n", 
                   books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
        }

        if (end < bookCount) printf("Press 'n' for Next Page\n");
        if (page > 0) printf("Press 'p' for Previous Page\n");
        printf("Press 'q' to Quit\n\nEnter your choice: ");
        scanf(" %c", &choice);

        if ((choice == 'n' || choice == 'N') && end < bookCount) {
            system("cls");
            page++;
        } else if ((choice == 'p' || choice == 'P') && page > 0) {
            system("cls");
            page--;
        } else if (choice == 'q' || choice == 'Q') {
            printf("\nExiting book list display. Goodbye!\n");
            return;
        } else {
            printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 'q' && choice != 'Q');
}

void addBook() {
    struct book books[MAX];
    int bookCount = 0;
    char choice;
    
    FILE *file = fopen("books.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
            fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
            fscanf(file, "ID: %d\n", &books[bookCount].id);
            fscanf(file, "Count: %d\n", &books[bookCount].count);
            fscanf(file, "Price: %f\n\n", &books[bookCount].price);
            bookCount++;
        }
        fclose(file);
    }

    do {
        if (bookCount >= MAX) {
            printf(RED "Library is full! Cannot add more books.\n" RESET);
            break;
        }

        printf(GREEN "\nEnter details of the new book:\n" RESET);
        
        printf("Title: ");
        getchar();
        fgets(books[bookCount].title, sizeof(books[bookCount].title), stdin);
        books[bookCount].title[strcspn(books[bookCount].title, "\n")] = '\0';

        printf("Author: ");
        fgets(books[bookCount].author, sizeof(books[bookCount].author), stdin);
        books[bookCount].author[strcspn(books[bookCount].author, "\n")] = '\0';

        printf("ID: ");
        scanf("%d", &books[bookCount].id);
        printf("Count: ");
        scanf("%d", &books[bookCount].count);
        printf("Price: ");
        scanf("%f", &books[bookCount].price);
        bookCount++;

        printf(YELLOW "\nDo you want to add another book? (y/n): " RESET);
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    sortBook(books, bookCount);

    file = fopen("books.txt", "w");
    if (file == NULL) {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
                books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
    }

    fclose(file);
    printf(GREEN "\nBooks added successfully!\n" RESET);
}

void sortBook(struct book books[], int bookCount) {
    for (int i = 0; i < bookCount - 1; i++) {
        for (int j = i + 1; j < bookCount; j++) {
            if (books[i].id > books[j].id) {
                struct book temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }
    printf(YELLOW "\nBooks sorted by ID.\n" RESET);
}

void issue_sellBook() {
    int id;
    struct user users;
    
    printf("Enter the Book ID\n");
    scanf("%d", &id);
    
    int count = searchBook(id);
    if (count == -1) {
        printf("Enter a valid Book ID\n");
        char response;
        printf("Would you like to be added to the waiting list? (y/n): ");
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') {
            char name[50];
            long long int phone;
            getchar();
            printf("Enter your name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            
            printf("Enter your phone: ");
            scanf("%lld", &phone);
            
            enqueue(waitingList, name, phone, id);
            printf("You've been added to the waiting list for book ID %d.\n", id);
        }
        return;
    } else if (count == 0) {
        printf("Book not available\n");
        char response;
        printf("Would you like to be added to the waiting list? (y/n): ");
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') {
            char name[50];
            long long int phone;
            getchar();
            printf("Enter your name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            
            printf("Enter your phone: ");
            scanf("%lld", &phone);
            
            enqueue(waitingList, name, phone, id);
            printf("You've been added to the waiting list for book ID %d.\n", id);
        }
        return;
    }
    
    FILE *file = fopen("Booklog.txt", "a");
    if (file == NULL) {
        printf("Error opening file! Make sure 'Booklog.txt' exists.\n");
        return;
    }
    
    getchar();
    printf("Name: ");
    fgets(users.name, sizeof(users.name), stdin);
    users.name[strcspn(users.name, "\n")] = '\0';

    printf("Phone: ");
    scanf("%lld", &users.phone);
    getchar();

    printf("Book Title: ");
    fgets(users.book, sizeof(users.book), stdin);
    users.book[strcspn(users.book, "\n")] = '\0';

    printf("ID: ");
    scanf("%d", &users.id);
    printf("Price: ");
    scanf("%f", &users.price);
    getchar();

    printf("Status(Including dates): ");
    fgets(users.status, sizeof(users.status), stdin);
    users.status[strcspn(users.status, "\n")] = '\0';

    fprintf(file, "Name: %s\nPhone: %lld\nBook: %s\nID: %d\nPrice: %.2f\nStatus: %s\n\n", 
            users.name, users.phone, users.book, users.id, users.price, users.status);
    fclose(file);
    
    countUpdate(id);
}

int searchBook(int id) {
    struct book b;
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) return -1;

    while (fscanf(file, "Title: %[^\n]\n", b.title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", b.author);
        fscanf(file, "ID: %d\n", &b.id);
        fscanf(file, "Count: %d\n", &b.count);
        fscanf(file, "Price: %f\n\n", &b.price);
        
        if (b.id == id) {
            fclose(file);
            return b.count;
        }
    }
    fclose(file);
    return -1;
}

void countUpdate(int id) {
    struct book books[MAX];
    int bookCount = 0, found = 0;
    
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) return;
    
    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        
        if (books[bookCount].id == id && books[bookCount].count > 0) {
            books[bookCount].count--;
            found = 1;
        }
        bookCount++;
    }
    fclose(file);

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) return;
    
    for (int i = 0; i < bookCount; i++) {
        fprintf(tempFile, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
                books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
    }
    fclose(tempFile);
    
    remove("books.txt");
    rename("temp.txt", "books.txt");
    
    if (!found) printf("Book ID not found or no copies available!\n");
    else printf("Count update successful\n");
}

void returnBook() {
    int id;
    printf("Enter the Book ID to return: ");
    scanf("%d", &id);
    
    struct book books[MAX];
    int bookCount = 0, found = 0;
    
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("Error opening books database!\n");
        return;
    }
    
    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        
        if (books[bookCount].id == id) {
            books[bookCount].count++;
            found = 1;
            printf(GREEN "Book '%s' returned successfully!\n" RESET, books[bookCount].title);
        }
        bookCount++;
    }
    fclose(file);
    
    if (!found) {
        printf(RED "Book ID not found in the system!\n" RESET);
        return;
    }
    
    file = fopen("books.txt", "w");
    if (file == NULL) {
        printf(RED "Error updating books database!\n" RESET);
        return;
    }
    
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
                books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
    }
    fclose(file);
    
    char name[50];
    getchar();
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    
    FILE *logFile = fopen("Booklog.txt", "a");
    if (logFile == NULL) {
        printf(RED "Error updating transaction log!\n" RESET);
        return;
    }
    
    fprintf(logFile, "Name: %s\nBook ID: %d\nStatus: Returned\n\n", name, id);
    fclose(logFile);
    
    updateWaitingList();
}

void displayLog() {
    FILE *file = fopen("Booklog.txt", "r");
    if (file == NULL) {
        printf(RED "Transaction log is empty or cannot be opened!\n" RESET);
        return;
    }
    
    char line[100];
    int pageCounter = 0;
    int lineCounter = 0;
    const int LINES_PER_PAGE = 20;
    
    printf(GREEN "\n===== Transaction Log =====\n" RESET);
    
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        lineCounter++;
        
        if (lineCounter >= LINES_PER_PAGE) {
            pageCounter++;
            lineCounter = 0;
            
            char choice;
            printf(YELLOW "\nPress 'n' for next page or 'q' to quit: " RESET);
            scanf(" %c", &choice);
            
            if (choice == 'q' || choice == 'Q') {
                break;
            }
            
            printf(GREEN "\n===== Transaction Log (Page %d) =====\n" RESET, pageCounter + 1);
        }
    }
    
    if (lineCounter == 0 && pageCounter == 0) {
        printf(RED "No transactions found in the log!\n" RESET);
    }
    
    fclose(file);
}

void updateWaitingList() {
    if (isQueueEmpty(waitingList)) {
        printf(YELLOW "No customers in the waiting list.\n" RESET);
        return;
    }
    
    struct waitingNode* next = waitingList->front;
    int notifiedCount = 0;
    
    while (next != NULL) {
        int bookCount = searchBook(next->bookId);
        if (bookCount > 0) {
            printf(GREEN "Notifying %s (Phone: %lld) that book ID %d is now available!\n" RESET, 
                   next->name, next->phone, next->bookId);
            
            struct waitingNode* temp = dequeue(waitingList);
            free(temp);
            notifiedCount++;
            
            if (isQueueEmpty(waitingList)) break;
            next = waitingList->front;
        } else {
            next = next->next;
        }
    }
    
    if (notifiedCount == 0) {
        printf(YELLOW "No customers could be notified - requested books still unavailable.\n" RESET);
    } else {
        printf(GREEN "%d customer(s) notified about available books.\n" RESET, notifiedCount);
    }
}

void displayWaitingList() {
    if (isQueueEmpty(waitingList)) {
        printf(YELLOW "Waiting list is empty.\n" RESET);
        return;
    }
    
    printf(GREEN "\n===== Waiting List =====\n" RESET);
    
    struct waitingNode* current = waitingList->front;
    int count = 1;
    
    while (current != NULL) {
        printf("%d. Name: %s\n   Phone: %lld\n   Book ID: %d\n", 
               count, current->name, current->phone, current->bookId);
        
        int bookCount = searchBook(current->bookId);
        printf("   Book Status: ");
        if (bookCount > 0) {
            printf(GREEN "Available (%d copies)\n" RESET, bookCount);
        } else if (bookCount == 0) {
            printf(RED "Out of stock\n" RESET);
        } else {
            printf(RED "Book not in system\n" RESET);
        }
        
        printf("--------------------------\n");
        current = current->next;
        count++;
    }
}