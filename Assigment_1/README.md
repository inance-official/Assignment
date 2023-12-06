- [Question](#question)
- [Formatting](#formatting)
- [Input & Output example](#input--output-example)

# Question
이 프로젝트는 9자리의 코드와 40자리의 이름을 파일에 쓰고 읽어오며, 코드를 기준으로 검색할 수 있는 프로그램을 개발하는 것

프로그램은 다음과 같은 기능을 수행:

- 메모리 → 파일: 사용자로부터 입력받은 레코드의 개수를 기반으로 데이터를 파일에 쓰는 기능으로 file.txt에는 입력한 데이터만 존재해야 한다. 프로그램을 다시 실행하여 데이터를 삽입하면 file.txt는 초기화.

- 쓰기 / 읽기 / 검색: 파일에 데이터를 쓰고, 읽어오며, 코드를 기준으로 데이터를 검색하는 기능.

이 프로젝트는 `fopen()`과 `fclose()` 함수를 사용하여 파일을 열고 닫는 작업을 수행한다. 또한, 각 사용자 정의 함수와 조건문 및 루프 내부에는 적절한 주석을 포함해야 한다.
마지막으로, Makefile을 사용하여 프로그램을 컴파일해야 한다.

# Formatting

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

# Input & Output example
- 9자리 code
- 40자리 name

## Insert
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
## file.txt
```txt
1           a
2           b
3           c
4           d
```

## Read
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

## Search
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

## Exit
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