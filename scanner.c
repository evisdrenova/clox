#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct
{
    const char *start;
    const char *current;
    int line;

} Scanner;

Scanner scanner;

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAtEnd()
{
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char *message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)(strlen(message));
    token.line = scanner.line;
    return token;
}

// move forward to the next token
static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

// look ahead for 2 character tokens like != to see if it's what we expect
static bool match(char expected)
{
    if (isAtEnd())
        return false;
    if (*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}
// returns the current value without advancing the pointer
static char peek()
{
    return *scanner.current;
}
// returns the next value without advancing the pointer
static char peekNext()
{
    if (isAtEnd())
        return '\0';
    return scanner.current[1];
}

// advances the scanner past any whitespace so we know the next token is not a whitespace
static void skipWhitespace()
{
    for (;;)
    {
        char c = peek();
        switch (c)
        {
            // check if it's a space, carriage return, tab or new line, if so then advance, otherwise just return
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        // this handles identifying and skipping comments
        case '/':
            if (peekNext() == '/')
            {
                // keep consuming the characters until we get to the end or the new line
                // effectively consuming the entire comment
                while (peek() != '\n' && !isAtEnd())
                    advance();
            }
            else
            {
                return;
            }
            break;
        default:
            return;
        }
    }
}

// handles string literals
static Token string()
{
    while (peek() != '"' && !isAtEnd())
    {
        // handles multi-lin strings
        if (peek() == '\n')
            scanner.line++;
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");

    advance();
    return makeToken(TOKEN_STRING);
}

// checks if the character is a digit
static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static Token number()
{
    while (isDigit(peek()))
        advance();

    // look for a fractional part
    if (peek() == '.' && isDigit(peekNext()))
    {
        // consumer the "."
        advance();

        while (isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

// checks if the character is an identifier
static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// checks to see if the characters match a reserved keyword
static checkKeyword(int start, int length, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0)
    {
        return type;
    }
}

static TokenType identifierType()
{
    switch (scanner.start[0])
    {
    case 'a':
        return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c':
        return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e':
        return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'f':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'a':
                return checkKeyword(2, 3, "lse", TOKEN_FALSE);
            case 'o':
                return checkKeyword(2, 1, "r", TOKEN_FOR);
            case 'u':
                return checkKeyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    case 'i':
        return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n':
        return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o':
        return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p':
        return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r':
        return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
        return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 't':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'h':
                return checkKeyword(2, 2, "is", TOKEN_THIS);
            case 'r':
                return checkKeyword(2, 2, "ue", TOKEN_TRUE);
            }
        }
        break;
    case 'v':
        return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w':
        return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    };

    return TOKEN_IDENTIFIER;
}

// once we find an identifier we'll scan it here
static Token identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    return makeToken(identifierType());
}

// scans one token in at a time from the source code string
Token scanToken()
{
    skipWhitespace();
    scanner.start = scanner.current;
    if (isAtEnd())
        return makeToken(TOKEN_EOF);

    char c = advance();

    if (isAlpha(c))
        return identifier();

    if (isDigit(c))
        return number();

    switch (c)
    {
    case '(':
        return makeToken(TOKEN_LEFT_PAREN);
    case ')':
        return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
        return makeToken(TOKEN_LEFT_BRACE);
    case '}':
        return makeToken(TOKEN_RIGHT_BRACE);
    case ';':
        return makeToken(TOKEN_SEMICOLON);
    case ',':
        return makeToken(TOKEN_COMMA);
    case '.':
        return makeToken(TOKEN_DOT);
    case '-':
        return makeToken(TOKEN_MINUS);
    case '+':
        return makeToken(TOKEN_PLUS);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '*':
        return makeToken(TOKEN_STAR);
    case '!':
        return makeToken(
            match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
        return makeToken(
            match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
        return makeToken(
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"':
        return string();
    }

    return errorToken("Unexpected characater.");
}