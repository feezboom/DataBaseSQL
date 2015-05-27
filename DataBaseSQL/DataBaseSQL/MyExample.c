#include "Database.h"

int temp()
{

//��������� ���� ��� ������ �, ���� � ���
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
	//������� �����������(����� �������� � ����������)(��������� ������ �������!!!!!!!!)

	
//������ ��� ������� �� ����� - �������� ���� �������� � �������� �� ����������
	char* sQueries = (char*)malloc(MAX_FILE_SIZE*sizeof(char));
	memset(sQueries, 0, MAX_FILE_SIZE);


	FILE * in = fopen("table.txt", "r");
	int i = -1;
	while (!feof(in) && i++ > -5)
		sQueries[i] = fgetc(in);

	sQueries[i] = '\0';
	printf("string :\n%s\n", sQueries);

	//��������� ����������, ������ ��� ������������� ������ prepare'��
	const char *pos= sQueries;
	const char *end = sQueries + strlen(sQueries);
	//�������������� �� ������� �, ���������������, ������ �������������� ����� ������

	//��� ���������� ��� �������� ����� ���������� �������, � �� prepare ������� ���� ����, � �������� ��� step
	sqlite3_stmt *st;
	//���� ���������� ���������� �������, � �������, prepare, step, column_text, column_count, reset() �� ��� ������...
	//������ ���������� �� http://www.sqlite.org/capi3ref.html
	//����� ���� �������� ���� �������, �������� � ������� � sqlite3
	//���������: http://vk.com

	rc = SQLITE_ROW;
	while ((pos < end - 1))	//������������� ��������, ������������� � ����� ������ sQueries
	{
		rc = sqlite3_prepare(
			MyBase,			//���� ���� ������
			pos,			//������ �������������� ����� �������
			end - pos,		//������ �������������� ����� �������
			&st,			//��� �����, ���� ���������� ��� �����
			&pos);			//Pos ������ ������� ���������� �������
		
		while ((rc = sqlite3_step(st)) == SQLITE_ROW)
		{
																		//��� ���� ������� sqlite3_reset(st), ������� ���������� ���� ���������� st � ������ 
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
		sqlite3_finalize(st);											//���-�� �������� free, �������� ���� ���������� ��� �����

	}
	

	printf("Type exit if want to exit...\n");
	sQueries[0] = '\0';

	while (1)															//���� ���������� ���������������� ��������
	{
		int i = 0, l = 0; sQueries[0] = '\0';							//���� � ������������� ����������
		char t = 34;													//���� � ������������� ����������
		char *temp = (char*)malloc(MAX_STRING_SIZE*sizeof(char));		//���� � ������������� ����������

		printf("SQLite3 Query --> ");									//���� ������� � ����������
		while ((t = getchar()) != '\n')									//���� �������
			sQueries[i++] = t;											//���� �������
		sQueries[i] = '\0';												//���� �������
		
		if (!strcmp(sQueries, "exit"))									//���� ������������ ������ �����
			break;

		pos = sQueries;													//������ �������������� ����� ��������
		end = sQueries + strlen(sQueries);								//����� ���� ��������
		
		while (pos < end - 1)											//������������� ��������, ������������� � ����� ������ sQueries
		{
			rc = sqlite3_prepare(MyBase, pos, end - pos, &st, &pos);	//���������, ��������������, ������� ������� � ������

			while ((rc = sqlite3_step(st)) == SQLITE_ROW)				//����� ������� ������� ����� ������������� �������				
			{
				int	i;
				for (i = 0; i < sqlite3_column_count(st); ++i)
					printf("%s", sqlite3_column_text(st, i));
				printf("\n");
			} 

			if (rc != SQLITE_DONE && rc != SQLITE_ROW)					//���� ����� �������� �����-�� ������, �� ������� �
			{
				printf("Error : %s\n", sqlite3_errmsg(MyBase));
				system("pause");
				return 1;
			}

			sqlite3_finalize(st);										//���-�� �������� free, �������� ���� ���������� ��� step'�

		}
	}
	system("pause");
	return 0;
}