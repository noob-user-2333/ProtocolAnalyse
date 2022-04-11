//
// Created by user on 2022/4/4.
//

#include "XML.h"
#include <stdexcept>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>


void XML::XMLAnalysis(const std::string& path) {
    xmlDocPtr pDoc = xmlReadFile(path.c_str(),nullptr,0);
    if(pDoc == nullptr)
        throw std::invalid_argument(path);
    xmlNodePtr pRootNode = xmlDocGetRootElement(pDoc);
    rootNodeName = reinterpret_cast<const char*>(pRootNode->name);
    xmlNodePtr pNode = pRootNode->children;
    while(pNode){
        if(pNode->type == XML_ELEMENT_NODE)
            nodes.emplace_back(pNode);
        pNode = pNode -> next;
    }
    xmlFreeDoc(pDoc);
}


