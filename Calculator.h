//
// Created by user on 2022/4/6.
//

#ifndef CACULATOR_CALCULATOR_H
#define CACULATOR_CALCULATOR_H

#include <stack>
#include "Lexer.h"

/*  Calculator指令格式:
 *      名称              偏移量     长度        介绍
 *      操作码(opCode)     0        1          说明立即数的作用
 *      立即数(Imm)        1        0~4        根据操作码的不同有不同的含义
 * */

/*  Calculator操作数解释:
 *          操作码     立即数长度    含义
 *          0         0           不含立即数，表示该段代码执行完毕
 *          1         4           该立即数对应运算时使用的常量 如: 0x8440
 *          2         1           该立即数对应使用第几个变量进行运算
 *
 *  注:对于操作符其对应操作码为其ASCII值，且立即数长度为0
 * */

class Calculator {
private:
    static unsigned char priorityMap[];
    static bool mapIsInit;
    static void PriorityMapInit();
    Lexer lexer;
    std::vector<Token> tokens;
    std::vector<Token*> postfix;
    std::stack<Token*> operationStack;

    unsigned int stack[32]{0};
    unsigned int stackTop;
    void Push(unsigned int data){
        stack[stackTop] = data;
        stackTop++;
    }
    unsigned int Pop(){
        stackTop--;
        return stack[stackTop];
    }
    static void InfixToPostFix(std::vector<Token> & tokens,std::vector<Token*> &postfix,
                               std::stack<Token*>& operationStack);
//    static unsigned char  NameIndexFind(const std::vector<std::string> & nameTable,const std::string & name);
    //将token转化为指令
    //返回生成指令的字节数
    static int  TokenToCode(unsigned char  (*NameIndexFind)(void * arg,const std::string & name),void * arg,
                            const Token & token,void *buffer);
public:

    static constexpr unsigned char End = 0;
    static constexpr unsigned char ConstNum = 1;
    static constexpr unsigned char Var = 2;
    Calculator():stackTop(0){
        if(!mapIsInit)
            PriorityMapInit();
        mapIsInit = true;
    }



    
    /*
     * function : 将expression编译为一串可被Calculator执行的代码
     * nameTable: 包含有expression中变量名的姓名表
     * expression:待被编译的表达式 example: 32 + data * 5
     * buff:      用于存放编译结果
     * return:    生成了多少字节的指令
    */
    int ExpressCompilation(unsigned char  (*NameIndexFind)(void *arg,const std::string & name),void *arg,
                           const std::string & expression,void *buffer);

    /*
     * function:运行pCode指向的一段代码并返回计算结果
     * varArray:用于存储变量数据的数组
     * pCode:   指向一段代码的指针
     * return:  计算结果
     * */
    unsigned int Compute(unsigned int (*VarValueGet)(void*arg,int index),void*arg,const void* pCode);
};


#endif //CACULATOR_CALCULATOR_H
