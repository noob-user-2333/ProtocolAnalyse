//
// Created by user on 2022/4/6.
//

#ifndef CACULATOR_CALCULATOR_H
#define CACULATOR_CALCULATOR_H

#include <stack>
#include "Lexer.h"


class Calculator {
private:

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
    static void InfixToPostFix(std::vector<Token> & tokens,std::vector<Token*> &postfix);
    static constexpr unsigned short End = 0;
    static constexpr unsigned short VarIndex = 1;
    static constexpr unsigned short ConstNum = 2;
    static constexpr unsigned short Operator = 3;
    static constexpr unsigned short LongConstNum = 4;
//    static unsigned char  NameIndexFind(const std::vector<std::string> & nameTable,const std::string & name);
    //将token转化为指令
    //返回生成指令的字节数
    static int  TokenToCode(unsigned short  (*NameIndexFind)(void * arg,const std::string & name),void * arg,
                            const Token & token,void *buffer);

    unsigned short (*NameIndexFind)(void *arg,const std::string & name) = nullptr;
    unsigned int  (*VarValueGet)(void*arg,int index) = nullptr;

public:


    Calculator():stackTop(0){
    }

    void NameIndexFindSet(unsigned short (*)(void *,const std::string&));
    void VarValueGetSet(unsigned int (*)(void*,int));
    /*
     * function:将编译成的二进制代码还原为字符串并显示
     * IndexNameFind: 用于根据索引找回变量名
     * arg: 传入IndexNameFind的参数
     * buffer: 指向存放代码的内存
     * */
    static void CodeDisplay(const std::string&(*IndexNameFind)(void*,int),void*arg,const void *buffer );

    /*
     *  function:提供可被直接调用的静态函数用以仅需编译表达式的情况
     *  NameIndexFind:用于查找变量位置的函数
     *  arg: 在执行NameIndexFind时传入的参数
     *  expression: 待编译的表达式
     *  buffer: 存放生成的指令
     *  return:共生成多少字节指令
     * */
    static int ExpressCompilation(unsigned short (*NameIndexFind)(void*,const std::string &),void *arg,const std::string & expression,void *buffer);
    /*
     * function : 将expression编译为一串可被Calculator执行的代码
     * expression:待被编译的表达式 example: 32 + data * 5
     * buff:      用于存放编译结果
     * return:    生成了多少字节的指令
    */
    int ExpressCompilation(void *arg,const std::string & expression,void *buffer);

    /*
     * function:运行pCode指向的一段代码并返回计算结果
     * varArray:用于存储变量数据的数组
     * pCode:   指向一段代码的指针
     * return:  计算结果
     * */
    unsigned int Compute(void*arg,const void* pCode);
};


#endif //CACULATOR_CALCULATOR_H
