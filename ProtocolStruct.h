//
// Created by user on 2022/4/9.
//

#ifndef XML_PROTOCOLSTRUCT_H
#define XML_PROTOCOLSTRUCT_H
#include <utility>

#include "XML.h"
#include "Calculator.h"

struct ProtocolField{
    std::string name;
    std::string length;
    std::string times;
    unsigned int input;
    unsigned int output;
    std::vector<ProtocolField> children;
    ProtocolField(std::string  name,std::string  length,std::string  times,
                  unsigned int input,unsigned int output):name(std::move(name)),length(std::move(length)),times(std::move(times)),
                  input(input),output(output){}
};

struct Protocol{
    std::string protocolName;
    std::string modeName;
    unsigned char firstByte;
    std::vector<ProtocolField> fields;
    Protocol(std::string protocolName,std::string modeName):
    protocolName(std::move(protocolName)),modeName(std::move(modeName)),firstByte(0){}
};






#endif //XML_PROTOCOLSTRUCT_H
