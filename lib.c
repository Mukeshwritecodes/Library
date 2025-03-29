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

struct book
{
    char title[50];
    char author[50];
    int id;
    int count;
    float price;
    char status[20];
};

void displayBook();
void sortBook();
void addBook();
void issue_sellBook();
void returnBook();
void displayLog();
void searchBook();
void updateWaitingList();
void displayWaitingList();

void showMenu()
{
    printf(GREEN "\n===== Library Management System =====\n" RESET);
    printf(YELLOW "1. Display Books\n" RESET);
    printf(YELLOW "2. Add a Book\n" RESET);
    printf(YELLOW "3. Borrow a Book\n" RESET);
    printf(YELLOW "4. Sell a Book\n" RESET);
    printf(YELLOW "5. Return a Book\n" RESET);
    printf(YELLOW "6. Display Transaction Log\n" RESET);
    printf(YELLOW "7. Search Book\n" RESET);
    printf(YELLOW "8. Update waiting list\n" RESET);
    printf(YELLOW "9. Display waiting list\n" RESET);

    printf(RED "10. Exit\n" RESET);
    printf(BLUE "Enter your choice: " RESET);
}

int main()
{
    int choice;
    do
    {
        showMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf(RED "Invalid input! Please enter a number.\n" RESET);
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            displayBook();
            break;

        case 2:
            addBook();
            break;

            // case 3:
            //     issue_sellBook();
            //     break;

            // case 4:
            //     issue_sellBook();
            //     break;

            // case 5:
            //     returnBook();
            //     break;

            // case 6:
            //     displayLog();
            //     break;

            // case 7:
            //     searchBook();
            //     break;

            // case 8:
            //     updateWaitingList();
            //     break;

            // case 9:
            //     displayWaitingList();
            //     break;

        case 10:
            printf(RED "Exiting the program. Thank you!\n" RESET);
            exit(0);
            break;

        default:
            printf(RED "Invalid choice! Please select a valid option.\n" RESET);
            break;
        }

    } while (choice != 10);

    return 0;
}

void displayBook()
{
    struct book books[MAX];
    FILE *file;
    file = fopen("books.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file! Make sure 'books.txt' exists.\n");
        exit(1);
    }

    int bookCount = 0;

    while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF)
    {
        fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
        fscanf(file, "ID: %d\n", &books[bookCount].id);
        fscanf(file, "Count: %d\n", &books[bookCount].count);
        fscanf(file, "Price: %f\n\n", &books[bookCount].price);
        fscanf(file, "Status: %[^\n]\n", books[bookCount].status);

        bookCount++;
    }
    fclose(file);

    if (bookCount == 0)
    {
        printf("No books found!\n");
        return;
    }

    int page = 0;
    char choice;

    do
    {
        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > bookCount)
        {
            end = bookCount;
        }

        printf("\n Displaying books %d to %d out of %d:\n", start + 1, end, bookCount);
        printf("--------------------------------------------------\n");

        for (int i = start; i < end; i++)
        {
            printf("Title: %s\n", books[i].title);
            printf("Author: %s\n", books[i].author);
            printf("ID: %d\n", books[i].id);
            printf("Count: %d\n", books[i].count);
            printf("Price: Rs %.2f\n", books[i].price);
            printf("Status: %s\n", books[i].status);
            printf("--------------------------\n");
        }

        if (end < bookCount)
        {
            printf("Press 'n' for Next Page\n");
        }
        if (page > 0)
        {
            printf("Press 'p' for Previous Page\n");
        }
        printf("Press 'q' to Quit\n");

        printf("\nEnter your choice: ");
        scanf(" %c", &choice);

        switch (choice)
        {
        case 'n':
        case 'N':
            if (end < bookCount)
            {
                system("cls");
                page++;
            }
            else
            {
                printf("\nNo more pages available!\n");
            }
            break;

        case 'p':
        case 'P':
            if (page > 0)
            {
                system("cls");
                page--;
            }
            else
            {
                printf("\nYou are already on the first page!\n");
            }
            break;

        case 'q':
        case 'Q':
            printf("\nExiting book list display. Goodbye!\n");
            return;

        default:
            printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 'q' && choice != 'Q');
}

void addBook()
{
    struct book books[MAX];
    int bookCount = 0;
    char choice;
    FILE *file;

    file = fopen("books.txt", "r");
    if (file != NULL)
    {
        while (fscanf(file, "Title: %[^\n]\n", books[bookCount].title) != EOF)
        {
            fscanf(file, "Author: %[^\n]\n", books[bookCount].author);
            fscanf(file, "ID: %d\n", &books[bookCount].id);
            fscanf(file, "Count: %d\n", &books[bookCount].count);
            fscanf(file, "Price: %f\n", &books[bookCount].price);
            fscanf(file, "Status: %[^\n]\n", books[bookCount].status);
            bookCount++;
        }
        fclose(file);
    }

    do
    {
        if (bookCount >= MAX)
        {
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

        printf("Status (Available/Issued): ");
        getchar();
        fgets(books[bookCount].status, sizeof(books[bookCount].status), stdin);
        books[bookCount].status[strcspn(books[bookCount].status, "\n")] = '\0';

        bookCount++;

        printf(YELLOW "\nDo you want to add another book? (y/n): " RESET);
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    sortBook(books, bookCount);

    file = fopen("books.txt", "w");
    if (file == NULL)
    {
        printf(RED "Error opening file!\n" RESET);
        exit(1);
    }

    for (int i = 0; i < bookCount; i++)
    {
        fprintf(file, "Title: %s\n", books[i].title);
        fprintf(file, "Author: %s\n", books[i].author);
        fprintf(file, "ID: %d\n", books[i].id);
        fprintf(file, "Count: %d\n", books[i].count);
        fprintf(file, "Price: %.2f\n", books[i].price);
        fprintf(file, "Status: %s\n\n", books[i].status);
    }

    fclose(file);
    printf(GREEN "\nBooks added successfully!\n" RESET);
}

void sortBook(struct book books[], int bookCount)
{
    struct book temp;

    for (int i = 0; i < bookCount - 1; i++)
    {
        for (int j = i + 1; j < bookCount; j++)
        {
            if (books[i].id > books[j].id)
            {
                temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }

    printf(YELLOW "\nBooks sorted by ID.\n" RESET);
}
