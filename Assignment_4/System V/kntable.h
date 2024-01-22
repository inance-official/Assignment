/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  : define과 struct를 정의한 헤더 파일
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define KEYSIZE 9
#define NAMESIZE 40

#ifndef KNLIST_H
#define KNLIST_H

/*--- struct ---*/
typedef struct KEYNAME
{
    char key[KEYSIZE];
    char name[NAMESIZE];
} KEYNAME;

extern KEYNAME *g_KnList;

#endif /* KNLIST_H */

