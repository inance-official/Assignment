/******************************************************************************
* PROGRAM NAME : delete
* SOURCE NAME  : delete.c
* DESCRIPTION  : 파일에서 데이터 삭제
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

KEYNAME   *g_KnList = NULL;

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int cmp(const void *aa, const void *bb)
{
    KEYNAME *k1 = (KEYNAME *)aa;
    KEYNAME *k2 = (KEYNAME *)bb;

    return strcmp(k1->key, k2->key);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileCount()
{
    int         fd, rlen;
    char        rbuff[BUFFER_SIZE] = {0,};
    int         num                = 0;
    int         ii                 = 0;

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
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME         *tkey                  = NULL;
    int              fd, rlen;
    char             rbuff[BUFFER_SIZE]    = {0,};
    int              ii;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    for (ii = 0; ii < num; ii++)
    {
        rlen = read(fd, rbuff, sizeof(KEYNAME));

        tkey = (KEYNAME *)rbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : Delete
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  : CODENAME *cnlist - 삭제할 데이터가 저장된 메모리
              int count        - 메모리 내 데이터 수
              char *delkey     - 삭제할 데이터의 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(KEYNAME *knlist, int count, char *delkey)
{
    int          fd;
    int          kcmp   = 0;
    int          ii, jj;


    for (ii = 0; ii < count; ii++)
    {
    	kcmp = strcmp(knlist[ii].key, delkey);

        if (kcmp == 0)
        {
            printf("\n");
            printf("Key : %s\tName : %s\n", knlist[ii].key, knlist[ii].name);

            memmove(&knlist[ii], &knlist[ii+1], sizeof(KEYNAME) * (count - ii));
            count--;

            printf("\n");

			//배열을 정렬
			qsort(knlist, count, sizeof(KEYNAME), cmp);

            /*--- 전체 데이터 신규 저장 --*/
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

    		close(fd);

			printf("Delete Complete\n");
		}
	}

    if(kcmp != 0)
    {
        printf("Not Found\n");
    }

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
    char    delkey[KEYSIZE]  = {0,};
    int     user             = 0;
    int     rtn;
    int     count            = 0;

    while (1)
    {
        /*--- 메모리 할당 ---*/
        count = FileCount();

        g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
        memset(g_KnList, 0x00, sizeof(KEYNAME) * count);

        /*--- 파일에서 데이터 로드 ---*/
        rtn = FileToMem(g_KnList, count);
        if (rtn < 0)
        {
            return -1;
        }

        /*--- 키 입력 ---*/
        printf("\n");
        printf("[Delete]\n");
        printf("\n");
        printf("Input key : ");
        scanf("%s", delkey);

        /*--- 삭제하고 재배열 ---*/
        rtn = Delete(g_KnList, count, delkey);
        if (rtn < 0)
        {
            return -1;
        }

        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d", &user);

        if (user == 2)  // (2)no --> 프로그램 종료
		{
            break;
		}

        if (g_KnList != NULL)
		{
            free(g_KnList);
		}
    }

    return 0;
}

