/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  : �������
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define KEYSIZE 9
#define NAMESIZE 40

/*--- struct ---*/
typedef struct _KEYNAME
{
    char code[KEYSIZE + 1];
    char name[NAMESIZE + 1];
} KEYNAME;
