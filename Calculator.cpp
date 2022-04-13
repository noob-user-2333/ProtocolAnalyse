//
// Created by user on 2022/4/6.
//

#include <iostream>
#include "Calculator.h"

class PriorityMap {
private:
    unsigned char priorityMap[256]{};
public:
    unsigned char inline PriorityGet(int index) { return priorityMap[index]; }

    PriorityMap() {
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

    unsigned char operator[](int index) { return priorityMap[index]; }
};


/*  Calculator指令格式:
 *      名称              偏移量     长度        介绍
 *      操作码(opCode)     0        2          说明立即数的作用
 *      立即数(Imm)        2        2          根据操作码的不同有不同的含义
 *      数值(data)         4        4          用于部分需要使用32bit数据的情况
 * */

/*  Calculator操作数解释:
 *          操作码        含义
 *          0            不含立即数，表示该段代码执行完毕
 *          1            该立即数表示变量位置
 *          2            该立即数表示长度为16bit的常数
 *          3            该立即数为对应运算符的ASCII码
 *          4            表示使用32bit常数运算，立即数固定为0xFFFF，后接一个32bit的常数
 * */
struct CalculatorInstruct {
    union {
        struct {
            unsigned short opCode;
            unsigned short Imm;
        };
        unsigned int data;
    };
};


void Calculator::InfixToPostFix(std::vector<Token> &tokens, std::vector<Token *> &postfix) {
    static PriorityMap priorityMap;
    static std::stack<Token *> operationStack;
    while (!operationStack.empty())
        operationStack.pop();
    size_t size = tokens.size();
    for (auto &token: tokens) {
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
Calculator::ExpressCompilation(void *arg, const std::string &expression, void *buffer) {
    return ExpressCompilation(NameIndexFind, arg, expression, buffer);
}

int Calculator::TokenToCode(unsigned short (*NameIndexFind)(void *, const std::string &), void *arg, const Token &token,
                            void *buffer) {
    auto pInstruct = static_cast<CalculatorInstruct *>(buffer);
    switch (token.typeId) {
        case Lexer::NameId: {
            pInstruct->opCode = VarIndex;
            pInstruct->Imm = NameIndexFind(arg, token.token);
            return sizeof(CalculatorInstruct);
        }
        case Lexer::DigitalId: {
            pInstruct->opCode = ConstNum;
            bool isHex = token.token.size() > 2 && (token.token[1] == 'x' || token.token[1] == 'X');
            unsigned int num = std::stoi(token.token, nullptr, isHex ? 16 : 10);
            if (num & 0xFFFF0000) { //如果num长度大于16bit，还需使用下一个32bit空间
                pInstruct->opCode = LongConstNum;
                pInstruct->Imm = 0xFFFF;
                pInstruct++;
                pInstruct->data = num;
                return sizeof(CalculatorInstruct) * 2;
            }
            pInstruct->Imm = num;
            return sizeof(CalculatorInstruct);
        }
        default:
            pInstruct->opCode = Operator;
            pInstruct->Imm = static_cast<unsigned short>(token.token[0]);
            return sizeof(CalculatorInstruct);
    }
    throw std::runtime_error("unknown error");
}

void Calculator::NameIndexFindSet(unsigned short (*func)(void *, const std::string &)) {
    NameIndexFind = func;
}

void Calculator::VarValueGetSet(unsigned int (*func)(void *, int)) {
    VarValueGet = func;
}

unsigned int Calculator::Compute(void *arg, const void *pCode) {
    if (pCode == nullptr)
        return -1;
    auto code_ptr = static_cast<const CalculatorInstruct *>(pCode);
    unsigned int result = 0;
    switch (code_ptr->opCode) {
        case End:
            return 0;
        case ConstNum: {
            result = code_ptr->Imm;
            break;
        }
        case LongConstNum: {
            code_ptr++;
            result = code_ptr->data;
            break;
        }
        case VarIndex: {
            result = VarValueGet(arg, code_ptr->Imm);
            break;
        }
        default :
            throw std::invalid_argument("pCode指向的第一条指令必定为数或结束符");
    }
    code_ptr++;
    while (code_ptr->opCode) {
        switch (code_ptr->opCode) {
            case ConstNum: {
                Push(result);
                result = code_ptr->Imm;
                break;
            }
            case LongConstNum: {

                code_ptr++;
                Push(result);
                result = code_ptr->data;
                break;
            }
            case VarIndex: {
                Push(result);
                result = VarValueGet(arg, code_ptr->Imm);
                break;
            }
            case Operator: {
                switch (code_ptr->Imm) {
                    case '+': {
                        result = Pop() + result;
                        break;
                    }
                    case '-': {
                        result = Pop() - result;
                        break;
                    }
                    case '*': {
                        result = Pop() * result;
                        break;
                    }
                    case '/': {
                        result = Pop() / result;
                        break;
                    }
                    case '&': {
                        result = Pop() & result;
                        break;
                    }
                    case '|': {
                        result = Pop() | result;
                        break;
                    }
                    case '^': {
                        result = Pop() ^ result;
                        break;
                    }
                    case '%': {
                        result = Pop() % result;
                        break;
                    }
                    default:
                        throw std::invalid_argument("unknown operator");
                }
                break;
            }
            default:
                throw std::runtime_error("unknown error in code");
        }
        code_ptr++;
    }
    return result;
}

int Calculator::ExpressCompilation(unsigned short (*NameIndexFind)(void *, const std::string &), void *arg,
                                   const std::string &expression, void *buffer) {
    static std::vector<Token *> postfix;
    static std::vector<Token> tokens;
    static Lexer lexer;
    int byteCount = 0;
    auto buff = static_cast < unsigned char * >(buffer);
    tokens.clear();
    postfix.clear();
    std::cout << expression << std::endl;
    lexer.LexerAnalysis(expression, tokens);
    InfixToPostFix(tokens, postfix);
    for (const auto token: postfix) {
        std::cout << token->token << "  ";
        int size = TokenToCode(NameIndexFind, arg, *token, buff);
        byteCount += size;
        buff = buff + size;
    }
    std::cout << std::endl;
    auto instruct = reinterpret_cast<CalculatorInstruct *>(buff);
    instruct->opCode = End;
    instruct->Imm = 0;
    byteCount += sizeof(CalculatorInstruct);
    return byteCount;
}

void Calculator::CodeDisplay(const std::string &(*IndexNameFind)(void *, int), void *arg, const void *buffer) {
    auto pCode = static_cast<const CalculatorInstruct *>(buffer);
    while (pCode->opCode) {
        switch (pCode->opCode) {
            case ConstNum: {
                std::cout << pCode->Imm << " ";
                break;
            }
            case LongConstNum: {
                pCode++;
                std::cout << pCode->data << " ";
            }
            case VarIndex: {
                std::cout << IndexNameFind(arg, pCode->Imm) << "  ";
                break;
            }
            case Operator: {
                std::cout << static_cast<char>(pCode->Imm) << "  ";
                break;
            }
            default:
                throw std::runtime_error("unknown optCode");
        }
        pCode++;
    }
    std::cout << std::endl;
}





