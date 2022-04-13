//
// Created by user on 2022/4/9.
//

#ifndef XML_PROTOCOLSTRUCT_H
#define XML_PROTOCOLSTRUCT_H

#include <utility>

#include "XML.h"
#include "Calculator.h"


class ProtocolNode final {
private:
    static std::string attrsSet[];

    static unsigned short NameIndexFind(void *arg, const std::string &name);

    static const std::string &IndexNameFind(void *arg, int index);

public:
    static constexpr unsigned char LengthId = 0;
    static constexpr unsigned char BitId = 1;
    static constexpr unsigned char InputId = 2;
    static constexpr unsigned char OutputId = 3;
    static constexpr unsigned char ConstId = 4;
    static constexpr unsigned char RepeatId = 5;
    static constexpr unsigned char CrcId = 6;

    ProtocolNode *parentNode;
    std::string name;
    std::vector<std::string> attrs;
    std::vector<ProtocolNode> children;

    explicit ProtocolNode(ProtocolNode *parentNode, std::string name);

    static unsigned char AttrIdGet(const std::string &attr);

    /*
     * 该函数主要用于通过SQLite中编译的特征二进制码添加特征
     * */
    void AttrAdd(const std::string &attr);

    /*
     * 该函数用于通过XMLNode添加特征
     * 在添加过程中原有的字符串会编译为二进制码
     * */
    void AttrAdd(const std::string &attrName, const std::string &attrValue);

    /*
     *  该函数用于展示字符串形式的Attr
     * */
    void AttrDisplay(int index);

    /*
     * 该函数用于添加子节点
     */
    void ChildAdd(const std::string& childName);

    ProtocolNode(ProtocolNode &) = delete;

    ProtocolNode operator=(ProtocolNode ) = delete;
};


#endif //XML_PROTOCOLSTRUCT_H
