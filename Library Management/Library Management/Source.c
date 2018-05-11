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

int IsAdmin = 0;
char userName[20];
char password[16];

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
int GetUniqueIDFromFile(const char*);
void PrintChar(char, int);
void PrintLine(const char*);
void ReadBooksFromFile();
void WriteBooksToFile();
int SearchBook(const char*);
void AddBook(Book);
void EditBook(const char*, const char*, const char*, const char*, const char*);
void DeleteBook(const char*);
void WriteBookContent(FILE*, Book);
char* GetUniqueIDForBook();
void PrintBookHeader();
void PrintBook(Book);
Book InputBook();

void ReadUsersFromFile();
void WriteUsersToFile();
void AddUser(User);
User* SearchUser(const char*);
int SearchUserByName(const char*);
void WriteUserContent(FILE*, User);
void EditUser(const char*, const char*, const char*, const char*, const char*);
void DeleteUser(const char*);
char* GetUniqueIDForUser();
void PrintUserHeader();
void PrintUser(User);
User InputUser();
void userLogin(int );
#pragma endregion Prototypes

int main()
{
	ReadBooksFromFile();
	ReadUsersFromFile();
	PrintLine("WELCOME TO LIBRARY MANAGEMENT");
	PrintLine("-----------------------------");
	int loginAttempRemaining = LOGIN_ATTEMPT;
	userLogin(loginAttempRemaining);
	getchar();
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
void PrintChar(char c, int times)
{
	while (times--)
	{
		printf("%c", c);
	}
}
void PrintLine(const char* s)
{
	printf("\n\t\t%s\n", s);
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
		PrintLine("BOOK DATABASE NOT FOUND");
		return;
	}
	WriteBookContent(bookFile, b);
	books[NumberOfBooks++] = b;
	fclose(bookFile);
	PrintLine("BOOK ADDED SUCCESSFULLY");
}

int SearchBook(const char* id)
{
	for (int i = 0; i < NumberOfBooks; i++)
	{
		if (strcmp(id, books[i].id) == 0)
		{
			return i;
		}
	}
	return -1;
}

void EditBook(const char* id, const char* name, const char* authorName, const char* price, const char* issue)
{
	Book* b = SearchBook(id);
	if (b == NULL)
	{
		PrintLine("BOOK NOT FOUND");
	}
	else
	{
		if (strlen(name)) strcpy(b->name, name);
		if (strlen(authorName)) strcpy(b->authorName, authorName);
		if (strlen(price)) strcpy(b->price, price);
		if (strlen(issue)) strcpy(b->issue, issue);
		WriteBooksToFile();
		PrintLine("BOOK UPDATED SUCCESSFULLY");
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
		PrintLine("BOOK NOT FOUND");
		return;
	}
	for (int i = index + 1; i < NumberOfBooks; i++)
	{
		books[i - 1] = books[i];
	}
	NumberOfBooks--;
	WriteBooksToFile();
	PrintLine("BOOK DELETED SUCCESSFULLY");
}

char* GetUniqueIDForBook()
{
	char id[10];
	char seq[3];
	int num = GetUniqueIDFromFile(BOOK_ID_FILE);
	itoa(num, seq, 10);
	strcpy(id, "B-");
	strcat(id, seq);
	return id;
}

void PrintBookHeader()
{
	printf("%s","ID");
	PrintChar(' ', 7);
	printf("%s", "NAME");
	PrintChar(' ', 20);
	printf("%s", "AUTHOR");
	PrintChar(' ', 18);
	printf("%s", "PRICE");
	PrintChar(' ', 1);
	printf("%s", "ISSUE\n");
	PrintChar('-', 68);
	printf("%s", "\n");
}

void PrintBook(Book b)
{
	printf("%-9s", b.id);
	printf("%-24s", b.name);
	printf("%-24s", b.authorName);
	printf("%-6s", b.price);
	printf("%s\n", b.issue);
}

Book InputBook()
{
	Book b;
	printf("\n\t\tENTER BOOK NAME: ");
	scanf("%[^\n]%*c", b.name);
	strcat(b.name, "\0");
	printf("\n\t\tENTER AUTHOR NAME: ");
	scanf("%[^\n]%*c", b.authorName);
	strcat(b.authorName, "\0");
	printf("\n\t\tENTER PRICE: ");
	scanf("%[^\n]%*c", b.price);
	strcat(b.price, "\0");
	strcpy(b.issue, "NO");
	return b;
}

void ChangeUserName();

