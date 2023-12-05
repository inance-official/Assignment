1. File I/O - f serise
2. 하나의 프로그램을 개별 프로그램으로 분리
3. File I/O - f serise 2 universal I/O model
4. SHM - 하나의 main
5. file disk speed test
6. mini 투자정보

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
- upper + snake case
```c
VARIABLE_IS_MEMORY
```

## Variable
- 기본적으로 한 줄에 하나의 정의만 작성
- 모든 전역변수(Global)변수는 초기화 값 필수
```
g_
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


## Conditionals & Loop
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
do
{
	contents;
}while (condition)

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
- 영문합성약어(영문 첫 글자만 모아놓은 약어)인 경우  upper case을 사용해도 무방
- 함수의 이름으로 역할 추측 가능 요구
```c
Customfunction()
```
