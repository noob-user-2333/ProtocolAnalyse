//
// Created by user on 2022/4/4.
//

#ifndef XML_XMLNODE_H
#define XML_XMLNODE_H
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string>
#include <utility>
#include <memory>
#include <map>
#include <vector>



class XMLNode {
public:
    std::string name;
    std::map<std::string,std::string> attrs;
    std::vector<XMLNode> children;

    explicit XMLNode(xmlNodePtr  pNode): name(reinterpret_cast<const char*>(pNode->name)){
        xmlAttrPtr  pAttr = pNode->properties;
        while(pAttr){
            xmlChar * pData = xmlGetProp(pNode,pAttr->name);
            attrs.insert(std::make_pair(reinterpret_cast<const char*>(pAttr->name),reinterpret_cast<const char*>(pData)));
            pAttr = pAttr->next;
            xmlFree(pData);
        }

        pNode = pNode->children;
        while(pNode){
            if(pNode->type == XML_ELEMENT_NODE)
                children.emplace_back(pNode);
            pNode=pNode->next;
        }

    }
};
#endif //XML_XMLNODE_H
