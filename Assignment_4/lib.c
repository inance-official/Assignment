/******************************************************************************
 * FILE NAME : lib.c
 * DESCRIPTION : 공유 메모리 함수
 * DATE : 2020/09/04
 * AUTHOR : JH
 * ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>

#include "kntable.h"
#include "glomem.h"

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

/*--- shared memory 내부의 레코드 개수 ---*/
/******************************************************************************
FUNCTION    : CountShm
DESCRIPTION : 공유 메모리 내의 레코드 개수
PARAMETERS  :
RETURNED    : count - 레코드 개수
******************************************************************************/
int CountShm()
{
	int count = 0;

	/*--- Entries 개수 ---*/
	while (strcmp(g_KnList[count].key, "") != 0)
	{
		while (strcmp(g_KnList[count].key, "") == 0)
		{
			break;
		}
	    count++;
	}

#if 0
	while (g_KnList[count].key != '\0')
	{
		while (g_KnList[count].key == '\0')
		{
			break;
		}

		count++;
	}
#endif

	return count;
}

/******************************************************************************
FUNCTION    : InsertShm
DESCRIPTION : 공유 메모리에 데이터 입력
PARAMETERS  :
RETURNED    :  1(SUCCESS)
******************************************************************************/
int lInsertShm()
{
	int 		shmid;
	int			count		= 0;
	int			ii;
	int			rtn;
	KEYNAME    *tkey  		= NULL;
	char		temp[128] 	= {0,};

	rtn = GlobalCheckShm();
	if (rtn < 0)
		GlobalCreateShm();
	else
		printf("create error!\n");

	g_KnList = GlobalAttachShm();
	if (g_KnList == NULL)
		printf("None exist Shared Memory [%d]\n", errno);
	else
		printf("Attach Success\n");

	count = GlobalCountShm();

	while (1)
	{
	    printf("\n");

	    /*--- key 입력 ---*/
	    printf("Input Key[9] : ");
	    scanf("%s", temp);
	    if (strcmp(temp, "q") == 0)
			break;
	    else
			memcpy(g_KnList[count].key, temp, strlen(temp));

	    /*--- Name 입력 ---*/
	    printf("Input Name[40] : ");
	    scanf("%s", temp);
	    memcpy(g_KnList[count].name, temp, strlen(temp));

	    count++;
	    if (count >= MAX_ENTRIES)
	    {
			printf("Maximum number of entries reached.\n");
			break;
	    }
	}

	/*--- 데이터를 정렬 ---*/
	qsort(g_KnList, count, sizeof(KEYNAME), Cmp);

	DetachShm(g_KnList);

	return 1;
}

/*--- System V shared memory read ---*/
/******************************************************************************
FUNCTION    : ReadShm
DESCRIPTION : 공유 메모리에 데이터 출력
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lReadShm()
{
	int			ii;
	int			count		= 0;
	int			rtn;
	char		tbuff[64]	= {0,};
	time_t      now			= time(NULL);
	struct		tm  *tt		= NULL;

	g_KnList = GlobalAttachShm();
	if (g_KnList == NULL)
	{
		printf("Fail AttachShm![%d] : %s\n", errno, strerror(errno));
	}
	else
	{
		printf("Attach Success\n");
	}

	tt = localtime(&now);
	strftime(tbuff, sizeof(tbuff), "%Y-%m-%d %H:%M:%S", tt);
	printf("Update key(2sec) : %s\n", tbuff);

	count = CountShm();
	printf("Count [%d]\n", count);

	printf("\n");
	printf("%-9s%-50s\n","[Key]", "[Name]");

	for (ii = 0; ii < count; ii++)
	{
	    printf("%-*.*s%-*.*s\n",
			sizeof(g_KnList[ii].key)-1, sizeof(g_KnList[ii].key)-1, g_KnList[ii].key,
			sizeof(g_KnList[ii].name)-1, sizeof(g_KnList[ii].name)-1, g_KnList[ii].name);
	}

	if (count == 0)
	{
		printf("None Data in Shared Memory\n");
	}

	GlobalDetachShm(g_KnList);

	return 1;
}

/*--- shared memory memmove ---*/
/******************************************************************************
FUNCTION    : SearchShm
DESCRIPTION : 공유 메모리에 데이터 검색
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lSearchShm()
{
	int			shmid;
	int			ii;
	int			count      = 0;
	int			rtn;
	int			keycmp     = 0;
	char	    skey [128] = {0,};
	KEYNAME		keyname;
	KEYNAME    *result;

	g_KnList = GlobalAttachShm();
	if (g_KnList == NULL)
	{
		printf("Fail GlobalAttachShm![%d] : %s\n", errno, strerror(errno));
	}
	else
	{
		printf("GlobalAttach Success\n");
	}

	/*--- 공유 메모리 내의 레코드 개수 ---*/
	count = CountShm();

	/*--- 검색할 키 입력 ---*/
	printf("Enter the key of the record to search: ");
	scanf("%s", skey);

	strcpy(keyname.key, skey);

	result = (KEYNAME *)bsearch(&keyname, g_KnList, count, sizeof(KEYNAME), Cmp);
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
    }

#if 0
	for (ii = 0; ii < count; ii++)
	{
	    keycmp = strcmp(g_KnList[ii].key, skey);

	    if (keycmp == 0)
	    {
			printf("				\n");
			printf("Search Success	\n");
			printf("key : %s name : %s \n", g_KnList[ii].key, g_KnList[ii].name);
			printf("				\n");

			break;
	    }
	}

	if(keycmp != 0)
	{
	    printf("No matching record found.\n");
	}
#endif

	GlobalDetachShm(g_KnList);

	return 1;
}

/*--- shared memory memmove ---*/
/******************************************************************************
FUNCTION    : DeleteShm
DESCRIPTION : 공유 메모리에 데이터 삭제
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lDeleteShm()
{
	int		shmid;
	int		ii;
	int		count			= 0;
	int		rtn;
	int		keycmp			= 0;
	char	delkey[128]     = {0,};

	g_KnList = GlobalAttachShm();
	if (g_KnList == NULL)
	{
		printf("Fail GlobalAttachShm![%d] : %s\n", errno, strerror(errno));
	}
	else
	{
		printf("Attach Success\n");
	}

	/*--- 공유 메모리 내의 레코드 개수 ---*/
	count = CountShm();
	printf("Count [%d]\n", count);

	/*--- 삭제할 키 입력 ---*/
	printf("Enter the key of the record to delete: ");
	scanf("%s", delkey);

	for (ii = 0; ii < count; ii++)
	{
	    keycmp = strcmp(g_KnList[ii].key, delkey);

	    if (keycmp == 0)
	    {
			printf("				\n");
			printf("Delete Success	\n");
			printf("key : %s name : %s \n", g_KnList[ii].key, g_KnList[ii].name);

			memmove(&g_KnList[ii], &g_KnList[ii+1], sizeof(KEYNAME) * (MAX_ENTRIES - (ii+1)));
			//memmove(&g_KnList[ii], &g_KnList[ii+1], sizeof(KEYNAME) * (count - ii));
			count--;

			printf("\n");

			break;
	    }
	}

	if (keycmp != 0)
	{
	    printf("No matching record found.\n");
	}

	GlobalDetachShm(g_KnList);

	return 1;
}
