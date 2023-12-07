/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name 파일 저장
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "kntable.h"

#define BUFFER_SIZE 9999

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strCmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
    KEYNAME *k1 = (KEYNAME *)aa;
    KEYNAME *k2 = (KEYNAME *)bb;

    return strCmp(k1->key, k2->key);
}

int DoubleChk()
{
	
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileCount()
{
	int			fd, rlen;
	char		rbuff[BUFFER_SIZE] = {0,};
	int			num				   = 0;
	int			ii				   = 0;

	fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

	while ((rlen = read(fd, rbuff, sizeof(KEYNAME))) > 0)
	{
		if (rlen == sizeof(KEYNAME))
			num++;
	}

	close(fd);

	return num;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : CODENAME *cnlist - 로드한 데이터를 저장할 메모리
              int num          - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int insert()
{
	int			 fd, rlen;

	KEYNAME		*knlist		 		= NULL, *tkey = NULL;
	int			 ii;
	char         rbuff[BUFFER_SIZE] = {0,};
	char       	 sbuff[64] 	 		= {0,};
	int			 count		 		= 0;
	char		 temp[128];


	/*--- 입력 ---*/
	fd = open("./file.txt", O_RDWR | O_CREAT | O_APPEND, 0777); //누구나 파일을 읽고 쓸수 있음
	if (fd < 0)
	{
		printf("file open error! error[%d]\n", errno);
		return -1;
	}

	while (1)
	{
		knlist = (KEYNAME *)malloc(sizeof(KEYNAME));
		memset(knlist, 0x00, sizeof(KEYNAME));

        printf("\n");

		/*--- key 입력 ---*/
		printf("Input Key[9] : ");
		scanf("%s", temp);
		memcpy(knlist->key, temp, strlen(temp));

		if(strCmp(knlist->key, "q") == 0)
		{
			if(knlist != NULL)
				free(knlist);

			break;
		}

		/*--- Name 입력 ---*/
		printf("Input Name[40] : ");
		scanf("%s", temp);
		memcpy(knlist->name, temp, strlen(temp));

		write(fd, knlist, sizeof(KEYNAME));

		if (knlist != NULL)
		{
			free(knlist);
		}
	}

	close(fd);


    /*--- 전체 데이터 메모리 로드 및 정렬 --*/
	fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

	count = FileCount();
	knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
    memset(knlist, 0x00, sizeof(KEYNAME) * count);

	ii = 0;

    while (rlen = read(fd, rbuff, sizeof(KEYNAME)) > 0)
    {
	   	tkey = (KEYNAME *)rbuff;
       	memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
		ii++;

		if (ii >= count)
			break;
    }

    close(fd);

    // 배열을 정렬
    qsort(knlist, count, sizeof(KEYNAME), Cmp);

	fd = open("./file.txt", O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

	for (ii = 0; ii < count; ii++)
    {
        write(fd, &knlist[ii], sizeof(KEYNAME));
    }

	free(knlist);

    close(fd);

	return 1;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");
    printf("       Insert Code & Name        \n");
    printf("                                 \n");
    printf("If you want to quit input code [q]\n");
    printf("---------------------------------\n");
    printf("                                 \n");

    insert();

    printf("                                 \n");
    printf("Exit the Program.                \n");
    printf("                                 \n");

    return 0;
}



