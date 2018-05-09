#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>

#define USER_DATABASE "USER_PASSWORD.txt"
#define BOOK_DATABASE "BOOK_INFORMATION.txt"
#define BOOK_ID_FILE "UNIQUE_ID_BOOK.txt"
#define USER_ID_FILE "UNIQUE_ID_USER.txt"
#define LOGIN_ATTEMPT 3


#pragma region Structures
typedef struct Book {
	char id[10];
	char name[25];
	char authorName[25];
	char price[6];
	char issue[5];
}Book;
Book books[100];

int NumberOfBooks;
typedef struct User {
	char id[10];
	char userName[20];
	char password[16];
	char type[2];
	char borrowStatus[10];
}User;
User users[100];
 
int NumberOfUsers;
#pragma endregion Structures

#pragma region Prototypes
int GetUniqueIDFromFile(const char* fileName);
void ReadBooksFromFile();
void WriteBooksToFile();
Book* SearchBook(const char*);
void AddBook(Book);
void EditBook(const char*, const char*, const char*, const char*, const char*);
void DeleteBook(const char*);
void WriteBookContent(FILE*, Book);
int GetUniqueIDForBook();

void ReadUsersFromFile();
void WriteUsersToFile();
void AddUser(User);
User* SearchUser(const char* id);
void WriteUserContent(FILE*, User);
void EditUser(const char*, const char*, const char*, const char*, const char*);
void DeleteUser(const char*);
int GetUniqueIDForUser();
void userLogin(int );
#pragma endregion Prototypes

int main()
{

	ReadUsersFromFile();
	ReadBooksFromFile();
	/*User u = { "a", "User", "pass", "A", "0" };
	char seq[3];
	int id = GetUniqueIDForUser();
	itoa(id, seq, 10);
	strcpy(u.id, "U-");
	strcat(u.id, seq);*/
	Book b = { "a", "Name", "AuthName", "10", "NO" };
	char seq[3];
	int id = GetUniqueIDForUser();
	itoa(id, seq, 10);
	strcpy(b.id, "B-");
	strcat(b.id, seq);
	
	AddBook(b);
	/*int loginAttempRemaining = LOGIN_ATTEMPT;
	userLogin(loginAttempRemaining);*/
	return 0;
}

int GetUniqueIDFromFile(const char* fileName)
{
	int id;
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		printf("Unique ID file not found");
		return;
	}

	fscanf(f, "%d", &id);
	fclose(f);
	f = fopen(fileName, "w");
	if (!f)
	{
		printf("Unique ID file not found");
		return;
	}
	fprintf(f, "%d", id + 1);
	fclose(f);
	return id;
}


#pragma region Book Functions
void ReadBooksFromFile()
{
	FILE* bookFile = fopen(BOOK_DATABASE, "r");
	if (!bookFile)
	{
		printf("Book Database not found");
		return;
	}
	char c;
	char buffer[80];
	int index = 0;
	int buffIndex = 0;
	while ((c = fgetc(bookFile)))
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
	fclose(bookFile);
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
	int found = 0;
	for (int i = 0; i < NumberOfBooks; i++)
	{
		if (strcmp(id, books[i].id) == 0)
		{
			return &books[i];
		}
	}
	return NULL;
}

void EditBook(const char* id, const char* name, const char* authorName, const char* price, const char* issue)
{
	Book* b = SearchBook(id);
	if (b == NULL)
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

int GetUniqueIDForBook()
{
	return GetUniqueIDFromFile(BOOK_ID_FILE);
}

#pragma endregion Book Functions

#pragma region User Functions
void ReadUsersFromFile()
{
	FILE* userFile = fopen(USER_DATABASE, "r");
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

void AddUser(User user)
{
	FILE* userFile = fopen(USER_DATABASE, "a");
	if (!userFile)
	{
		printf("User Database not found");
		return;
	}
	WriteUserContent(userFile, user);
	NumberOfUsers++;
	fclose(userFile);
	printf("User Added Successfully");
}

User* SearchUser(const char* id)
{
	int found = 0;
	for (int i = 0; i < NumberOfUsers; i++)
	{
		if (strcmp(id, users[i].id) == 0)
		{
			return &users[i];
		}
	}
	return NULL;
}

void EditUser(const char* id, const char* userName, const char* password, const char* type, const char* borrowStatus)
{
	User* u = SearchUser(id);
	if (u == NULL)
	{
		printf("User not found");
	}
	else
	{
		if (strlen(userName)) strcpy(u->userName, userName);
		if (strlen(password)) strcpy(u->password, password);
		if (strlen(type)) strcpy(u->type, type);
		if (strlen(borrowStatus)) strcpy(u->borrowStatus, borrowStatus);
		WriteUsersToFile();
		printf("User Updated");
	}
}

void DeleteUser(const char* id)
{
	int index = -1;
	for (int i = 0; i < NumberOfUsers; i++)
	{
		if (strcmp(id, users[i].id) == 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		printf("User not found");
		return;
	}
	for (int i = index + 1; i < NumberOfUsers; i++)
	{
		users[i] = users[i + 1];
	}
	NumberOfUsers--;
	WriteUsersToFile();
	printf("User successfully deleted");
}

void WriteUsersToFile()
{
	FILE* userFile = fopen(USER_DATABASE, "w");
	if (!userFile)
	{
		printf("User Database not found");
		return;
	}
	for (int i = 0; i < NumberOfUsers; i++)
	{
		WriteUserContent(userFile, users[i]);
	}
	fclose(userFile);
}

void WriteUserContent(FILE* userFile, User u)
{
	fprintf(userFile, "%s", u.id);
	fprintf(userFile, "%c", '|');
	int nameLength = strlen(u.userName);
	for (int i = 0; i < nameLength; i++)
	{
		fprintf(userFile, "%c", u.userName[i]);
	}
	fprintf(userFile, "%c", '|');
	int passLength = strlen(u.password);
	for (int i = 0; i < passLength; i++)
	{
		fprintf(userFile, "%c", u.password[i]);
	}
	fprintf(userFile, "%c", '|');
	fprintf(userFile, "%s", u.type);
	fprintf(userFile, "%c", '|');
	fprintf(userFile, "%s", u.borrowStatus);
	fprintf(userFile, "%c", '\n');
}

int GetUniqueIDForUser()
{
	return GetUniqueIDFromFile(USER_ID_FILE);
}
#pragma endregion User Functions

void userLogin(int loginAttemptRemaining)
{
	FILE* userFile;
	
	if (loginAttemptRemaining == 0)
	{
		printf("\n\t\t\tYOU FINISHED YOUR CHANCE TO LOGIN.\n\n");
		return;
	}
	userFile = fopen(USER_DATABASE, "r");
	if (userFile)
	{
		
	}
	else
	{
		printf("\n\n\t\t\tFILE DOSN'T FOUND.\a\a\n");
		return;
	}



}