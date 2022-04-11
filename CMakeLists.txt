cmake_minimum_required(VERSION 3.21)
project(XML)

set(CMAKE_CXX_STANDARD 14)

add_executable(XML main.cpp XML.cpp XML.h XMLNode.h Calculator.cpp Calculator.h Lexer.h Lexer.cpp ProtocolAnalysis.cpp ProtocolAnalysis.h ProtocolStruct.h)
target_link_libraries(XML xml2 sqlite3)
