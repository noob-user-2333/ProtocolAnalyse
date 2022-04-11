//
// Created by user on 2022/4/9.
//

#include "ProtocolAnalysis.h"
#include <unordered_map>
#include <iostream>

Calculator ProtocolAnalysis::cal;
const char ProtocolAnalysis::defaultRootNodeName[] = "protocol";
static std::unordered_map < std::string,unsigned int> keywordMap({std::make_pair("length",1),std::make_pair("times",2),
                                                                  std:: make_pair("const",3),std::make_pair("input",4),
                                                                  std::make_pair("output",5)
});
static unsigned char buffer[512]{0};

ProtocolAnalysis::ProtocolAnalysis(const XML &xml) {
    if(xml.RootNodeNameGet() != defaultRootNodeName)
        throw std::invalid_argument("error root name");
    for(int i = 0;i < 32;i++)
        fieldValue[i] = i * 10;
    auto & protocolNodes = xml.NodesGet();
    for(auto & protocol : protocolNodes){
        const std::string & protocolName = protocol.name;
        for(auto & mode : protocol.children){
            protocols.emplace_back(protocolName,mode.name);
            auto & protocolMode = protocols.back();

            for(auto & field : mode.children) {
                XMLFieldAdd(protocolMode, field);

            }

        }

    }

}

void ProtocolAnalysis::XMLFieldAdd(Protocol &protocol, const XMLNode &node) {
    std::string length,times;
    unsigned int input = 0,output = 0;
    for(auto & attr : node.attrs){
        switch(keywordMap[attr.first]){
            case 1:{
                size_t size = cal.ExpressCompilation(NameIndexFind,&protocol,attr.second,buffer);
                length = std::string(buffer,buffer + size);
                break;
            }
            case 2:{
                size_t size = cal.ExpressCompilation(NameIndexFind,&protocol,attr.second,buffer);
                times = std::string(buffer,buffer + size);
                break;
            }
            case 3:{
                bool isHex = attr.second.size() > 2 && (attr.second[1] == 'x' || attr.second[1] == 'X');
                input = strtol(attr.second.c_str(),nullptr,isHex ? 16 : 10 );
                output = input;
                break;
            }
            case 4:{
                bool isHex = attr.second.size() > 2 && (attr.second[1] == 'x' || attr.second[1] == 'X');
                input = strtol(attr.second.c_str(),nullptr,isHex ? 16 : 10);
                break;
            }
            case 5: {
                bool isHex = attr.second.size() > 2 && (attr.second[1] == 'x' || attr.second[1] == 'X');
                output = strtol(attr.second.c_str(), nullptr, isHex ? 16 : 10);
                break;
            }
            default:
                throw std::invalid_argument(attr.first);
        }
    }
    protocol.fields.emplace_back(node.name,length,times,input,output);

}

void ProtocolAnalysis::ProtocolDisplay(int index) {
    auto & protocol = protocols[index];
    std::cout << "protocol:" << protocol.protocolName << std::endl;
    std::cout << "mode:" << protocol.modeName << std::endl;
    for(auto & data : protocol.fields) {
        std::cout << "field:" << data.name;
        if(!data.length.empty())
            std::cout << "  length:" << cal.Compute(VarValueGet, this, data.length.c_str());
        if(!data.times.empty())
            std::cout << " times:" << cal.Compute(VarValueGet, this, data.times.c_str());
        std::cout << " input:" << data.input << "  output:" << data.output << std::endl;
    }
    std:: cout << std::endl;
}

unsigned char ProtocolAnalysis::NameIndexFind(void * pProtocol, const std::string &name) {
    int index = 0;
    auto protocol = static_cast<Protocol*>(pProtocol);
    for(auto & fields : protocol->fields){
        if(fields.name == name)
            return index;
        index++;
    }
    throw std::invalid_argument("can not find name in protocol");
}

unsigned int ProtocolAnalysis::VarValueGet(void *arg, int index) {
    auto pProtocolAnalysis = static_cast < ProtocolAnalysis * >(arg);
    return pProtocolAnalysis->fieldValue[index];
}
