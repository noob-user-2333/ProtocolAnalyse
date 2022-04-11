//
// Created by user on 2022/4/4.
//

#ifndef XML_XML_H
#define XML_XML_H


#include <string>
#include <utility>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <map>
#include <vector>
#include "XMLNode.h"
/*
 * nodes中不包含顶层节点
 *
 *
 * */
class XML {
private:
    std::vector<XMLNode> nodes;
    std::string rootNodeName;
    unsigned char buffer[4096];
    static constexpr unsigned short defaultPageSize = 1024;
public:
    void XMLAnalysis(const std::string& path);



    const std::vector<XMLNode> & NodesGet() const {return nodes;}
    const std::string & RootNodeNameGet() const {return rootNodeName;}
};


#endif //XML_XML_H
