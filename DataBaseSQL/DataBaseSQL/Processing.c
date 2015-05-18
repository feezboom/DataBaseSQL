#include "Database.h"

#include <fcntl.h>
#include <string.h>

//Написать программу для обработки пользовательских запросов
//На русском языке
bool opened(char* BaseName)
{
	return !strcmp("", BaseName) ? false : true;
}



int PerformQueries(sqlite3 *MyBase, char* Queries)
{
	const char *pos = Queries;										//Начало нераспарсенной части запросов
	const char *end = Queries + strlen(Queries);					//Конец всех запросов
	
	int rc; sqlite3_stmt *st;

	while (pos < end - 1)											//Распарсивание запросов, расположенных в одной строке sQueries
	{
		rc = sqlite3_prepare(MyBase, pos, end - pos, &st, &pos);	//Выделение, соостветсвенно, первого запроса в строке

		while ((rc = sqlite3_step(st)) == SQLITE_ROW)				//Вывод строчек текущей части распарсенного запроса				
		{
			int	i;
			for (i = 0; i < sqlite3_column_count(st); ++i)
				printf("%s", sqlite3_column_text(st, i));
			printf("\n");
		}

		if (rc != SQLITE_DONE && rc != SQLITE_ROW)					//Если вдруг возникла какая-то ошибка, то вывести её
		{
			printf("Error : %s\n", sqlite3_errmsg(MyBase));
			system("pause");
			return -1;
		}

		sqlite3_finalize(st);										//Что-то подобное free, очистить план выполнения для step'а
	}
	return 0;
}
int Processing()
{
	char Queries[MAX_STRING_SIZE];
	char Command[MAX_STRING_SIZE];
	char BaseName[20] = "";

	printf("Type \"exit\" if you want to finish program.\n");
	FILE * in; sqlite3 *MyBase;
	bool OK = true;
	while (strcmp(Command, "exit"))
	{
		printf("Command: ");
		scanf("%s", Command);

		if (!strcmp("open", Command))
		{
			if (opened(BaseName)) sqlite3_close(MyBase);
			scanf("%s", BaseName);
			strcat(BaseName, ".sqlite3");
			int rc = sqlite3_open(BaseName, &MyBase);
			if (rc)
			{
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(MyBase));
				sqlite3_close(MyBase);
				BaseName[0] = '\0';
			}
			continue;
		}

		if (!strcmp("perform", Command))
		{
			if (!opened(BaseName))
			{
				printf("DataBase %s wasn't opened...", BaseName);
				continue;
			}
			char type[10];
			scanf("%s", type);
			if (!strcmp("sql", type))
			{
				scanf("%s", Queries);
				PerformQueries(MyBase, Queries);
			}
			if (!strcmp("file", type))
			{
				char filename[40] = "";
				scanf("%s", filename);
				FILE* in = fopen(filename, "r");
				if (in == NULL)
				{
					printf("Error opening file %s", filename);
					continue;
				}
				char c; int i = 0;
				while ((c = fgetc(in)) != EOF) Queries[i++] = c;
				Queries[i] = '\0';
				PerformQueries(MyBase, Queries);
			}
			continue;
		}
		if (!strcmp("sql", Command))
		{
			char t; int i = 0;
			while ((t = getchar()) != '\n') Queries[i++] = t;
			Queries[i] = '\0';
			PerformQueries(MyBase, Queries);
			continue;
		}
		if (!strcmp("close", Command))
		{
			if (opened(BaseName))
			{
				sqlite3_close(MyBase);
				BaseName[0] = "\0";
			}
			continue;
		}
		if (!strcmp("insert", Command))
		{
			FILE* insert = fopen("insert.txt", "r");
			char TableName[MAX_STRING_SIZE];
			printf("TableName --> ");
			scanf("%s", TableName);
			printf("To finish entering params print '_ok'\n");
			printf("Params --> "); 
			char str[MAX_STRING_SIZE]; 
			char param[MAX_STRING_SIZE];
			scanf("%s", param);
			sprintf(str, "INSERT INTO %s VALUES(", TableName);
			if (strcmp("_ok", param))
			{
				strcat(str, "'");
				strcat(str, param);
				strcat(str, "'");
			}
			scanf("%s", param);
			while (strcmp("_ok", param))
			{
				strcat(str, ", '");
				strcat(str, param);
				strcat(str, "'");
				scanf("%s", param);
			}
			strcat(str, ");");
			PerformQueries(BaseName, str);
			

			continue;
		}
		if (!strcmp("create", Command))
		{
		}
		if (!strcmp("create", Command))
		{
		}
		if (!strcmp("create", Command))
		{
		}

		printf("Unknown command: %s\n", Command);
	}


	return 0;
}