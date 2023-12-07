- [Question](#question)
- [Formatting](#formatting)
- [Input & Output example](#input--output-example)

# Question
이 프로젝트는 file.txt 파일에 대해 쓰기, 읽기, 검색, 삭제 기능을 수행하는 프로그램을 만들어야 한다. 각 기능은 별도의 main 함수를 가진 소스 파일에 구현되야하며 구조체와 define은 헤더 파일에 포함되어야 한다.

- insert.c
	- `qsort()`를 사용하여 데이터를 삽입하고, 'q'를 입력하면 프로그램이 종료
- read.c
	- `system()`을 사용하여 화면을 지우고, `sleep()`을 사용하여 2초마다 화면을 갱신. 현재 날짜와 시간도 출력
- search.c
	- `bsearch()`를 사용하여 키를 검색하고, 중복된 키가 있으면 모두 출력. 검색이 끝나면 추가 검색 여부를 확인하고, 검색이 실패하면 다시 검색 여부를 확인
- delete.c
	- 데이터를 삭제하고 `qsort()`를 사용하여 재배열. 키가 중복된 경우에만 목록을 출력하고, 중복된 키 중에서 이름을 입력하여 삭제. 삭제 확인 여부를 출력
- kntable.h
	- 구조체와 define이 포함된 헤더 파일
- file.txt
	- 데이터만 저장된 파일

프로젝트에 필수적으로 사용해야할 함수는 `fopen()`, `fclose()`, `qsort()`, `bsearch()`이다. 또한, 동적 할당 값을 입력으로 받지 않아야 하며, 파일에 데이터를 쓸 때는 반드시 정렬해야 한다. 텍스트 파일에는 데이터만 있어야 하며, 각 함수에 대한 주석을 작성해야 하며 컴파일은 Makefile로 수행한다.

# Formatting
- 기본 형식
- 필요에 따라 사용자 지정 함수 변경 및 추가 삭제 가능

## read.c
```c
/******************************************************************************
* PROGRAM NAME : read
* SOURCE NAME  : read.c
* DESCRIPTION  : txt 파일 읽어오기
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>

/******************************************************************************
FUNCTION    : ReadFile
DESCRIPTION : txt 파일 읽어오기
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadFile()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{

}
```

## insert.c
```c
/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : key, name 파일 저장
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  :
RETURNED    :
******************************************************************************/
int Cmp()
{

}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    :
******************************************************************************/
int FileCount()
{

}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  :
RETURNED    :
******************************************************************************/
int insert()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{

}
```

## search.c
```c
/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : key, name 파일 검색
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  :
RETURNED    :
******************************************************************************/
int Cmp()
{

}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    :
******************************************************************************/
int FileCount()
{

}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 데이터 로드
PARAMETERS  :
RETURNED    :
******************************************************************************/
int FileToMem()
{

}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : 검색 키에 해당하는 데이터 조회
PARAMETERS  :
RETURNED    :
******************************************************************************/
int Search()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{

}
```

## delete.c
```c
/******************************************************************************
 * PROGRAM NAME : delete
 * SOURCE NAME  : delete.c
 * DESCRIPTION  : 파일에서 데이터 삭제
 * DATE         :
 * AUTHOR       :
 ******************************************************************************/
#include <stdio.h>

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  :
RETURNED    :
******************************************************************************/
int Cmp()
{

}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    :
****************************************************************************/
int FileCount()
{

}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  :
RETURNED    :
******************************************************************************/
int FileToMem()
{

}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : 정렬 및 저장
PARAMETERS  :
RETURNED    :
******************************************************************************/
int SortSave()
{

}

/******************************************************************************

FUNCTION    : Delete
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  :
RETURNED    :
******************************************************************************/
int Delete()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{

}
```

## kntable.c
```c
/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  : 헤더파일
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define KEYSIZE 9
#define NAMESIZE 40

/*--- struct ---*/
typedef struct _KEYNAME
{
	char key[KEYSIZE + 1];
	char name[NAMESIZE + 1];
} KEYNAME;

```

# Input & Output example

## insert
```bash
$ ./insert

Insert Key & Name
If you want to quit input key [q]

Input key[9]   : 1
Input name[40] : a

Input key[9]   : 1
Input name[40] : aa

Input key[9]   : 2
Input name[40] : b

Input key[9]   : 3
Input name[40] : c

Input key[9]   : 4
Input name[40] : d

Input key[9] : q

Exit the Program
```

## read

```bash
$./read
```

```bash
Update key(2sec)
Date : 2023-12-06 12:44:28

[Key]    [Name]
1        a
1        aa
2        b
3        c
4        d
```

## search
```bash
$ ./search

---------------------------------

    Enter a key to find name

---------------------------------

Input key(9) : 1

---------------------------------
key : 1         Name : a
key : 1         Name : aa
---------------------------------

Do you search more? (1)yes (2)no : 1

Input key(9) : 2

---------------------------------
key : 2         Name : b
---------------------------------

Do you search more? (1)yes (2)no : 1

Input key(9) : 12

---------------------------------
Not Found. key[12]
Search failed. Please try again.
---------------------------------

Do you search more? (1)yes (2)no : 2

```
## delete

```bash
$ ./delete

---------------------------------

  Enter a key to Delete recode

---------------------------------

Input key(9) : 1

---------------------------------
Key : 1	Name : a
Key : 1	Name : aa
---------------------------------

Enter the name to delete: a

Key : 1	Name : a

Are you sure? (1)yes (2)no : 1

Delete Complete

Save Complete

Do you search more? (1)yes (2)no : 1
---------------------------------

Input key(9) : 2

Key : 2	Name : b

Are you sure? (1)yes (2)no : 1

Delete Complete

Save Complete

Do you search more? (1)yes (2)no : 2
---------------------------------

```