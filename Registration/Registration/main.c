#include <stdio.h>
#include "sqlite3.h"

#pragma warning (disable:4996)


int login_occupied(sqlite3 *MyBase, char* login)
{
	sqlite3_stmt *st;
	char* check = "select login from users where login = ?";
	int rc = sqlite3_prepare(MyBase, check, strlen(check), &st, 0);
	sqlite3_bind_text(st, 1, login, strlen(login), 0);
	int result =  sqlite3_step(st) == SQLITE_ROW ? 1 : 0;
	sqlite3_finalize(st);
	return result;
}

int main()
{
	int i = 1;
	char BaseName[20] = "Registration.db3";
	sqlite3 *MyBase;
	sqlite3_stmt *st;

	int rc = sqlite3_open(BaseName, &MyBase);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(MyBase));
		sqlite3_close(MyBase);
	}

	char* query = "CREATE TABLE IF NOT EXISTS Users(\
									ID INTEGER PRIMARY KEY AUTOINCREMENT,\
									login varchar(40),\
									password varchar(40),\
									Name varchar(40),\
									Surname varchar(40))";
	rc = sqlite3_prepare(MyBase, query, strlen(query), &st, 0);
	while ((rc = sqlite3_step(st)) == SQLITE_ROW);
	if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK)
	{
		printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
		getchar();
		return -1;
	}
	sqlite3_finalize(st);

	
	query = "INSERT INTO Users(Name, Surname, login, password) VALUES(?, ?, ?, ?)";
	rc = sqlite3_prepare(MyBase, query, strlen(query), &st, 0);
	char name[41], surname[41], login[41], password[41];

	printf("Name-->");
	scanf("%s", name);
	sqlite3_bind_text(st, 1, name, strlen(name), 0);
	printf("Surname-->");
	scanf("%s", surname);
	sqlite3_bind_text(st, 2, surname, strlen(surname), 0);
	printf("login-->");
	scanf("%s", login);
	if (login_occupied(MyBase, login))
	{
		printf("Somebody has already registered using this login...\n");
		system("pause");
		return 1;
	}
	sqlite3_bind_text(st, 3, login, strlen(login), 0);
	printf("password-->");
	scanf("%s", password);
	sqlite3_bind_text(st, 4, password, strlen(password), 0);

	while ((rc = sqlite3_step(st)) == SQLITE_ROW);
	if (rc != SQLITE_DONE && rc != SQLITE_OK)
	{
		printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
		getchar();
		return -1;
	}

	printf("Successfully registered!\n");
	sqlite3_finalize(st);
	system("pause");
	return 0;
}