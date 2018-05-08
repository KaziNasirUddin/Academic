#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>

#define USERNAME_PASSWORD_FILE "password.txt"
#define BOOK_DATABASE "BOOK_INFORMATION.txt"
#define LOGIN_ATTEMPT 3;

#pragma region Structures
struct Book {
	char id[10];
	char name[25];
	char authorName[25];
	char price[6];
	char issue[5];
}books[100];
int NumberOfBooks;
struct User {
	char id[10];
	char userName[20];
	char password[16];
	char type[2];
	char borrowStatus[10];
}users[100];
int NumberOfUsers;
#pragma endregion Structures


void ReadUsersFromFile();
void ReadBooksFromFile();
void WriteBooksToFile();
Book* SearchBook(const char*);
void AddBook(Book);
void EditBook(const char*, const char*, const char*, const char*, const char*);
void DeleteBook(const char*);
void WriteBookContent(FILE*, Book);
void userLogin(int );

int main()
{
	ReadUsersFromFile();
	ReadBooksFromFile();
	DeleteBook("A1-6");
	/*int loginAttempRemaining = LOGIN_ATTEMPT;
	userLogin(loginAttempRemaining);*/
	return 0;
}

void ReadUsersFromFile()
{
	FILE* userFile = fopen(USERNAME_PASSWORD_FILE, "r");
	if (!userFile)
	{
		printf("User Database not found");
		return;
	}
	char c;
	char buffer[80];
	int index = 0;
	int buffIndex = 0;
	while ((c = fgetc(userFile)))
	{
		if (c == '\n' || c == EOF)
		{
			if (buffIndex == 0) break;
			buffer[buffIndex] = 0;
			buffIndex = 0;
			strcpy(users[index].id, strtok(buffer, "|"));
			strcpy(users[index].userName, strtok(NULL, "|"));
			strcpy(users[index].password, strtok(NULL, "|"));
			strcpy(users[index].type, strtok(NULL, "|"));
			strcpy(users[index].borrowStatus, strtok(NULL, "\n"));
			index++;
			if (c == EOF) break;
		}
		else 
		{
			buffer[buffIndex++] = c;
		}
	}
	NumberOfUsers = index;
	fclose(userFile);
}

void ReadBooksFromFile()
{
	FILE* bookFIle = fopen(BOOK_DATABASE, "r");
	if (!bookFIle)
	{
		printf("Book Database not found");
		return;
	}
	char c;
	char buffer[80];
	int index = 0;
	int buffIndex = 0;
	while ((c = fgetc(bookFIle)))
	{
		if (c == '\n' || c == EOF)
		{
			if (buffIndex == 0) break;
			buffer[buffIndex] = 0;
			buffIndex = 0;
			strcpy(books[index].id, strtok(buffer, "|"));
			strcpy(books[index].name, strtok(NULL, "|"));
			strcpy(books[index].authorName, strtok(NULL, "|"));
			strcpy(books[index].price, strtok(NULL, "|"));
			strcpy(books[index].issue, strtok(NULL, "\n"));
			index++;
			if (c == EOF) break;
		}
		else
		{
			buffer[buffIndex++] = c;
		}
	}
	NumberOfBooks = index;
	fclose(bookFIle);
}

void WriteBooksToFile()
{
	FILE* bookFile = fopen(BOOK_DATABASE, "w");
	if (!bookFile)
	{
		printf("Book Database not found");
		return;
	}
	for (int i = 0; i < NumberOfBooks; i++)
	{
		WriteBookContent(bookFile, books[i]);
	}
	fclose(bookFile);
}

void WriteBookContent(FILE* bookFile, Book b)
{
	fprintf(bookFile, "%s", b.id);
	fprintf(bookFile, "%c", '|');
	int nameLength = strlen(b.name);
	for (int i = 0; i < nameLength; i++)
	{
		fprintf(bookFile, "%c", b.name[i]);
	}
	fprintf(bookFile, "%c", '|');
	int authorLength = strlen(b.authorName);
	for (int i = 0; i < authorLength; i++)
	{
		fprintf(bookFile, "%c", b.authorName[i]);
	}
	fprintf(bookFile, "%c", '|');
	fprintf(bookFile, "%s", b.price);
	fprintf(bookFile, "%c", '|');
	fprintf(bookFile, "%s", b.issue);
	fprintf(bookFile, "%c", '\n');

}

void AddBook(Book b)
{
	FILE* bookFile = fopen(BOOK_DATABASE, "a");
	if (!bookFile)
	{
		printf("Book Database not found");
		return;
	}
	WriteBookContent(bookFile, b);
	NumberOfBooks++;
	fclose(bookFile);
	printf("Book Added Successfully");
}

Book* SearchBook(const char* id)
{
	bool found = false;
	for (int i = 0; i < NumberOfBooks; i++)
	{
		if (strcmp(id, books[i].id) == 0)
		{
			return &books[i];
		}
	}
	return nullptr;
}

void EditBook(const char* id, const char* name, const char* authorName, const char* price, const char* issue)
{
	Book* b = SearchBook(id);
	if (b == nullptr)
	{
		printf("Book not found");
	}
	else
	{
		if (strlen(name)) strcpy(b->name, name);
		if (strlen(authorName)) strcpy(b->authorName, authorName);
		if (strlen(price)) strcpy(b->price, price);
		if (strlen(issue)) strcpy(b->issue, issue);
		WriteBooksToFile();
		printf("Book Updated");
	}
}

void DeleteBook(const char* id)
{
	int index = -1;
	for (int i = 0; i < NumberOfBooks; i++)
	{
		if (strcmp(id, books[i].id) == 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		printf("Book not found");
		return;
	}
	for (int i = index + 1; i < NumberOfBooks; i++)
	{
		books[i] = books[i + 1];
	}
	NumberOfBooks--;
	WriteBooksToFile();
	printf("Book successfully deleted");
}

void userLogin(int loginAttemptRemaining)
{
	FILE* userFile;
	
	if (loginAttemptRemaining == 0)
	{
		printf("\n\t\t\tYOU FINISHED YOUR CHANCE TO LOGIN.\n\n");
		return;
	}
	userFile = fopen(USERNAME_PASSWORD_FILE, "r");
	if (userFile)
	{
		
	}
	else
	{
		printf("\n\n\t\t\tFILE DOSN'T FOUND.\a\a\n");
		return;
	}



}