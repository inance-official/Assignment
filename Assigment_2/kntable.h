/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  : 헤더파일
* DATE         :
* AUTHOR       :
******************************************************************************/

typedef struct tm NOWTIME;

#define KEYSIZE 9
#define NAMESIZE 40

typedef struct _KEYNAME
{
    char key[KEYSIZE + 1];
    char name[NAMESIZE + 1];
} KEYNAME;

