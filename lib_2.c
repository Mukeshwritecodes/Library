#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Book structure
struct book {
    char title[50];
    char author[50];
    int id;
    int count;
    float price;
};

// User structure
struct user {
    char name[50];
    long long int phone;
    int bookId;
    float price;
    char status[20];
};

// Waiting list queue implementation
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

// Function declarations
void displayBooks();
void addBook();
void borrowBook();
void returnBook();
int searchBook(int id);
void updateWaitingList();
void displayWaitingList();
struct waitingQueue* createQueue();
void enqueue(struct waitingQueue* q, char name[], long long int phone, int bookId);
struct waitingNode* dequeue(struct waitingQueue* q);
int isQueueEmpty(struct waitingQueue* q);

struct waitingQueue* waitingList = NULL;

int main() {
    waitingList = createQueue();
    int choice;
    
    do {
        printf("\n===== Library Management System =====\n");
        printf("1. Display Books\n2. Add Book\n3. Borrow Book\n");
        printf("4. Return Book\n5. Display Waiting List\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: displayBooks(); break;
            case 2: addBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: displayWaitingList(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
    
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

void displayBooks() {
    struct book b;
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    printf("\nAvailable Books:\n");
    printf("--------------------------------------------------\n");
    
    while (fscanf(file, "Title: %[^\n]\n", b.title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", b.author);
        fscanf(file, "ID: %d\n", &b.id);
        fscanf(file, "Count: %d\n", &b.count);
        fscanf(file, "Price: %f\n\n", &b.price);
        
        printf("Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: $%.2f\n-----------\n", 
               b.title, b.author, b.id, b.count, b.price);
    }
    fclose(file);
}

void addBook() {
    struct book b;
    FILE *file = fopen("books.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    getchar();
    printf("Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = '\0';
    
    printf("Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = '\0';
    
    printf("ID: ");
    scanf("%d", &b.id);
    
    printf("Count: ");
    scanf("%d", &b.count);
    
    printf("Price: ");
    scanf("%f", &b.price);
    
    fprintf(file, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
            b.title, b.author, b.id, b.count, b.price);
    
    fclose(file);
    printf("Book added successfully!\n");
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

void borrowBook() {
    int id;
    printf("Enter Book ID: ");
    scanf("%d", &id);
    
    int count = searchBook(id);
    if (count == -1) {
        printf("Book ID not found!\n");
        return;
    } else if (count == 0) {
        printf("Book not available. Adding to waiting list...\n");
        
        char name[50];
        long long int phone;
        getchar();
        printf("Your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        
        printf("Your phone: ");
        scanf("%lld", &phone);
        
        enqueue(waitingList, name, phone, id);
        printf("Added to waiting list!\n");
        return;
    }
    
    // Update book count
    struct book books[100];
    int bookCount = 0;
    FILE *file = fopen("books.txt", "r");
    
    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        
        if (books[bookCount].id == id) {
            books[bookCount].count--;
        }
        bookCount++;
    }
    fclose(file);
    
    file = fopen("books.txt", "w");
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
                books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
    }
    fclose(file);
    
    printf("Book borrowed successfully!\n");
}

void returnBook() {
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    
    if (searchBook(id) == -1) {
        printf("Book ID not found in system!\n");
        return;
    }
    
    // Update book count
    struct book books[100];
    int bookCount = 0;
    FILE *file = fopen("books.txt", "r");
    
    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF) {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        
        if (books[bookCount].id == id) {
            books[bookCount].count++;
        }
        bookCount++;
    }
    fclose(file);
    
    file = fopen("books.txt", "w");
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "Title: %s\nAuthor: %s\nID: %d\nCount: %d\nPrice: %.2f\n\n", 
                books[i].title, books[i].author, books[i].id, books[i].count, books[i].price);
    }
    fclose(file);
    
    printf("Book returned successfully!\n");
    updateWaitingList();
}

void updateWaitingList() {
    if (isQueueEmpty(waitingList)) return;
    
    struct waitingNode* next = waitingList->front;
    while (next != NULL) {
        int bookCount = searchBook(next->bookId);
        if (bookCount > 0) {
            printf("Notifying %s (Phone: %lld) that book ID %d is now available!\n", 
                   next->name, next->phone, next->bookId);
            
            struct waitingNode* temp = dequeue(waitingList);
            free(temp);
            
            if (isQueueEmpty(waitingList)) break;
            next = waitingList->front;
        } else {
            next = next->next;
        }
    }
}

void displayWaitingList() {
    if (isQueueEmpty(waitingList)) {
        printf("Waiting list is empty.\n");
        return;
    }
    
    printf("\n===== Waiting List =====\n");
    struct waitingNode* current = waitingList->front;
    int count = 1;
    
    while (current != NULL) {
        printf("%d. Name: %s, Phone: %lld, Book ID: %d\n", 
               count, current->name, current->phone, current->bookId);
        current = current->next;
        count++;
    }
}