#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdio.h>
#include<string.h>
 
#define OK 1//没有出现错误，正常完成。
#define ERROR 0//出现了错误
#define STACK_INIT_SIZE 100 // 存储空间初始分配量
#define STACKINCREMENT 10 // 存储空间分配增量
#define ERROR_01 -1//第一种错误类型，命令行参数不正确
#define ERROR_02 -2//第二种错误类型，格式错误
#define ERROR_03 -3//第三种错误类型，出现逻辑错误
#define TRUE 1
#define FALSE 0
#define Legal_symbols 8
#define temp_size 100
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+声明作用到的类型-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
 
typedef char SElemType; // 定义OPTR栈元素类型
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef double TElemType;//定义OPND栈元素类型
typedef struct Sqack
{
    SElemType* base; // 在栈构造之前和销毁之后，base的值为NULL
    SElemType* top; // 栈顶指针
    int stacksize; // 当前已分配的存储空间，以元素为单位
}SqStack; // 顺序栈，SElemType
typedef struct Sqack1
{
    TElemType* base; // 在栈构造之前和销毁之后，base的值为NULL
    TElemType* top; // 栈顶指针
    int stacksize; // 当前已分配的存储空间，以元素为单位
}SqStack1; // 顺序栈，TElemType
 
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+栈的基本操作（包含pop,init,isempty等）-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+--*/
 
Status InitStack(SqStack* S) 
{
    S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    S->stacksize = STACK_INIT_SIZE;
    S->top = S->base;
    return OK;
}//输入一个栈名（需在主程序中定义），对其进行初始化定义线性栈，长度为STACK_INIT_SIZE（宏定义）。OPTR
Status InitStack1(SqStack1* S)
{
    S->base = (TElemType*)malloc(STACK_INIT_SIZE * sizeof(TElemType));
    S->stacksize = STACK_INIT_SIZE;
    S->top = S->base;
    return OK;
}//输入一个栈名（需在主程序中定义），对其进行初始化定义线性栈，长度为STACK_INIT_SIZE（宏定义）。OPND
Status IsEmpty(SqStack* S)
{
    if (S->top == S->base) return TRUE;
    else return FALSE;
}//判断是否为空栈。头指针等于尾指针则为空栈
Status Push_symbol (SqStack *S, SElemType e)
{
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (SElemType*)(realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType)));
        if (!S->base)return ERROR;
        S->stacksize += STACKINCREMENT;
    }
    //追加存储空间
    S->top++;
    *S->top = e;
    return OK;
}//将e压进操作符栈
Status Push_num(SqStack1* S, TElemType e)
{
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (TElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(TElemType));
        if (!S->base)return ERROR;
        S->stacksize += STACKINCREMENT;
    }
    //追加存储空间
    S->top++;
    *S->top = e;
    return OK;
}//压栈，OPND
SElemType GetTop(SqStack* S)
{
    return *S->top;
}//返回S栈的栈顶元素
SElemType GetTop1(SqStack1* S)
{
    return *S->top;
}//返回OPND的第一个元素
Status Pop_symbol (SqStack *S, SElemType  *e)
{
    if (S->base == S->top)return ERROR;
    *e = (S->top[0]);//这里注意运算符优先级
    S->top--;
    return OK;
    // 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
}
Status Pop_num(SqStack1* S, TElemType* e)
{
    if (S->base == S->top)return ERROR;
    *e = *(S->top);//这里注意运算符优先级
    S->top--;
    return OK;
    // 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
}//
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+状态判断函数（是否是操作符，主函数是否1停止，输入字符串是否合法）-+-+-+-+-+-+-+-+-++-*/
 
 
Status IsInOP (SElemType e)//判断是否是操作符
{
    switch (e) {
    case '+': return TRUE; break;
    case '-': return TRUE; break;
    case '*': return TRUE; break;
    case '/': return TRUE; break;
    case '(': return TRUE; break;
    case ')': return TRUE; break;
    case '#': return TRUE; break;
    case '^': return TRUE; break;
    default: return FALSE;
    }
}//判断该元素e是否是操作符。输入字符，返回bool状态值
Status Stop(SqStack S)
{
    if (IsEmpty(&S)) return TRUE;
    else return FALSE;
}//如果OPTR栈空了，那就停止就行了，因为#对已经被pop掉了，说明没有运算符了，整体运算已经结束了。//输入栈S，返回TRUE和FALSE状态。
Status IsIllegial(char in[], int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        if (IsInOP(in[i]) && IsInOP(in[i + 1]))
        {
            if ((IsInOP(in[i]) && in[i + 1] == '(') || (IsInOP(in[i + 1]) && in[i] == ')') )//当有括号时认为存在可能的可能行
            {
                if ((in[i] == '(' && in[i + 1] == ')'))
                {
                    return TRUE;
                }//如果是（）或者[]，后边判断不出来，单独判断。
            }
            else
            {
                return TRUE;//其他情况直接报错
            }
        }
    }
    return FALSE;
}//用于判断是否存在++，(),[]这种东西。输入字符串，返回状态值TRUE和FALSE
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+操作函数（计算器主函数，具体操作函数，成幂运算，级别返回运算等）-+-+-+-+-+-+-+-+-+-+-*/
 
