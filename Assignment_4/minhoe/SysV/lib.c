/******************************************************************************
 * FILE NAME   : lib.c
 * DESCRIPTION : ���� �޸� �Լ�
 * DATE        :
 * AUTHOR      :
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "kntable.h"
#include "glomem.h"
/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
              const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int Cmp(const void *a, const void *b)
{

    return strcmp(((KEYNAME*)a)->key,((KEYNAME*)b)->key);
}


/******************************************************************************
FUNCTION    : CountShm
DESCRIPTION : ���� �޸� ���� ���ڵ� ����
PARAMETERS  :
RETURNED    : count - ���ڵ� ����
******************************************************************************/
int CountShm()
{
    int rec_cnt = 0;
    int shm_id = 0;

    //�����޸��� id���� ������
    shm_id = shmget(SHM_KEY, 0, PERM);

    GlobalAttachShm();

    //key ���� �ԷµǾ����� ���, rec_cnt�� ����
    while (strcmp(g_KnList[rec_cnt].key,"") != 0)
    {
        rec_cnt++;
    }

    GlobalDetachShm();

    return rec_cnt;
}

/******************************************************************************
FUNCTION    : lInsertShm
DESCRIPTION : ���� �޸𸮿� ������ �Է�
PARAMETERS  :
RETURNED    :  1(SUCCESS)
******************************************************************************/
int lInsertShm()
{
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};

    int rec_cnt;

    //���� ���ڵ� ������ ������ 
    rec_cnt = CountShm();

    while(1)
    {
        printf("\nInsert Key & Name\n");
        printf("Input Key[9] : ");
        scanf("%9s", key_buf);

        while (getchar() != '\n'){}

        //q �Է½� ���α׷� ����    
        if (strcmp(key_buf,"q") == 0) break;

        //�ߺ� Ű �� ����� �Է� ����
        if (DupKeyChk(key_buf) < 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        GlobalAttachShm();

        //������ �Է�
        memcpy(g_KnList[rec_cnt].key, key_buf, sizeof(key_buf));
        memcpy(g_KnList[rec_cnt].name, name_buf, sizeof(name_buf));

        rec_cnt++;

        //�Էµ� ������ ����
        qsort(g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

        GlobalDetachShm();
    }

    return 1;
}
/******************************************************************************
FUNCTION    : DupKeyChk
DESCRIPTION : ���� �޸𸮿� ������ �Է½� �ߺ� Ű �� Ȯ��
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DupKeyChk(char *key_buf)
{
    int ii = 0;
    int rec_cnt = 0;

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm();

    //��� ���ڵ� Ž��
    for( ii=0; ii < rec_cnt; ii++)
    {
        //�Է��ϰ��� �ϴ� key ���� ���� ���� ���� �� ���
        if (strncmp(g_KnList[ii].key, key_buf, sizeof(key_buf)) == 0)
        {
            printf("A duplicate key exists.\n");
            printf("%-9s    %-s\n","[Key]", "[Name]");
            printf("%-9s    %-s\n", g_KnList[ii].key, g_KnList[ii].name);

            GlobalDetachShm();
            return -1;
        }
    }
    GlobalDetachShm();

    return 1;
}
/******************************************************************************
FUNCTION    : ReadShm
DESCRIPTION : ���� �޸� ������ ���
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lReadShm()
{
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};
    int rec_cnt;
    int ii = 0;
    time_t now;
    struct tm* t;

    while(1)
    {
        system("clear");

        // �ð� ���ϱ�
        now = time(NULL);
        t = localtime(&now);

        rec_cnt = CountShm();

        GlobalAttachShm();

        printf("Update Key (2sec)\n");
        printf("Date : %d-%02d-%02d %02d:%02d:%02d\n\n",t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
        printf("%-10s\t%-40s\n", "[Key]", "[NAME]");

        //���� �޸� ���� ������ ���
        for (ii = 0 ; ii < rec_cnt ; ii++)
        {
            printf("%-10s\t", g_KnList[ii].key);
            printf("%-40s\n", g_KnList[ii].name);
        }

        GlobalDetachShm();
        sleep(2);
    }

    return 1;
}

/******************************************************************************
FUNCTION    : SearchShm
DESCRIPTION : ���� �޸𸮿� ������ �˻�
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lSearchShm()
{
    int rec_cnt;
    KEYNAME find_kn;
    KEYNAME* kn_res = NULL;

    printf("---------------------------------");
    printf("\n\n    Enter a key to find name\n\n");
    printf("---------------------------------\n\n");

    //ã���� �ϴ� key �� �Է�
    printf("Input key(9) : ");
    scanf("%9s", find_kn.key);

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm();

    //bsearch�� �̿��Ͽ� ã�����ϴ� ���� kn_res�� ����
    kn_res = bsearch(&find_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    //���� ���� ���
    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",find_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }
    //���� ���� �� ��� �� ���
    else printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);

    GlobalDetachShm();

    return 0;
}
/******************************************************************************
FUNCTION    : DeleteShm
DESCRIPTION : ���� �޸𸮿� ������ ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lDeleteShm()
{
    int fin_sig;
    int del_sig;
    int ii;
    int rec_cnt;
    KEYNAME del_kn;
    KEYNAME *kn_res;

    printf("---------------------------------");
    printf("\n\n    Enter a key to delete\n\n");
    printf("---------------------------------\n\n");

    //������� �ϴ� key �� �Է�
    printf("Input key(9) : ");
    scanf("%9s", del_kn.key);

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm();

    //������� �ϴ� key �� �˻�
    kn_res = bsearch(&del_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    if (kn_res == NULL)
    {
        //���� ���� ���
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",del_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
        return -1;
    }
    //���� ���� �� ��� ���� ��Ȯ��
    printf("\n---------------------------------\n");
    printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);

    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    //���� Ȯ�� �� ����
    if (fin_sig == 1)
    {
        //���� �ش� ���ڵ� ��ġ Ž��
        for (ii = 0; ii < rec_cnt; ii++)
        {
            if (g_KnList[ii].key == kn_res -> key) break;
        }

        //���� �ش� ���ڵ���� �� ĭ�� ��ܼ� �Է�
        //������ ���ڵ�� ����
        for (; ii < rec_cnt - 1; ii++)
        {
            memcpy(g_KnList[ii].key, g_KnList[ii + 1].key, sizeof(g_KnList[ii + 1].key));
            memcpy(g_KnList[ii].name, g_KnList[ii + 1].name, sizeof(g_KnList[ii + 1].name));
            memset(&g_KnList[ii + 1],0x00,sizeof(KEYNAME));

        }
        printf("Delete Complete\n\n");
    }

    GlobalDetachShm();
}