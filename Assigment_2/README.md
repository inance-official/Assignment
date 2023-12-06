# Question
file.txt에 쓰기 / 읽기 / 검색 / 삭제를 수행하는 각각의 main이 존재하는 프로그램을 만들고 구조체, define은 헤더파일로 빼서 만들어야 한다.

- 컴파일은 Makefile 사용

- **Source file**
	- insert.c
		- `qsort()` 사용
		- insert 후 code에 q입력 시 프로그램 종료
	- read.c
		- `system()`
			- 화면 clear
		- `sleep()`
			 - 2초에 한번 씩 화면 갱신
		- 현재 날짜 시간 출력
	- search.c
		- 하나 씩 검색
		- `bsearch()` 사용
		- 중복값 전부 출력
		- 검색 후 추가 검색 여부 확인
		- 검색 실패 시 검색 여부 확인
	- delete.c
		- 삭제 후 재배열
			- `qsort()`
		- key 중복 건에 대해서만 목록 출력
			- key 중복 건 중에서 이름 입력으로 삭제
		- 삭제 확인 여부 출력
	- kntable.h
	- file.txt
- **function**
	- `fopen()`
	- `fclose()`
	- `qsort()`
	- `bsearch()`

- **주의 사항**
	-  insert에서 동적 할당 값을 input로 받지 않는다
	- 정렬 필수
	- text파일에는 데이터만 존재
	- 각 함수에 대한 주석 작성 필수
	- 함수 내에서 거시적으로 주석 작성

# Formatting

## read.c
```c
/******************************************************************************
* PROGRAM NAME : read
* SOURCE NAME  : read.c
* DESCRIPTION  : 
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>

/******************************************************************************
FUNCTION    : ReadFile
DESCRIPTION : 
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
DESCRIPTION : 
PARAMETERS  : 
RETURNED    : 
******************************************************************************/
int Cmp()
{

}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 
PARAMETERS  :
RETURNED    : 
******************************************************************************/
int FileCount()
{

}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 
PARAMETERS  : 
RETURNED    : 
******************************************************************************/
int insert()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 
PARAMETERS  :
RETURNED    : 
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
DESCRIPTION : 
PARAMETERS  : 
RETURNED    : 
******************************************************************************/
int Cmp()
{

}

/******************************************************************************
FUNCTION    : 
DESCRIPTION : 
PARAMETERS  :
RETURNED    : 
******************************************************************************/
int FileCount()
{

}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 
PARAMETERS  : 
RETURNED    : 
******************************************************************************/
int FileToMem()
{

}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : 
PARAMETERS  : 
RETURNED    : 
******************************************************************************/
int Search()
{

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 
PARAMETERS  :
RETURNED    : 
******************************************************************************/
int main()
{

}
```
## delete.c
```c

```
## kntable.c
```c

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
Input name[40] : 11

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
Update key(2sec)
Date : 2023-12-06 12:44:28

[Key]    [Name]
1        a
1        11
2        b
3        c
4        d
```

## search
```bash
---------------------------------

    Enter a key to find name

---------------------------------

Input key(9) : 1

---------------------------------
key : 1         Name : a
key : 1         Name : 11
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