double power(TElemType a, TElemType b) 
{
    double output=(double)1;
    for (int i = 0; i < b; i++) output = output * a;
    return output;
}//实现乘幂运算，a^b，其中b为整数。   输入数字a和数字b  返回a^b的值。
TElemType Change(char* temp,int length)
{
    int len = 0;//总体长度
    int i = 0;//整数位长度
    int j = 0;//小数位长度
    int* a=(int*)malloc((length+STACKINCREMENT) *sizeof(int));//用于存放整数
    int* b=(int*)malloc((length + STACKINCREMENT) *sizeof(int));//用于存放小数
    while (len < length && temp[len]!='.')
    {
        a[i] = (temp[len])-'0';
        i++;
        len++;
    }//将整数取出来
    len += 1;
    while (len < length)
    {
        b[j] = temp[len]-'0';
        len++;
        j++;
    }//将小数取出来。
    double output=(double)0;//用于输出这个数字
    int k = 0;//用于取用数字
    while (i > 0)//转整数部分
    {
        output = output +(double)(a[k] *power(10,(i - 1)));
        i--;
        k++;
    }
    k = 0;
    while (j > 0)//转小数部分
    {
        output = output +(double)( b[k] / (power(10 ,(k + 1))));
        j--;
        k++;
    }
    return output;
}//将字符串转换为double类型小数。输入字符串，返回double类型数字
Status ChangeArgv(char argv[], char* argv_usable, int Length)
{
    int i = 0;
    for (i = 0; i < Length; i++)
    {
        argv_usable[i] = argv[i];
    }
    argv_usable[i] = '#';
    argv_usable[i + 1] = '\0';
    return OK;
}//argv无法直接操作，要先转成可操作的，再进行处理。输入argv[1]返回argv_usable,之后进行操作
double Operate(TElemType a, SElemType op,TElemType b )
{
    double output;
    switch (op) {
    case '+':output = (double)(a)+(double)(b); break;
    case '-':output = (double)(a)-(double)(b); break;
    case '*':output = (double)(a)*(double)(b); break;
    case '/':output = (double)(a)/(double)(b); break;
    case '^':output = power((double)(a),(double)(b)); break;
    default:return ERROR_02;
    }
    return output;
}//定义二元之间的运算转换符    输入 数字a 操作符字符theta 数字b   返回计算结果double类型
char OPTR_Prior_Table(SElemType e1, SElemType e2)
{
    int i=0;
    int j=0;
    char pre[Legal_symbols][Legal_symbols] = {
        /*运算符之间的优先级制作成一张表格*/
            {'>','>','<','<','<','>','>','<'},
            {'>','>','<','<','<','>','>','<'},
            {'>','>','>','>','<','>','>','<'},
            {'>','>','>','>','<','>','>','<'},
            {'<','<','<','<','<','=','0','<'},
            {'>','>','>','>','0','>','>','>'},
            {'<','<','<','<','<','0','=','<'},
            {'>','>','>','>','<','>','>','>'}};//如果输入是 []  OR （）则为=
        switch (e1) {
        case '+': i = 0; break;
        case '-': i = 1; break;
        case '*': i = 2; break;
        case '/': i = 3; break;
        case '(': i = 4; break;
        case ')': i = 5; break;
        case '#': i = 6; break;
        case '^': i = 7; break;
        }
        switch (e2) {
        case '+': j = 0; break;
        case '-': j = 1; break;
        case '*': j = 2; break;
        case '/': j = 3; break;
        case '(': j = 4; break;
        case ')': j = 5; break;
        case '#': j = 6; break;
        case '^': j = 7; break;
        }
        return pre[i][j];//优先级存放在pre函数之中。
}//定义判断运算符优先级的函数。返回值为><=或0（为0时认为无法比较，要报错！）先输入e1，再输入e2.     输入e1和e2 返回两个的优先级。
Status OPERATION(int argc, char* argv[], double* result)
{
    if (IsIllegial(argv[1], strlen(argv[1]))) { result[0] = ERROR_02; return ERROR_02; }; //表达式格式错误
    if (argc != 2) {  result[0] = ERROR_01; return ERROR_01; }//命令行参数个数不正确
    int LEN = strlen(argv[1]);
    char* argv_usable = malloc((strlen(argv[1]) + 10) * sizeof(char));
    ChangeArgv(argv[1],argv_usable,LEN);
    SqStack *OPTR=(SqStack*)malloc(sizeof(SqStack));//运算符栈
    SqStack1 *OPND=(SqStack*)malloc(sizeof(SqStack));//运算数字栈
    InitStack(OPTR);
    InitStack1(OPND);
    Push_symbol(OPTR, '#');
    //定义和生成两个栈，OPTR为运算符栈，OPTD为数字栈；并将运算符栈头尾压入#，作为标识符。
 
    int i = 0;//用于判定argv的个数。
    SElemType *opr1=(SElemType*)malloc(sizeof(SElemType));//用来存放被pop掉的OPTR
    SElemType *opr2=(SElemType*) malloc(sizeof(SElemType));//用于存放被pop掉的OPTR
    TElemType *num1=(TElemType*)malloc(sizeof(TElemType));//用于存放被pop掉的OPND
    TElemType* num2 = (TElemType*)malloc(sizeof(TElemType));//用于存放被pop掉的OPND
    char* temp=(char*)malloc(temp_size*sizeof(char));//用于暂存数字，存完之后转为double并压进OPND。100为极长的一套数字。)
    if(opr1==NULL)
	{
	printf("malloc failed");
	return FALSE;
	}
	memset(opr1,0,sizeof(SElemType));
    free(opr1);
    if(opr2==NULL)
	{
	printf("malloc failed");
	return FALSE;
	}
	memset(opr2,0,sizeof(SElemType));
    free(opr2);
    if(num1==NULL)
	{
	printf("malloc failed");
	return FALSE;
	}
	memset(num1,0,sizeof(SElemType));
    free(num1);
    if(num2==NULL)
	{
	printf("malloc failed");
	return FALSE;
	}
	memset(num2,0,sizeof(SElemType));
    free(num2);
    if(temp==NULL)
	{
	printf("malloc failed");
	return FALSE;
	}
	memset(temp,0,temp_size*sizeof(char));
    free(temp);//分别判断上述五个malloc是否为空指针
    
	int flag=1;//用于判断是否应当停止循环
    int j = 0;//用于判断是第几位的
    while (flag)//flag用于判断是否停止循环，其值的更改有Stop函数判定
    {
        SElemType nowchar = argv_usable[i];
        if (IsInOP(argv_usable[i]) && j != 0)
        {
            Push_num(OPND, Change(temp, j));
            j = 0;
        }//如果判断出来下一位是字符且且此时j！=0（数组不空），那把数组转换为double类型。
        if (IsInOP(argv_usable[i]))
        {
            if (OPTR_Prior_Table(GetTop(OPTR), argv_usable[i]) == '0')
            {
                result[0] = ERROR_02;
                return ERROR_02;
            }//当返回值为0是，不符合运算正确定义的要求。
            switch (OPTR_Prior_Table(GetTop(OPTR), argv_usable[i]))//进行比较操作
            {
            case'<':Push_symbol(OPTR, nowchar); i++; break;//栈顶元素优先级低
            case'=':Pop_symbol(OPTR, opr1); i++; break;//优先级相同时
            case'>': {Pop_symbol(OPTR, opr2);//栈顶元素优先级高
                Pop_num(OPND, num2);//取数字栈中栈顶元素
                Pop_num(OPND, num1);
                if ((*opr2 == '/' && *num2 == 0))
                {
                    result[0] = ERROR_03;
                    return ERROR_03;
                }//当除以0的时候报ERROR-03
                Push_num(OPND, Operate(*num1, *opr2, *num2)); //ab经过运算后重新压入栈中
                break; }
            }
            if (Stop(*OPTR))//栈空
            {
                flag = 0;
            }//flag的判定及更改函数，决定主函数循环是否继续进行
        }
        else
        {
            if (argv_usable[i] == '0' || argv_usable[i] == '1'|| argv_usable[i] == '2'|| argv_usable[i] == '3'|| argv_usable[i] == '4'|| argv_usable[i] == '5'|| argv_usable[i] == '6'|| argv_usable[i] == '7'|| argv_usable[i] == '8'|| argv_usable[i] == '9'|| argv_usable[i] == '.')
            {
                temp[j] = argv[1][i];
                j++;
                //flag = 0;
                i++;
            }
            //如果是正常数字及小数点，则放进字符串之中，等待转换为double类型小数。
            else
            {
                result[0] = ERROR_02;
                return ERROR_02;
            }//如果不是正常的数字，输入了非法字符，直接报错。ERROR_02。
        }//当此时读进来的数字不是操作符时，判断它是否是正常的数字（包含小数点）
    }
    //运算函数
    result[0] = OK;
    result[1] = OPND->base[1];//此时OPND栈顶即为操作结果
    return OK;
}//主要操作函数，输入int aggc, char* argv[]，返回操作结果状态和操作结果数字。
 
 /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+主函数和用于测试小函数的测试函数。-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-*/
 
double main(int argc, char* argv[])
{
    double* result = (double*)malloc(2 * sizeof(double));
    switch (OPERATION(argc, argv, result))
    {
    case ERROR_01: printf("ERROR_01"); return ERROR_01;
    case ERROR_02: printf("ERROR_02"); return ERROR_02;
    case ERROR_03: printf("ERROR_03"); return ERROR_03;
    case OK: printf("%.9g", result[1]); return result[1];
    }
}//主函数，调用操作，返回结果。