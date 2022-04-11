//
// Created by user on 2022/4/2.
//

#include "Lexer.h"
#include <stdexcept>

bool Lexer::arrayIsInit = false;
unsigned char Lexer::typeMapArray[256] = {0};
unsigned char Lexer::classMapArray[256] = {0};
void Lexer::ArrayInit() {
    classMapArray[0] = NullId;
    for (int index = '0'; index <= '9'; index++)
        classMapArray[index] = DigitalId;
    for (int index = 'a'; index <= 'z'; index++)
        classMapArray[index] = NameId;
    for (int index = 'A'; index <= 'Z'; index++)
        classMapArray[index] = NameId;
    classMapArray['_'] = NameId;
    classMapArray[' '] = SpaceId;
    classMapArray['\t'] = SpaceId;
    classMapArray['\n'] = SpaceId;
    classMapArray['"'] = StringId;
    classMapArray['\''] = StringId;


    classMapArray['|'] = PipeId;
    classMapArray['+'] = PlusId;
    classMapArray['%'] = PercentId;
    classMapArray['!'] = BangId;
    classMapArray['-'] = MinusId;
    classMapArray['<'] = LessId;
    classMapArray['>'] = MoreId;

    classMapArray['='] = EqualId;
    classMapArray['/'] = SlashId;
    classMapArray[';'] = SemiId;
    classMapArray['*'] = StarId;
    classMapArray[','] = CommaId;
    classMapArray['&'] = AndId;
    classMapArray['~'] = TildaId;
    classMapArray['.'] = DotId;

    classMapArray['('] = SmallLeftBracket;
    classMapArray[')'] = SmallRightBracket;
    classMapArray['['] = MiddleLeftBracket;
    classMapArray[']'] = MiddleRightBracket;
    classMapArray['{'] = BigLeftBracket;
    classMapArray['}'] = BigRightBracket;
    classMapArray['$'] = DollarId;
    classMapArray['^'] = XorId;
    for (int index = '0'; index <= '9'; index++)
        typeMapArray[index] |= HexBit;
    for (int index = 'a'; index <= 'f'; index++)
        typeMapArray[index] |= HexBit;
    for (int index = 'A'; index <= 'F'; index++)
        typeMapArray[index] |= HexBit;
    arrayIsInit = true;
}

void Lexer::LexerAnalysis(const char* source,size_t length,std::vector<Token> &tokens) {
    int offset = 0;
    while (offset < length) {
        int size = 1;
        UCHAR classId = classMapArray[source[offset]];
        switch (classId) {
            case NullId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != NullId)
                        break;
                break;
            case SpaceId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != SpaceId)
                        break;
                break;
            case NameId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != NameId &&
                        classMapArray[source[offset + size]] != DigitalId)
                        break;
                break;
            case StringId: {
                char startSign = source[offset];
                for (size = 1; size + offset < length; size++)
                    if (source[offset + size] == startSign)
                        break;
                if ((size + offset == length && source[length - 1] != startSign) || size == 1)
                    classId = IllegalId;
                break;
            }
            case DigitalId: {
                if (offset + size == length)
                    break;
                bool isHex = source[offset + size] == 'x' || source[offset + size] == 'X';
                size++;
                for (; size + offset < length; size++)
                    if (!(classMapArray[source[offset + size]] == DigitalId ||
                            (isHex && (typeMapArray[source[offset + size]] | HexBit))) )
                        break;
                if (size + offset < length && classMapArray[source[offset + size]] == DotId) {
                    size++;
                    int origin = size;
                    for (; size + offset < length; size++)
                        if (!(classMapArray[source[offset + size]] == DigitalId ||
                                (isHex && (typeMapArray[source[offset + size]] | HexBit) > 0) ))
                            break;
                    if (origin == size)
                        classId = IllegalId;
                }
                break;
            }
            default:
                size = 1;
        }

        std::string subString(source,offset, size);
        tokens.emplace_back(classId, subString);
        offset += size;
    }
}

void Lexer::LexerAnalysis(const std::string &source, std::vector<Token> &tokens) {
    int length = source.size();
    int offset = 0;
    while (offset < length) {
        int size = 1;
        UCHAR classId = classMapArray[source[offset]];
        switch (classId) {
            case NullId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != NullId)
                        break;
                break;
            case SpaceId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != SpaceId)
                        break;
                break;
            case NameId:
                for (size = 1; size + offset < length; size++)
                    if (classMapArray[source[offset + size]] != NameId &&
                        classMapArray[source[offset + size]] != DigitalId)
                        break;
                break;
            case StringId: {
                char startSign = source[offset];
                for (size = 1; size + offset < length; size++)
                    if (source[offset + size] == startSign)
                        break;
                if ((size + offset == length && source[length - 1] != startSign) || size == 1)
                    classId = IllegalId;
                break;
            }
            case DigitalId: {
                if (offset + size == length)
                    break;
                bool isHex = source[offset + size] == 'x' || source[offset + size] == 'X';
                size++;
                for (; size + offset < length; size++)
                    if (!(classMapArray[source[offset + size]] == DigitalId ||
                            (isHex && (typeMapArray[source[offset + size]] | HexBit)) ))
                        break;
                if (size + offset < length && classMapArray[source[offset + size]] == DotId) {
                    size++;
                    int origin = size;
                    for (; size + offset < length; size++)
                        if (!(classMapArray[source[offset + size]] == DigitalId ||
                                (isHex && (typeMapArray[source[offset + size]] | HexBit) > 0) ))
                            break;
                    if (origin == size)
                        classId = IllegalId;
                }
                break;
            }
            default:
                size = 1;
        }

        std::string subString = source.substr(offset, size);
        tokens.emplace_back(classId, subString);
        offset += size;
    }
}