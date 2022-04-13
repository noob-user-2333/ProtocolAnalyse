//
// Created by user on 2022/4/12.
//

#include <iostream>
#include "ProtocolStruct.h"


std::string ProtocolNode::attrsSet[] = {"length", "bit", "input", "output", "const", "repeat", "crc"};

unsigned char ProtocolNode::AttrIdGet(const std::string &attr) {
    static std::map<std::string, unsigned char> attrsMap({
                                                                 std::make_pair("length", 0), std::make_pair("bit", 1),
                                                                 std::make_pair("input", 2),
                                                                 std::make_pair("output", 3),
                                                                 std::make_pair("const", 4),
                                                                 std::make_pair("repeat", 5),
                                                                 std::make_pair("crc", 6)});
    return attrsMap[attr];
}

ProtocolNode::ProtocolNode(ProtocolNode *parentNode, std::string name) : parentNode(parentNode), name(std::move(name)) {
}

void ProtocolNode::AttrAdd(const std::string &attr) {
//    attrs.emplace_back(attr);
}

void ProtocolNode::AttrAdd(const std::string &attrName, const std::string &attrValue) {
    static unsigned char buffer[512] = {0};
    int byteCount = 1;
    buffer[0] = AttrIdGet(attrName);
    byteCount += Calculator::ExpressCompilation(NameIndexFind, parentNode, attrValue, buffer + 1);
    attrs.emplace_back(buffer, buffer + byteCount);
}

unsigned short ProtocolNode::NameIndexFind(void *arg, const std::string &name) {
    auto parent = static_cast<ProtocolNode *>(arg);
    unsigned char index = 0;
    for (auto &child: parent->children) {
        if (name == child.name)
            return index;
        index++;
    }
    throw std::invalid_argument(name);
}

void ProtocolNode::ChildAdd(const std::string &childName) {
//    children.emplace_back(this, childName);
}

const std::string &ProtocolNode::IndexNameFind(void *arg, int index) {
    auto parent = static_cast<ProtocolNode *>(arg);
    if (index >= parent->children.size())
        throw std::invalid_argument("index");
    return parent->children[index].name;
}

void ProtocolNode::AttrDisplay(int index) {
    if (index >= attrs.size())
        throw std::invalid_argument("index");
    auto pCode = attrs[index].c_str();
    std::cout << "attr:" << attrsSet[*pCode] << "  value:";
    Calculator::CodeDisplay(IndexNameFind, parentNode, pCode + 1);
}