void ChangePassword();

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
		PrintLine("USER DATABASE NOT FOUND");
		return;
	}
	WriteUserContent(userFile, user);
	users[NumberOfUsers++] = user;
	fclose(userFile);
	PrintLine("USER ADDED SUCCESSFULLY");
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

int SearchUserByName(const char* name)
{
	int found = 0;
	for (int i = 0; i < NumberOfUsers; i++)
	{
		if (strcmp(name, users[i].userName) == 0)
		{
			return i;
		}
	}
	return -1;
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

char* GetUniqueIDForUser()
{
	char id[10];
	char seq[3];
	int num = GetUniqueIDFromFile(USER_ID_FILE);
	itoa(num, seq, 10);
	strcpy(id, "U-");
	strcat(id, seq);
	return id;
}

void PrintUserHeader()
{
	printf("%s", "ID");
	PrintChar(' ', 7);
	printf("%s", "NAME");
	PrintChar(' ', 15);
	printf("%s", "PASSWORD");
	PrintChar(' ', 7);
	printf("%s", "TYPE");
	PrintChar(' ', 1);
	printf("%s", "BORROW_STATUS\n");
	PrintChar('-', 61);
	printf("%s", "\n");
}

void PrintUser(User u)
{
	printf("%-9s", u.id);
	printf("%-19s", u.userName);
	printf("%-15s", u.password);
	printf("%-5s", u.type);
	printf("%s\n", u.borrowStatus);
}

User InputUser()
{
	User u;
	printf("\n\t\tENTER USER NAME: ");
	scanf("%[^\n]%*c", u.userName);
	strcat(u.userName, "\0");
	printf("\n\t\tENTER PASSWORD: ");
	scanf("%[^\n]%*c", u.password);
	strcat(u.password, "\0");
	printf("\n\t\tENTER TYPE: ");
	scanf("%[^\n]%*c", u.type);
	strcat(u.type, "\0");
	strcpy(u.borrowStatus, "NO");
	return u;
}

void ChangeUserName()
{
	char oldUserName[20];
	char newUserName[20];
	printf("\n\t\tENTER CURRENT USER NAME: ");
	scanf("%[^\n]%*c", oldUserName);
	int index = SearchUserByName(oldUserName);
	if (index == -1)
	{
		PrintLine("USERNAME NOT FOUND");
		return;
	}
	printf("\n\t\tENTER NEW USER NAME: ");
	scanf("%[^\n]%*c", newUserName);
	strcpy(users[index].userName, newUserName);
	WriteUsersToFile();
	PrintLine("USERNAME CHANGED SUCCESSFULLY");
}

void ChangePassword()
{
	char oldUserName[20];
	char oldPassword[16];
	char newPassword[20];
	printf("\n\t\tENTER CURRENT USER NAME: ");
	scanf("%[^\n]%*c", oldUserName);
	printf("\n\t\tENTER CURRENT PASSWORD: ");
	scanf("%[^\n]%*c", oldPassword);
	int index = SearchUserByName(oldUserName);
	if (index == -1)
	{
		PrintLine("USERNAME NOT FOUND");
		return;
	}
	if (strcmp(users[index].password, oldPassword))
	{
		PrintLine("USERNAME/PASSWORD NOT MATCHED");
		return;
	}
	printf("\n\t\tENTER NEW PASSWORD:");
	scanf("%[^\n]%*c", newPassword);
	strcpy(users[index].password, newPassword);
	WriteUsersToFile();
	PrintLine("PASSWORD CHANGED SUCCESSFULLY");
}

void ViewUserProfile()
{
	int viewType;
	char garbage;
	PrintLine("WHAT YOU WANT TO VIEW:");
	PrintLine("\tPRESS 1\"FOR ALL STUDENT PROFILE\"");
	PrintLine("\tPRESS 2\"FOR ALL ADMIN PROFILE\"");
	PrintLine("\tPRESS 3\"FOR MY PROFILE\" ");
	scanf("%d%c", &viewType, &garbage);
	PrintUserHeader();
	switch (viewType)
	{
		case 1:
			for (int i = 0; i < NumberOfUsers; i++)
			{
				if (!strcmp(users[i].type, "S"))
				{
					PrintUser(users[i]);
				}
			}
			break;
		case 2:
			for (int i = 0; i < NumberOfUsers; i++)
			{
				if (!strcmp(users[i].type, "A"))
				{
					PrintUser(users[i]);
				}
			}
			break;
		case 3:
			for (int i = 0; i < NumberOfUsers; i++)
			{
				if (!strcmp(users[i].userName, userName))
				{
					PrintUser(users[i]);
					break;
				}
			}
			break;
	}
}
#pragma endregion User Functions

int GetAdminCommands() {

	char command[25];
	printf("\n\t\tADMIN COMMAND: ");
	gets(command);
	if (strcmp(strupr(command), "ADD_BOOK") == 0) 
	{
		Book b = InputBook();
		strcpy(b.id, GetUniqueIDForBook());
		AddBook(b);
	}
	else if (strcmp(strupr(command), "DELETE_BOOK") == 0) 
	{
		char* id[10];
		printf("\n\t\tENTER BOOK ID: ");
		scanf("%s", id);
		DeleteBook(id);
		
	}
	else if (strcmp(strupr(command), "EDIT_BOOK") == 0)
	{
		char id[10];
		printf("\n\t\tENTER BOOK ID: ");
		scanf("%[^\n]%*c", id);
		Book b = InputBook();
		EditBook(id, b.name, b.authorName, b.price, b.issue);
	}
	else if (strcmp(strupr(command), "BORROW_BOOK") == 0) 
	{
		char bookId[10]; //todo: Input Book Id
		char userId[10]; //todo: Input User Id
		//todo: BorrowBook(); 
	}
	else if (strcmp(strupr(command), "VIEW_BOOK") == 0) 
	{
		PrintBookHeader();
		ReadBooksFromFile();
		for (int i = 0; i < NumberOfBooks; i++)
		{
			PrintBook(books[i]);
		}
	}
	else if (strcmp(strupr(command), "SEARCH_BOOK") == 0) 
	{
		char* id[10];
		printf("\n\t\tENTER BOOK ID: ");
		scanf("%s", id);
		PrintBookHeader();
		int i = SearchBook(id);
		PrintBook(books[i]);
	}
	else if (strcmp(strupr(command), "LOG_OUT") == 0) 
	{
		exit(1);
	}
	else if (strcmp(strupr(command), "RETURN_BOOK") == 0) 
	{
		//todo:
		RETURN_BOOK();
	}
	else if (strcmp(strupr(command), "HELP") == 0) 
	{
		GetAdminHelp();
	}
	else if (strcmp(strupr(command), "CLEAR") == 0) 
	{
		system("cls");
	}
	else if (strcmp(strupr(command), "RESTART") == 0) 
	{
		ReadBooksFromFile();
		ReadUsersFromFile();
	}
	else if (strcmp(strupr(command), "CHANGE_USER_NAME") == 0) 
	{
		ChangeUserName();
	}
	else if (strcmp(strupr(command), "CHANGE_PASSWORD") == 0) 
	{
		ChangePassword();
	}
	else if (strcmp(strupr(command), "VIEW_PROFILE") == 0) 
	{
		ViewUserProfile();
	}
	else if (strcmp(strupr(command), "ADD_USER") == 0) 
	{
		User u = InputUser();
		strcpy(u.id, GetUniqueIDForUser());
		AddUser(u);
	}
	else 
	{
		PrintLine("Please Provide Correct Command");
	}

	return 0;
}
int GetStudentCommands() {

	char command[25];
	printf("\n\t\tSTUDENT COMMAND: ");
	gets(command);

	if (strcmp(strupr(command), "VIEW_BOOK") == 0) 
	{
		PrintBookHeader();
		ReadBooksFromFile();
		for (int i = 0; i < NumberOfBooks; i++)
		{
			PrintBook(books[i]);
		}
	}
	else if (strcmp(strupr(command), "SEARCH_BOOK") == 0) 
	{
		char* id[10];
		printf("\n\t\tENTER BOOK ID: ");
		scanf("%s", id);
		PrintBookHeader();
		int i = SearchBook(id);
		PrintBook(books[i]);
	}
	else if (strcmp(strupr(command), "LOG_OUT") == 0) 
	{
		exit(1);
	}
	else if (strcmp(strupr(command), "HELP") == 0) 
	{
		GetStudentHelp();
	}
	else if (strcmp(strupr(command), "CLEAR") == 0) 
	{
		system("cls");
	}
	else if (strcmp(strupr(command), "CHANGE_USER_NAME") == 0) 
	{
		ChangeUserName();
	}
	else if (strcmp(strupr(command), "CHANGE_PASSWORD") == 0) 
	{
		ChangePassword();
	}
	else if (strcmp(strupr(command), "MY_PROFILE") == 0) 
	{
		PrintUserHeader();
		for (int i = 0; i < NumberOfUsers; i++)
		{
			if (!strcmp(users[i].userName, userName))
			{
				PrintUser(users[i]);
				break;
			}
		}
	}
	else 
	{
		PrintLine("Please Provide Correct Command");
	}

	return 0;
}
int GetCommands(int isAdmin) 
{
	if (isAdmin) 
	{
		GetAdminCommands();
	}
	else 
	{
		GetStudentCommands();
	}
	return 0;
}
int GetAdminHelp () 
{
	printf("\n\t\t*ENTER ADD_BOOK FOR ADDING BOOK.*\n");
	printf("\n\t\t*ENTER DELETE_BOOK FOR DELETING BOOK.*\n");
	printf("\n\t\t*ENTER EDIT_BOOK FOR EIDITING BOOK.*\n");
	printf("\n\t\t*ENTER BORROW_BOOK FOR BORROW BOOK.*\n");
	printf("\n\t\t*ENTER VIEW_BOOK FOR VIEWING BOOK.*\n");
	printf("\n\t\t*ENTER SEARCH_BOOK FOR SEARCHING BOOK.*\n");
	printf("\n\t\t*ENTER RETURN_BOOK FOR RETURNING BOOK.*\n");
	printf("\n\t\t*ENTER LOG_OUT FOR LOGING OUT.*\n");
	printf("\n\t\t*ENTER CLEAR FOR CLEANING CONSOL.*\n");
	printf("\n\t\t*ENTER RESTART FOR RESTART THE SYSTEM.*\n");
	printf("\n\t\t*ENTER CHANGE_USER_NAME FOR CHANGING USER NAME.*\n");
	printf("\n\t\t*ENTER CHANGE_PASSWORD FOR CHANGING PASSWORD.*\n");
	printf("\n\t\t*ENTER ADD_USER FOR ADDING NEW USER.*\n");
	printf("\n\t\t*ENTER VIEW_PROFILE FOR VIEWING PROFILE .*\n");
	GetCommands(IsAdmin);
	return 0;
}

int GetStudentHelp() 
{
	printf("\n\t\t*ENTER VIEW_BOOK FOR VIEWING BOOK.*\n");
	printf("\n\t\t*ENTER SEARCH_BOOK FOR SEARCHING BOOK.*\n");
	//printf("\n\t\t*ENTER RETURN_BOOK FOR RETURNING BOOK.*\n");
	printf("\n\t\t*ENTER LOG_OUT FOR LOGING OUT.*\n");
	printf("\n\t\t*ENTER CLEAR FOR CLEANING CONSOL.*\n");
	printf("\n\t\t*ENTER CHANGE_USER_NAME FOR CHANGING USER NAME.*\n");
	printf("\n\t\t*ENTER CHANGE_PASSWORD FOR CHANGING PASSWORD.*\n");
	printf("\n\t\t*ENTER MY_PROFILE FOR PROFILE INFORMATION.*\n");
	COMMAND(IsAdmin);

	return 0;
}
void userLogin(int loginAttemptRemaining)
{
	FILE* userFile;
	if (loginAttemptRemaining == 0)
	{
		PrintLine("LOGIN CHANCE FINISHED.");
		return;
	}
	printf("\n\t\tYOU HAVE %d CHANCES TO LOG IN\n\n", loginAttemptRemaining);
	
	printf("\t\tENTER USERNAME: ");
	scanf("%[^\n]%*c", userName);
	printf("\t\tENTER PASSWORD: ");
	scanf("%[^\n]%*c", password);

	userFile = fopen(USER_DATABASE, "r");
	if (userFile)
	{
		ReadUsersFromFile();
	    int index = SearchUserByName(userName);
		if (index == -1 )
		{
			PrintLine("USERNAME NOT FOUND");
			userLogin(--loginAttemptRemaining);
			return;
		}
		User u = users[index];
		if (strcmp(u.password, password))
		{
			PrintLine("USERNAME/PASSWORD NOT MATCHED");
			userLogin(--loginAttemptRemaining);
			return;
		}
		if (!strcmp(u.type, "A"))
		{
			IsAdmin = 1;
		}
		else if (!strcmp(u.type, "S"))
		{
			IsAdmin = 0;
		}
		else
		{
			PrintLine("INVALID USER TYPE");
			userLogin(--loginAttemptRemaining);
			return;
		}
		PrintLine("YOU HAVE SUCCESFULLY LOGGED IN");
		PrintLine("Type HELP FOR SEEING AVAILABLE COMMANDS");
		printf("\t\tYOUR USER ID: %s\n", u.id);
		while (1)
		{
			GetCommands(IsAdmin);
		}
	}
	else
	{
		PrintLine("USER DATABASE NOT FOUND.");
		return;
	}
}