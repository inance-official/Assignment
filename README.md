
Assigment_1 File I/O - f series

Assigment_2 Separate one program into individual programs

Assigment_3 File I/O - f serise 2 universal I/O model

Assigment_4 SHM - one main

Assigment_5 file disk speed test

Assigment_6 mini Program

## Table

- [Naming](#naming)
- [Variable](#variable)
- [Conditional & Loop](#conditional--loop)
- [Custom function](#custom-function)
- [Fomatting](#fomatting)
- [Header File](#header-file)

# Coding style (Rule)

- Assigment를 해결하기 위해 지켜야 할 style

## Naming
- 길이는 최대한 30자 이내로 정의
- lower case (소문자 형식)
```C
variableismemory
```
- upper case (대문자 형식)
```c
VARIABLEISMEMORY
```
- pascal case (파스칼 형식)
```c
VariableIsMemory
```
- snake case (뱀 형식)
```c
variable_is_memory
```
- Screaming Snake Case
```c
VARIABLE_IS_MEMORY
```

## Variable
- 기본적으로 한 줄에 하나의 정의만 작성
- 모든 전역변수(Global)변수는 초기화 값 필수
```c
char        g_Name     [128] = {0,};
int         g_Len            = 0;
```
- 변수의 이름은 고유해야 한다
    - `ii` `jj` `tmp` `len` 과 같은 임시 변수의 경우는 제외
-  단일 문자는 사용 금지
    - `i` ( X )→ `ii`
    - `j` ( X )→ `jj`
    - `a` `b` `c`→ 절대 사용 금지
- 추측 가능한 변수의 이름 사용
- 줄 맞추기
```c
char    code[3]   = {0,};    //char형 변수
char    code[3+1] = {0,};    //char형 변수 + string
char    *code     = NULL;    //char형 변수 + pointer
```


## Conditional & Loop
```c
if (condition)
{
    contents;
}
```

```c
if (condition1 ||
    condition2)
{
    contents;
}
```

```c
if (condition)
{

}
else
{

}
```

```c
while (condition)
{
    contents;
}
```

```c
for (init; condition; step)
{
    contents;
}
```

```c
switch (variable)
{
    case 0:
        contents;
        break;
    case 1:
        contents;
        break;
    default:
        contents;
        break;
}

switch (variable)
{
    case 0:  contents; break;
    case 1:  contents; break;
    default: contents; break;
}
```

- contents가 한 줄인 경우
```c
if (condition)
    contents;

while (condition)
    contents;

for (init; condition; step)
    contents;
```

```c
if (condition) contents;

while (condition) contents;

for (init; condition; step) contents;

```
## Custom function
- 기본적으로 pascal case을 사용
- 영문 합성 약어(영문 첫 글자만 모아 놓은 약어)인 경우  upper case을 사용해도 무방
- 함수의 이름으로 역할 추측 가능 요구
```c
/******************************************************************************
FUNCTION    : Customfunction
DESCRIPTION :
PARAMETERS  : int AAA - 설명
              int BBB - 설명
              int CCC - 설명
              int DDD - 설명
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Customfunction(int AAA, int BBB, int CCC, int DDD)
{

}
```

```c
/******************************************************************************
FUNCTION    : Customfunction
DESCRIPTION :
PARAMETERS  : int AAA - 설명
              int BBB - 설명
              int CCC - 설명
              int DDD - 설명
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Customfunction(int AAA, int BBB,
                   int CCC, int DDD)
{

}
```

```c
/******************************************************************************
FUNCTION    : Customfunction
DESCRIPTION :
PARAMETERS  : int AAA - 설명
              int BBB - 설명
              int CCC - 설명
              int DDD - 설명
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Customfunction(int AAA,
                   int BBB,
                   int CCC,
                   int DDD)
{

}
```

## Fomatting
- 한 줄에 최대 80글자까지 허용
    - 예외의 경우 80라인 초과 가능
        - 파일 경로
        - URL
        - printf()
        - 주석
- Tab의 크기는 4 space
- 기본적으로 BSD(Allman) style 적용
```c
/******************************************************************************
FILE NAME   :
DESCRIPTION : main
DATA        :
******************************************************************************/
#include <stdio.h>

int main()
{
    if (true)
    {
        printf("Hello, World!\n");
    }
    else
    {
        printf("Goodbye, World!\n");
    }

    return 0;
}
```

## Header File
- 구조체 테이블 및 매크로 헤더 파일 형식
- 공백만 사용
- 줄 끝 공백 제거 요구
- 여백 제거 요구

- define.h
```c
/******************************************************************************
* SOURCE FILE NAME : define.h
* DESCRIPTION      :
* DATA             :
******************************************************************************/
#ifndef _DEFINE_H_       // 중복 정의 방지 매크로 시작
#define _DEFINE_H_

/*--- define ---*/
#define NUMBER         1
#define NUMBER_KEY    10

#endif /*_DEFINE_H_*/    // 중복 정의 방지 매크로 끝
```

- protype.h
```c
/******************************************************************************
* SOURCE FILE NAME : protype.h
* DESCRIPTION      :
* DATA             :
******************************************************************************/
#ifndef  _PORTO_H_         //중복 정의 방지 매크로 시작
#define  _PORTO_H_

#include <stdio.h>         // 참조할 헤더파일

/*--- PROTO TYPE ---*/
#ifdef __cplusplus         // C++에서 C의 함수를 인식시키기 위한 매크로 시작
extern "C"
{
#endif

/* PROTO TYPE */           // 함수 선언문(Prototype) 리스트
int Customfunction()

#ifdef __cplusplus         // C++에서 C의 함수를 인식시키기 위한 매크로 끝
}
#endif

#endif /*_PORTO_H_*/       // 중복 정의 방지 매크로 끝
```

- table.h
```c
/******************************************************************************
* SOURCE FILE NAME : table.h
* DESCRIPTION      :
* DATA             :
******************************************************************************/
#ifndef _TABLE_H_          // 중복 정의 방지 매크로 시작
#define _TABLE_H_

#include "header.h"        // 참조할 헤더파일

typedef struct _TABLE
{
    int  AAA             ; /* Member name */
    char BBB       [10+1]; /* Member name */
} TABLE

#endif  /* _TABLE_H_ */
```