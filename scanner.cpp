#include "scanner.h"
#include "execution.h"

Token::Token(TokenType type, std::string lexeme, int line)
{
    m_type = type;
    m_lexeme = lexeme;
    m_line = line;
}

std::string Token::getLexeme()
{
    return m_lexeme;
}

TokenType Token::getType()
{
    return m_type;
}

int Token::getLine()
{
    return m_line;
}


Scanner::Scanner(const std::string& source)
{
    m_source = source;
    m_tokens = {};
}

char Scanner::advance()
{
    return m_source.at(m_current++);
}

void Scanner::addToken(TokenType type, std::string stringValue)
{
    //TODO: do something with stringValue in later steps/reorganization
    stringValue = "";
    std::string temporaryLexeme = m_source.substr(m_start, m_current - m_start);
    m_tokens.push_back(Token(type, temporaryLexeme, m_line));
}

bool Scanner::match(const char& expected)
{
    if (m_current >= int(m_source.length()))
        return false;
    if (m_source.at(m_current) != expected)
        return false;

    m_current++;
    return true;
}

char Scanner::peekChar()
{
    if (m_current >= int(m_source.length()))
        return '\0';
    return m_source.at(m_current);
}

char Scanner::peekNextChar()
{
    if (m_current + 1 >= int(m_source.length()))
        return '\0';
    return m_source.at(m_current + 1);
}

void Scanner::processString()
{
    while (peekChar() != '"' && m_current < int(m_source.length()))
    {
        if (peekChar() == '\n')
            m_line++;
        advance();
    }

    if (m_current >= int(m_source.length()))
    {
        error(m_line, "Unterminated string.");
        return;
    }

    advance();

    //trim the quotes surrounding our string value
    std::string stringValue = m_source.substr(m_start + 1, (m_current - 1 - m_start));
    addToken(TOKEN_STRING, stringValue);
}

bool Scanner::isDigit(const char& character)
{
    return character >= '0' && character <= '9';
}

bool Scanner::isAlpha(const char& character)
{
    return (character >= 'a' && character <= 'z') ||
           (character >= 'A' && character <= 'Z') ||
           (character == '_');
}

bool Scanner::isAlphaNumeric(const char& character)
{
    return isAlpha(character) || isDigit(character);
}

void Scanner::processIdentifier()
{
    while (isAlphaNumeric(peekChar()))
    {
        advance();
    }

    std::string text = m_source.substr(m_start, m_current - m_start);
    TokenType type = TOKEN_IDENTIFIER;

    if (m_keywords.find(text) != m_keywords.end())
    {
        type = m_keywords.at(text);
    }

    addToken(type, "");
}

void Scanner::processNumber()
{
    while (isDigit(peekChar()))
    {
        advance();
    }

    if (peekChar() == '.' && isDigit(peekNextChar()))
    {
        advance();

        while (isDigit(peekChar()))
        {
            advance();
        }
    }

    addToken(TOKEN_NUMBER, m_source.substr(m_start, m_current - m_start));
}

void Scanner::scanToken()
{
    char tempCharacter = advance();

    //TODO: try to refactor by assigning temporaryToken to the token value and running addToken at the end of the function
    //TokenType temporaryToken;

    switch (tempCharacter)
    {
        case '(':
            //temporaryToken = TOKEN_LEFT_PAREN;
            addToken(TOKEN_LEFT_PAREN, "");
            break;
        case ')':
            addToken(TOKEN_RIGHT_PAREN, "");
            break;
        case '{':
            addToken(TOKEN_LEFT_BRACE, "");
            break;
        case '}':
            addToken(TOKEN_RIGHT_BRACE, "");
            break;
        case ',':
            addToken(TOKEN_COMMA, "");
            break;
        case '.':
            addToken(TOKEN_DOT, "");
            break;
        case '-':
            addToken(TOKEN_MINUS, "");
            break;
        case '+':
            addToken(TOKEN_PLUS, "");
            break;
        case ';':
            addToken(TOKEN_SEMICOLON, "");
            break;
        case '*':
            addToken(TOKEN_STAR, "");
            break;
        case '!':
            addToken(match('=') ? TOKEN_NOT_EQUAL : TOKEN_NOT, "");
            break;
        case '=':
            addToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL, "");
            break;
        case '<':
            addToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS, "");
            break;
        case '>':
            addToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER, "");
            break;
        case '/':
            if (match('/'))
            {
                while (peekChar() != '\n' && m_current < int(m_source.length()))
                {
                    advance();
                }
            }
            else if (match('*'))
            {
                while ((peekChar() != '*' || peekNextChar() != '/') && m_current < int(m_source.length()))
                {
                    advance();
                }

                //collect multiline comment ending
                if (m_current < int(m_source.length()))
                {
                    advance();
                    advance();
                }
            }
            else
            {
                addToken(TOKEN_SLASH, "");
            }
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '\n':
            m_line++;
            break;
        case '"':
            processString();
            break;
        default:
            if (isDigit(tempCharacter))
            {
                processNumber();
            }
            else if (isAlpha(tempCharacter))
            {
                processIdentifier();
            }
            else
            {
                error(m_line, "Unexpected character");
            }
            break;
    }
}

std::vector<Token> Scanner::scanTokens()
{
    while (m_current < int(m_source.length()))
    {
        m_start = m_current;
        scanToken();
    }

    m_tokens.push_back(Token(TOKEN_EOF, "", m_line));
    return m_tokens;
}