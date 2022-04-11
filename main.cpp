#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <sqlite3.h>
#include "ProtocolAnalysis.h"
constexpr char defaultXML[] = "../test.xml";

unsigned char buffer[4096];
int main() {
    XML xml;
    xml.XMLAnalysis(defaultXML);
    ProtocolAnalysis analysis(xml);
    for(int index = 0;index < analysis.ProtocolCountGet();index++)
        analysis.ProtocolDisplay(index);




    return 0;
}
