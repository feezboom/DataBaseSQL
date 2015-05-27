#include "Database.h"

int temp()
{

//Открываем базу или создаём её, если её нет
	sqlite3 *MyBase;
	char BaseName[20] = "Russia.sqlite3";
	int rc = sqlite3_open(BaseName, &MyBase);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(MyBase));
		sqlite3_close(MyBase);
		return 1;
	}

//	char sQueries[] = "create table if not exists FederalAreas (Id int  NOT NULL primary key, name varchar(40));";	
	//Пробелы обязательны(между селектом и аргументом)(ПРАВИЛЬНО писать запросы!!!!!!!!)

	
//Строка под запросы из файла - создание всех табличек и возможно их заполнение
	char* sQueries = (char*)malloc(MAX_FILE_SIZE*sizeof(char));
	memset(sQueries, 0, MAX_FILE_SIZE);


	FILE * in = fopen("table.txt", "r");
	int i = -1;
	while (!feof(in) && i++ > -5)
		sQueries[i] = fgetc(in);

	sQueries[i] = '\0';
	printf("string :\n%s\n", sQueries);

	//Временные переменные, нужные для распарсивания строки prepare'ом
	const char *pos= sQueries;
	const char *end = sQueries + strlen(sQueries);
	//Инициализируем их началом и, соостветственно, концом нераспарсенной части строки

	//Это структурка для хранения плана выполнения запроса, в неё prepare запишет этот план, а выполнит его step
	sqlite3_stmt *st;
	//Этой структурой пользуются функции, к примеру, prepare, step, column_text, column_count, reset() ну еще многие...
	//Больше информации на http://www.sqlite.org/capi3ref.html
	//Здесь есть описания всех функций, констант и другого в sqlite3
	//Вконтакте: http://vk.com

	rc = SQLITE_ROW;
	while ((pos < end - 1))	//Распарсивание запросов, расположенных в одной строке sQueries
	{
		rc = sqlite3_prepare(
			MyBase,			//Наша база данных
			pos,			//Начало нераспарсенной части запроса
			end - pos,		//размер нераспарсенной части запроса
			&st,			//Для степа, план выполнения для степа
			&pos);			//Pos станет началом следующего запроса
		
		while ((rc = sqlite3_step(st)) == SQLITE_ROW)
		{
																		//Еще есть функция sqlite3_reset(st), которая возвращает план выполнения st в начало 
			int	i;
			for (i = 0; i < sqlite3_column_count(st); ++i)
				printf("%s ", sqlite3_column_text(st, i));
			printf("\n");
		}


		if (rc != SQLITE_DONE && rc != SQLITE_ROW)
		{
			printf("Error : %s\n", sqlite3_errmsg(MyBase));
			system("pause");
			return 1;
		}
		sqlite3_finalize(st);											//Что-то подобное free, очистить план выполнения для степа

	}
	

	printf("Type exit if want to exit...\n");
	sQueries[0] = '\0';

	while (1)															//Цикл выполнения пользовательских запросов
	{
		int i = 0, l = 0; sQueries[0] = '\0';							//Ввод и инициализация переменных
		char t = 34;													//Ввод и инициализация переменных
		char *temp = (char*)malloc(MAX_STRING_SIZE*sizeof(char));		//Ввод и инициализация переменных

		printf("SQLite3 Query --> ");									//Ввод запроса с клавиатуры
		while ((t = getchar()) != '\n')									//Ввод запроса
			sQueries[i++] = t;											//Ввод запроса
		sQueries[i] = '\0';												//Ввод запроса
		
		if (!strcmp(sQueries, "exit"))									//Если пользователь просит выйти
			break;

		pos = sQueries;													//Начало нераспарсенной части запросов
		end = sQueries + strlen(sQueries);								//Конец всех запросов
		
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
				return 1;
			}

			sqlite3_finalize(st);										//Что-то подобное free, очистить план выполнения для step'а

		}
	}
	system("pause");
	return 0;
}