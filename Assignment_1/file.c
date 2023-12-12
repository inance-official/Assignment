#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define codeSize 10
#define nameSize 41

typedef struct LIST
{
	char code[codeSize];
	char name[nameSize];
} LIST;

LIST 	*g_List 	= NULL;
int 	 g_LIdx 	= 0;


/*******************************************************************************
* FUNCTION:    Insert
* DESCRIPTION: 레코드 형식으로 추가하는 함수
*******************************************************************************/
int Insert()
{
	FILE 	*fp;
	int		 ii = 0;

	fp = fopen("./file.txt", "w");
	//파일 열기 실패시
	if (fp == NULL)
	{
		printf("fopen error[%d]\n", errno);
		return -1; //실패시 -1 반환
	}
	//Code 입력
	printf("\n");
	printf("Input Code[9] : ");
	scanf("%s", g_List[g_LIdx].code);

	//Name 입력
	printf("Input Name[40] : ");
	scanf("%s", g_List[g_LIdx].name);

	//index증가
	g_LIdx++;

	//file.txt에 입력
	for (ii = 0; ii < g_LIdx; ii++)
		fprintf(fp, "%-9s\t%-50s\n", g_List[ii].code, g_List[ii].name);

	fclose(fp);

	return 1;
}

/*******************************************************************************
* FUNCTION:    Read
* DESCRIPTION: file.txt에서 console화면으로 읽어옵니다.
*******************************************************************************/
int Read()
{
	FILE 	*fp;

	char 	 rbuff[128] = {0,};

	fp = fopen("./file.txt", "r");
	//파일 열기 실패시
	if (fp == NULL)
	{
		printf("fopen error[%d]\n", errno);
		return -1; //실패시 -1 반환
	}

	printf("\n");
	printf("%-9s\t%-50s\n", "[Code]", "[Name]");

	while (fgets(rbuff, sizeof(rbuff), fp))
	{
		printf("%s", rbuff);

		//파일의 끝에 도달하면 반복문 멈춤
		if (feof(fp))
			break;
	}

	fclose(fp);

	return 1;
}


/*******************************************************************************
* FUNCTION:    Search
* DESCRIPTION: Code를 입력해 검색하는 함수
*******************************************************************************/
int Search()
{
	FILE 	*fp;

	char 	 ibuff[codeSize] = {0,};
	char	 sbuff[128] 	 = {0,};

	fp = fopen("./file.txt", "r");
	//파일 열기 실패시
	if (fp == NULL)
	{
		printf("fopen error[%d]\n", errno);
		return -1; //실패시 -1 반환
	}

	//검색할 코드 입력
	printf("\n");
	printf("Search Code[9] : ");
	scanf("%s", ibuff);

	printf("\n");
	printf("%-9s\t%-50s\n", "[Code]", "[Name]");

	//파일 끝까지 한 줄씩 읽어오면서 입력한 코드와 비교해서 같으면 출력 후 반복문 멈춤
	while (!feof(fp))
	{
		fgets(sbuff, sizeof(sbuff), fp);

		if (memcmp(sbuff, ibuff, strlen(ibuff)) == 0)
		{
			printf("%s", sbuff);
			break;
		}
	}

	fclose(fp);

	return 1;
}

/*******************************************************************************
* FUNCTION:    main
* DESCRIPTION:
*******************************************************************************/
int main()
{
	int		lcnt   = 0;		// Record
	int	 	uopt   = 0;		// Menu 선택
	bool 	escape = true;	// 반복문 탈출

	/*--- 저장할 레코드 개수 설정 ---*/
	printf("\n");
	printf("How many storage : ");
	scanf("%d", &lcnt);

	/*--- 구조체 동적 할당 및 초기화 ---*/
#if 0
	/* malloc */
	g_List = (LIST *)malloc(sizeof(LIST) * lcnt);
	memset(g_List, 0x00, sizeof(LIST) * lcnt);
#else
	/* calloc */
	g_List = (LIST *)calloc(lcnt, sizeof(LIST));
#endif

	/*--- Menu ---*/
	while (escape)
	{
		printf("------------------\n");
		printf("[1] Insert        \n");
		printf("[2] Read          \n");
		printf("[3] Search        \n");
		printf("[4] Exit          \n");
		printf("------------------\n");

		printf("Choose Menu : ");
		scanf("%d", &uopt);
		printf("------------------\n");

        switch (uopt)
        {
            case 1:
                printf("\n");
                printf("[ Insert a new Code & Name ]            \n");
                Insert();
                printf("\n");
                break;

            case 2:
                printf("\n");
                printf("[ Read the list (file.txt) ]            \n");
                Read();
                printf("\n");
                break;

            case 3:
                printf("\n");
                printf("[ Enter the code to search for a name. ]\n");
                Search();
                printf("\n");
                break;

            case 4:
                printf("\n");
                printf("[ Exit the Program. ]                   \n");
                printf("\n");
                escape = false;
                break;
            //  return 0;

            default :
                printf("\n");
                printf("[ This is an unknown command. ]        \n");
                printf("\n");
        }
    }

	/*--- 동적 할당 해제 ---*/
	free(g_List);

	return 0;
}