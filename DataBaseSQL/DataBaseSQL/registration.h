#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#pragma warning (disable:4996)

int MaxId = -1023;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
    printf("\n");
    return 0;
}
static int intcallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	MaxId = atoi(argv[0]);
	return 0;
}

int registrationtest()
{
    sqlite3 *db;
	char TableName[100] = "Mytable.sqlite";
	char *query = (char*)malloc(1024 * sizeof(char));

    char *zErrMsg = 0;
    int rc;

	//DeleteFile(TableName);
    rc = sqlite3_open(TableName, &db);
    if (rc)
	{
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
	  system("pause");
      return(1);
	}
	rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users\
							(\
								id INT NOT NULL PRIMARY KEY,\
								email VARCHAR(40) NOT NULL,\
								pass VARCHAR(40) NOT NULL\
							);", callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
	{
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
	}
	char email[40];
	char pass[40];
	
	scanf("%s", email);
	scanf("%s", pass);

	
	sprintf(query, "SELECT MAX(id) FROM users");
	sqlite3_exec(db, query, intcallback, 0, &zErrMsg);
	//printf("MaxId = %d", MaxId);


	sprintf(query, "INSERT INTO users(id, email, pass) VALUES (%d, '%s', '%s')", MaxId + 1, email, pass);
	rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);

	sprintf(query, "SELECT* FROM users");
	rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
	
	sqlite3_close(db);


	system("pause");
    return 0;
}