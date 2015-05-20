#include "Database.h"
#include <string.h>
#include <stdio.h>

//Написать программу для обработки пользовательских запросов

int PerformQueries(sqlite3 *MyBase, char* Queries, int length)
{
	const char *pos = Queries;										//Начало нераспарсенной части запросов
	const char *end = Queries + length;								//Конец всех запросов
	
	int rc; sqlite3_stmt *st;

	while (pos < end - 1)											//Распарсивание запросов, расположенных в одной строке sQueries
	{
		rc = sqlite3_prepare(MyBase, pos, end - pos, &st, &pos);	//Выделение, соостветсвенно, первого запроса в строке

		while ((rc = sqlite3_step(st)) == SQLITE_ROW)				//Вывод строчек текущей части распарсенного запроса				
		{
			int	i;
			for (i = 0; i < sqlite3_column_count(st); ++i)
				printf(i == 0 ? "%s|" : "%30s|", sqlite3_column_text(st, i));
			printf("\n");
		}

		if (rc != SQLITE_DONE && rc != SQLITE_ROW && rc != SQLITE_OK)//Если вдруг возникла какая-то ошибка, то вывести её
		{
			printf("Error %d : %s\n", rc, sqlite3_errmsg(MyBase));
			getchar();
			return -1;
		}

		sqlite3_finalize(st);										//Что-то подобное free, очистить план выполнения для step'а
	}
	return 0;
}
bool CheckQuery(char* Query)
{
	if (strstr(Query, "'"))
	{
		printf("Wrong query.\n");
		return 0;
	}
	else 
		return 1;
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

	printf("Here you can type your commands:\n");
	printf(" - 'exit' to close program\n");
	printf(" - 'clear' to clear screen\n");
	printf(" - 'sql [sql query]' to perform sql query\n");
	printf(" - 'script [scriptname]' to perform script\n");

	while (true)
	{
		printf("Command: ");
		scanf("%s", Command);

		if (!strcmp("exit", Command))
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
			if (!CheckQuery(Queries))
				continue;
			PerformQueries(MyBase, Queries, i);
			continue;
		}
		if (!strcmp("clear", Command))
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			continue;
		}
		printf("Unknown command: %s\n", Command);
	}
	return 0;
}