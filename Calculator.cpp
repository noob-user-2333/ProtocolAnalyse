//
// Created by user on 2022/4/6.
//

#include <iostream>
#include "Calculator.h"

unsigned char Calculator::priorityMap[256];
bool Calculator::mapIsInit = false;


void Calculator::PriorityMapInit() {
    for (unsigned char &i: priorityMap)
        i = 255;
    priorityMap[Lexer::DigitalId] = 0;
    priorityMap[Lexer::NameId] = 0;
    priorityMap[Lexer::SmallLeftBracket] = 1;
    priorityMap[Lexer::StarId] = 2;
    priorityMap[Lexer::SlashId] = 2;
    priorityMap[Lexer::PercentId] = 2;
    priorityMap[Lexer::PlusId] = 3;
    priorityMap[Lexer::MinusId] = 3;
    priorityMap[Lexer::AndId] = 4;
    priorityMap[Lexer::XorId] = 5;
    priorityMap[Lexer::PipeId] = 6;
    priorityMap[Lexer::SmallRightBracket] = 7;
}
//unsigned char Calculator::NameIndexFind(const std::vector<std::string> &nameTable, const std::string &name) {
//    size_t size = nameTable.size();
//    for(int index = 0;index < size; ++index)
//        if(nameTable[index] == name)
//            return index;
//    throw std::invalid_argument("can not find name in nameTable");
//}

void Calculator::InfixToPostFix(std::vector<Token> &tokens, std::vector<Token *> &postfix,
                                std::stack<Token *> &operationStack) {
    while (!operationStack.empty())
        operationStack.pop();
    int size = tokens.size();
    for (auto & token: tokens) {
        if (token.typeId != Lexer::SpaceId) {
            int currentPriority = priorityMap[token.typeId];
            switch (currentPriority) {
                case 0: {
                    postfix.push_back(&token);
                    break;
                }
                case 255:
                    throw std::invalid_argument("error tokens!!!");
                default:
                    if (operationStack.empty() ||
                        currentPriority < priorityMap[operationStack.top()->typeId])
                        operationStack.push(&token);
                    else if (token.typeId == Lexer::SmallRightBracket) {
                        while (!operationStack.empty() && operationStack.top()->typeId != Lexer::SmallLeftBracket) {
                            postfix.push_back(operationStack.top());
                            operationStack.pop();
                        }
                        operationStack.pop();
                    } else {
                        while (!operationStack.empty() && operationStack.top()->typeId != Lexer::SmallLeftBracket &&
                        currentPriority >= priorityMap[operationStack.top()->typeId]) {
                            postfix.push_back(operationStack.top());
                            operationStack.pop();
                        }
                        operationStack.push(&token);
                    }
            }
        }
    }

    while (!operationStack.empty()) {
        postfix.push_back(operationStack.top());
        operationStack.pop();
    }

}

int
Calculator::ExpressCompilation(unsigned char (*NameIndexFind)(void*,const std::string &),void*arg,const std::string &expression,
                               void *buffer){
    int byteCount = 0;
    auto * buff = static_cast < unsigned char * >(buffer);
    tokens.clear();
    postfix.clear();
    std::cout << expression << std::endl;
    lexer.LexerAnalysis(expression, tokens);
    InfixToPostFix(tokens, postfix, operationStack);
    for(const auto token : postfix){
        std::cout << token->token << "  " ;
        int size = TokenToCode(NameIndexFind,arg,*token,buff);
        byteCount += size;
        buff = buff + size;
    }
    std::cout << std::endl;
    *buff = End;
    byteCount += 1;
    return byteCount;
}

int Calculator::TokenToCode(unsigned char (*NameIndexFind)(void*,const std::string &),void*arg,const Token &token, void *buffer){
    auto *buff = static_cast<unsigned char *>(buffer);
    unsigned char & codeType = *buff;
    buff++;
    unsigned int & num = *(reinterpret_cast<unsigned int*>(buff));
    switch (token.typeId){
        case Lexer::NameId:{
            codeType = Var;
            *buff = NameIndexFind(arg,token.token);
            return 2;
        }
        case Lexer::DigitalId:{
            codeType = ConstNum;
            bool isHex = token.token.size() > 2 && (token.token[1] == 'x' || token.token[1] == 'X');
            num = std::stoi(token.token,nullptr,isHex ? 16:10);
            return 5;
        }
        default:
            codeType = token.token[0];
            return 1;
    }
    throw std::runtime_error("unknown error");
}

unsigned int Calculator::Compute(unsigned int (*varValueGet)(void*,int),void*arg,const void *pCode) {
    if(pCode == nullptr)
        return -1;
    auto code_ptr = static_cast<const unsigned char*>(pCode);
    while(*code_ptr){
        switch (*code_ptr) {
            case ConstNum:{
                auto data_ptr = reinterpret_cast<const unsigned int*>(code_ptr + 1);
                Push(*data_ptr);
                code_ptr += 5;
                break;
            }
            case Var: {
                Push(varValueGet(arg,*(code_ptr + 1)));
                code_ptr += 2;
                break;
            }
            case '+':{
                unsigned int data = Pop();
                Push(Pop() + data);
                code_ptr += 1;
                break;
            }
            case '-':{
                unsigned int data = Pop();
                Push(Pop() - data);
                code_ptr += 1;
                break;
            }
            case '*':{
                unsigned int data = Pop();
                Push(Pop() * data);
                code_ptr += 1;
                break;
            }
            case '/':{
                unsigned int data = Pop();
                Push(Pop() / data);
                code_ptr += 1;
                break;
            }
            case '&':{
                unsigned int data = Pop();
                Push(Pop()  & data);
                code_ptr += 1;
                break;
            }
            case '|':{
                unsigned int data = Pop();
                Push(Pop() | data);
                code_ptr += 1;
                break;
            }
            case '^':{
                unsigned int data = Pop();
                Push( Pop() ^ data);
                code_ptr += 1;
                break;
            }
            case '%':{
                unsigned int data = Pop();
                Push(Pop() % data);
                code_ptr += 1;
                break;
            }
            default:
                throw std::runtime_error("unknown error in code");
        }
    }
    return Pop();
}





