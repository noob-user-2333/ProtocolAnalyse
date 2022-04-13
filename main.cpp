#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <sqlite3.h>
#include "ProtocolAnalysis.h"
constexpr char defaultXML[] = "../test.xml";
const std::string defaultVar = "a";
unsigned char buffer[4096];
unsigned short count = 0;
unsigned short NameIndexFind(void *,const std::string &){
    return count++;
}
unsigned int VarValueGet(void*arg,int index){
    return 0xFF;
}
const std::string & IndexNameFind(void*,int){
    return defaultVar;
}
Calculator cal;
int main() {

    cal.NameIndexFindSet(NameIndexFind);
    cal.VarValueGetSet(VarValueGet);
    cal.ExpressCompilation(nullptr,"a * 7 + 16 / 2",buffer);
    printf("result:%u,success result:%u",cal.Compute(nullptr,buffer),0xFF * 7 + 16 /2);
    cal.CodeDisplay(IndexNameFind,nullptr,buffer);
    return 0;
}
