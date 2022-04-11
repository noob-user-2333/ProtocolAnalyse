//
// Created by user on 2022/4/2.
//

#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <list>
#include <string>
#include <memory>
#include <vector>

struct Token{
    const unsigned char typeId;
    const std::string token;
    Token(unsigned char typeId,std::string  token):typeId(typeId),token(std::move(token)){

    }
};



class Lexer {
private:

    typedef unsigned char UCHAR;
    static UCHAR classMapArray[256];
    static UCHAR typeMapArray[256];
    static bool arrayIsInit;
    static constexpr UCHAR HexBit = 128;
    static void ArrayInit();

public:
    static constexpr UCHAR IllegalId = 255;
    static constexpr UCHAR NullId = 0;
    static constexpr UCHAR NameId = 1;
    static constexpr UCHAR DigitalId = 2;
    static constexpr UCHAR SpaceId = 3;
    static constexpr UCHAR StringId = 4;
    static constexpr UCHAR PipeId = 5; // |
    static constexpr UCHAR BangId = 6; // !
    static constexpr UCHAR MinusId = 7; // -
    static constexpr UCHAR LessId = 8; // <
    static constexpr UCHAR MoreId = 9; // >
    static constexpr UCHAR EqualId = 10; // =
    static constexpr UCHAR SlashId = 11; // /
    static constexpr UCHAR SemiId = 12; // ;
    static constexpr UCHAR PlusId = 13; // +
    static constexpr UCHAR StarId = 14; // *
    static constexpr UCHAR PercentId = 15; // %
    static constexpr UCHAR CommaId = 16; // ,
    static constexpr UCHAR AndId = 17; // &
    static constexpr UCHAR TildaId = 18; // ~
    static constexpr UCHAR DotId = 19; // .
    static constexpr UCHAR SmallLeftBracket = 20; // (
    static constexpr UCHAR SmallRightBracket = 21; // )
    static constexpr UCHAR MiddleLeftBracket = 22; // [
    static constexpr UCHAR MiddleRightBracket = 23; // ]
    static constexpr UCHAR BigLeftBracket = 24; // {
    static constexpr UCHAR BigRightBracket = 25; // }
    static constexpr UCHAR DollarId = 26; // $
    static constexpr UCHAR XorId = 27;    // ^


    explicit Lexer() {
        if (!arrayIsInit)
            ArrayInit();
    }
    void LexerAnalysis(const std::string& source,std::vector<Token> & tokens);
    void LexerAnalysis(const char* source,size_t length,std::vector<Token> & tokens);




};


#endif //LEXER_LEXER_H
