cmake_minimum_required(VERSION 3.21)
project(XML)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")


add_executable(XML main.cpp XML.cpp XML.h XMLNode.h Calculator.cpp Calculator.h Lexer.h Lexer.cpp ProtocolAnalysis.cpp ProtocolAnalysis.h ProtocolStruct.h ProtocolStruct.cpp)
target_link_libraries(XML xml2 sqlite3)