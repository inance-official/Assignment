/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name ���� ����
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
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
              const void *bb - ���� ������
RETURNED    : strCmp()�� ��ȯ��
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
DESCRIPTION : ���� �� ���ڵ� �� ��ȯ
PARAMETERS  :
RETURNED    : num - ���� �� ���ڵ� ��
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
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : CODENAME *cnlist - �ε��� �����͸� ������ �޸�
              int num          - ���� �� ���ڵ� ��
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


	/*--- �Է� ---*/
	fd = open("./file.txt", O_RDWR | O_CREAT | O_APPEND, 0777); //������ ������ �а� ���� ����
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

		/*--- key �Է� ---*/
		printf("Input Key[9] : ");
		scanf("%s", temp);
		memcpy(knlist->key, temp, strlen(temp));

		if(strCmp(knlist->key, "q") == 0)
		{
			if(knlist != NULL)
				free(knlist);

			break;
		}

		/*--- Name �Է� ---*/
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


    /*--- ��ü ������ �޸� �ε� �� ���� --*/
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

    // �迭�� ����
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
DESCRIPTION : ���� �Լ�
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



