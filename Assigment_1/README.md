#file
# Question
9자리 code와 40자리 name을 파일에 쓰고 읽어오고 code를 기준으로 검색 가능한 프로그램 만들기

- file.c
- file.txt
- 메모리 → 파일
	- 파일에 쓰기 전에 몇 개의 레코드를 받을지 입력
	- file.txt에는 입력한 데이터만 존재
	- 프로그램을 다시 실행시켜 Insert하면 file.txt 초기
- 쓰기 / 읽기 / 검색
- `fopen()` / `fclose()`
- 각 사용자 정의 함수의 주석
- 사용자 함수 내부에서 Conditional & loop의 주석
- Makefile로 컴파일

```c
#include <stdio.h>

/*--- define ---*/

/*--- struct ---*/

/*--- global ---*/

/*******************************************************************************
* FUNCTION:    Insert
* DESCRIPTION: 레코드 형식으로 추가하는 함수
*******************************************************************************/
int Insert()
{

}

/*******************************************************************************
* FUNCTION:    Read
* DESCRIPTION: file.txt에서 console화면으로 읽어옵니다.
*******************************************************************************/
int Read()
{

}

/*******************************************************************************
* FUNCTION:    Search
* DESCRIPTION: Code를 입력해 검색하는 함수
*******************************************************************************/
int Search()
{

}

/*******************************************************************************
* FUNCTION:    main
* DESCRIPTION:
*******************************************************************************/
int main()
{

}
```

# Input
- 9자리 code
- 40자리 name
## Input example

### Insert
```bash
$ ./file

How many storage : 4
------------------
[1] Insert
[2] Read
[3] Search
[4] Exit
------------------
Choose Menu : 1
------------------

[ Insert a new Code & Name ]

Code(9)  : 1
name(40) : a

```

### file.txt
```txt
1           a
2           b
3           c
4           d
```
### Read
```bash
------------------
[1] Insert
[2] Read
[3] Search
[4] Exit
------------------
Choose Menu : 2
------------------

[ Read the list (file.txt) ]

[Code]   	[Name]
1        	a
2        	b
3        	c
4        	d
```

### Search
```bash
------------------
[1] Insert
[2] Read
[3] Search
[4] Exit
------------------
Choose Menu : 3
------------------

[ Enter the code to search for a name. ]

Search Code[9] : 2

[Code]   	[Name]
2        	b
```

### Exit
```bash
------------------
[1] Insert
[2] Read
[3] Search
[4] Exit
------------------
Choose Menu : 4
------------------

[ Exit the Program. ]
```


## Makefile
```Makefile

```