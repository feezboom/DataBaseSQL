#include "Database.h"
#include <string.h>
#include <stdio.h>

//�������� ��������� ��� ��������� ���������������� ��������

int PerformQueries(sqlite3 *MyBase, char* Queries, int length)
{
	const char *pos = Queries;										//������ �������������� ����� ��������
	const char *end = Queries + length;								//����� ���� ��������
	
	int rc; sqlite3_stmt *st;

	while (pos < end - 1)											//������������� ��������, ������������� � ����� ������ sQueries
	{
		rc = sqlite3_prepare(MyBase, pos, end - pos, &st, &pos);	//���������, ��������������, ������� ������� � ������

		while ((rc = sqlite3_step(st)) == SQLITE_ROW)				//����� ������� ������� ����� ������������� �������				
		{
			int	i;
			for (i = 0; i < sqlite3_column_count(st); ++i)
				printf(i == 0 ? "%s|" : "%10s|", sqlite3_column_text(st, i));
			printf("\n");
		}

		if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK)//���� ����� �������� �����-�� ������, �� ������� �
		{
			printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
			getchar();
			return -1;
		}

		sqlite3_finalize(st);										//���-�� �������� free, �������� ���� ���������� ��� step'�
	}
	return 0;
}
int InsertIntoProducts(sqlite3 *MyBase, char* Product)
{
	int rc; sqlite3_stmt *st;
	char* query = "INSERT INTO Products(ProductName) VALUES(?)";
	rc = sqlite3_prepare(MyBase, query, strlen(query), &st, 0);
	sqlite3_bind_text(st, 1, Product, strlen(Product), 0);
	while ((rc = sqlite3_step(st)) == SQLITE_ROW);

	if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK)
	{
		printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
		getchar();
		return -1;
	}
	sqlite3_finalize(st);									

	return 0;
}
int InsertIntoWorkers(sqlite3 *MyBase, char* Worker)
{	
	int rc; sqlite3_stmt *st;
	char* query = "INSERT INTO Workers(FullName) VALUES(?)";
	rc = sqlite3_prepare(MyBase, query, strlen(query), &st, 0);	
	sqlite3_bind_text(st, 1, Worker, strlen(Worker), 0);
	while ((rc = sqlite3_step(st)) == SQLITE_ROW);

	if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK)
	{
		printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
		getchar();
		return -1;
	}
	sqlite3_finalize(st);									

	return 0;
}
void PrintHelp()
{
	printf("Here you can type your commands:\n");
	printf(" - 'q' to close program\n");
	printf(" - 'clear' to clear screen\n");
	printf(" - 'sql [sql query]' to perform sql query\n");
	printf(" - 'script [scriptname]' to perform script\n");
	printf(" - '0 [product name]' to add new product\n");
	printf(" - '1 [worker name]' to add new worker\n");
	printf(" - '--help' to show this message again\n");
}
int Processing()
{
	char Queries[MAX_STRING_SIZE];
	char Command[MAX_STRING_SIZE];
	char BaseName[20] = "Restaraunt.db3";

	sqlite3 *MyBase;
	int rc = sqlite3_open(BaseName, &MyBase);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(MyBase));
		sqlite3_close(MyBase);
		BaseName[0] = '\0';
	}

	PrintHelp();

	while (true)
	{
		printf("Command: ");
		scanf("%s", Command);

		if (!strcmp("q", Command))
			exit(0);

		if (!strcmp("script", Command))
		{
			char filename[1024] = "";
			scanf("%s", filename);

			FILE* in = fopen(filename, "r");
			if (in == NULL)
			{
				printf("Error opening script %s.\n", filename);
				continue;
			}
			char c; int i = 0;
			while ((c = fgetc(in)) != EOF) Queries[i++] = c;
			Queries[i] = '\0';

			PerformQueries(MyBase, Queries, i);
			continue;
		}
		if (!strcmp("sql", Command))
		{
			char t; int i = 0;
			while ((t = getchar()) != '\n') Queries[i++] = t;
			Queries[i] = '\0';
			PerformQueries(MyBase, Queries, i);
			continue;
		}
		if (!strcmp("clear", Command))
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			continue;
		}
		if (!strcmp("0", Command))
		{
			char t; int i = 0;
			while ((t = getchar()) != '\n') Queries[i++] = t;
			Queries[i] = '\0';
			InsertIntoProducts(MyBase, Queries);
			continue;
		}
		if (!strcmp("1", Command))
		{
			char t; int i = 0;
			while ((t = getchar()) != '\n') Queries[i++] = t;
			Queries[i] = '\0';
			InsertIntoWorkers(MyBase, Queries);
			continue;
		}
		if (!strcmp("--help", Command))
		{
			PrintHelp();
			continue;
		}
		printf("Unknown command: %s\n", Command);
	}
	return 0;
}