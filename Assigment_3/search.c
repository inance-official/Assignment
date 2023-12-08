/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : code, name 파일 검색
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
int Cmp(const void *aa, const void *bb)
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
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 데이터 로드
PARAMETERS  : CODENAME *cnlist - 로드한 데이터를 저장할 메모리
              int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME			*tkey         		   = NULL;
    int				 fd, rlen;
    char			 rbuff[BUFFER_SIZE]    = {0,};
    int				 ii;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

	for	(ii = 0; ii < num; ii++)
	{
		rlen = read(fd, rbuff, sizeof(KEYNAME));

        tkey = (KEYNAME *)rbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    close(fd);

	return 1;
}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : 검색 키에 해당하는 데이터 조회
PARAMETERS  : CODENAME *cnlist - 검색할 데이터가 저장된 메모리
              int num         - 검색할 데이터의 수
              char *code       - 검색할 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Search(KEYNAME *knlist, int num, char *key)
{
    KEYNAME 	 keyname;
    KEYNAME		*result;

    strcpy(keyname.key, key);

    result = (KEYNAME *)bsearch(&keyname, knlist, num, sizeof(KEYNAME), Cmp);
    if (result == NULL)
    {
        printf("\n");
        printf("Not Found. key[%s]\n", keyname.key);
        return -1;
    }
    else
    {
        printf("\n");
        printf("key : %-*.*s Name : %-*.*s\n",
            sizeof(result->key)-1, sizeof(result->key)-1, result->key,
            sizeof(result->name)-1, sizeof(result->name)-1, result->name);

        return 1;
    }
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    char    key[KEYSIZE] = {0,};
    int     user         = 0;
    int     num          = 0;
    int     rtn;

    while (1)
    {
        /*--- 메모리 할당 ---*/
        num = FileCount();

        g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * num);
        memset(g_KnList, 0x00, sizeof(KEYNAME) * num);

        /*--- 파일에서 데이터 로드 ---*/
        rtn = FileToMem(g_KnList, num);
        if (rtn < 0)
        {
            return -1;
        }

        /*--- 검색 키 입력받기 ---*/
        printf("\n");
        printf("[Search]\n");

        printf("\n");
        printf("Input key : ");
        scanf("%s", key);

        /*--- 검색 수행 ---*/
        rtn = Search(g_KnList, num, key);
        if (rtn < 0)
        {
            return -1;
        }

        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d", &user);

        if (user == 2)   // 2:no --> 프로그램 종료
            break;

        if (g_KnList != NULL)
            free(g_KnList);
  }

    return 0;
}

