
#include <QStringBuilder>

#include "lexer/lexanal.hpp"
#include "lexer/token.hpp"

struct TokenDef {
    const char*     szTokenText;
    TokenType       type;
};

#define RAW_TOKEN(token,type) {token, type}
#define OPER_TOKEN(token,type) RAW_TOKEN(token, type)
#define TEXT_TOKEN(token,type) RAW_TOKEN(#token, type)

static TokenDef textTokens[] = {
    TEXT_TOKEN(begin,       T_BEGIN),
    TEXT_TOKEN(end,         T_END),
    TEXT_TOKEN(program,     T_PROGRAM),
    RAW_TOKEN(NULL,         T__ERROR)
};

static TokenDef operTokens[] = {
    // reverse sort by token text length
    OPER_TOKEN(":=", T_ASSIGNMENT),
    OPER_TOKEN("<=", T_LESS_THAN_EQUAL),
    OPER_TOKEN(">=", T_GREATER_THAN_EQUAL),
    OPER_TOKEN("<>", T_INEQUAL),
    OPER_TOKEN("..", T_ELLIPSIS),

    OPER_TOKEN("+", T_PLUS),
    OPER_TOKEN("-", T_MINUS),
    OPER_TOKEN("/", T_SLASH),
    OPER_TOKEN("*", T_ASTERIKS),
    OPER_TOKEN("<", T_LEFT_ANGLE),
    OPER_TOKEN(">", T_RIGHT_ANGLE),
    OPER_TOKEN("(", T_LEFT_PAREN),
    OPER_TOKEN(")", T_RIGHT_PAREN),
    OPER_TOKEN("[", T_LEFT_SQUARE),
    OPER_TOKEN("]", T_RIGHT_SQUARE),
    OPER_TOKEN(",", T_COMMA),
    OPER_TOKEN(":", T_COLON),
    OPER_TOKEN(";", T_SEMICOLON),
    OPER_TOKEN("@", T_AT_SIGN),
    OPER_TOKEN("^", T_CARET),
    RAW_TOKEN(NULL,         T__ERROR)
};

TokenType isTextToken(const char* token) {
    int pos = -1;
    while (textTokens[++pos].szTokenText) {
        if (!strcasecmp(textTokens[pos].szTokenText, token))
            break;
    }
    return textTokens[pos].type;
}

TokenType isOperToken(const char* token) {
    int pos = -1;
    while (operTokens[++pos].szTokenText) {
        if (!strcmp(operTokens[pos].szTokenText, token))
            break;
    }
    return operTokens[pos].type;
}

LexicalAnalyzer::LexicalAnalyzer(Errors::Ptr errors, const QString& fileName)
    : mErrors(errors)
    , mInputStream(new InputStream(errors, fileName))
    , mIsAnalyed(false)
{
}

class Analyzer {
public:
    QVector< Token > run(InputStream::Ptr is, Errors::Ptr errors) {
        mStream = is;
        mData = is->data();
        mFileName = is->fileName();
        mPos = 0;
        mLine = mColumn = 1;

        while (!eof()) {
            skipSpace();
            if (eof()) {
                return mTokens;
            }

            char ch = mData[mPos];
            if (ch == '{') {
                if (!skipBraceComment()) {
                    return QVector<Token>();
                }
                continue;
            }
            else if (ch == '(' && mPos + 1 < mData.count() && mData[mPos+1] == '*') {
                mColumn += 2;
                mPos += 2;
                if (!skipParenComment()) {
                    return QVector<Token>();
                }
                continue;
            }
            else if (ch >= '0' && ch <= '9') {
                int c = mColumn;
                int i = 0;
                do {
                    i *= 10;
                    i += ch - '0';
                    mColumn++; mPos++;
                    if (eof()) { // numeric at EOF will be handled by parser
                        break;
                    }
                    ch = mData[mPos];
                } while (ch >= '0' && ch <= '9');
                mTokens.append(Token(InputStreamRef(mStream, mLine, c), i));
                continue;
            }
            else if ((ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                int c = mColumn;
                QByteArray s;
                do {
                    s += ch;
                    mColumn++; mPos++;
                    if (eof()) { // wired identifer at EOF will be handled by parser
                        break;
                    }
                    ch = mData[mPos];
                } while ((ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
                TokenType tt = isTextToken(s.constData());
                if (tt == T__ERROR) {
                    mTokens.append(Token(InputStreamRef(mStream, mLine, c), T__IDENTIFIER, s));
                }
                else {
                    mTokens.append(Token(InputStreamRef(mStream, mLine, c), tt));
                }
                continue;
            }
            else if (ch == '\'') {
                int c = mColumn;
                QByteArray s;
                mColumn++; mPos++;

                if (eof()) {
                    mErrors->emitError(QLatin1String("Unfinished String"), mFileName, mLine, c);
                    return QVector<Token>();
                }

                ch = mData[mPos];
                do {
                    if (ch == '\n') {
                        mErrors->emitError(QLatin1String("Linebreak in String"), mFileName, mLine, c);
                        return QVector<Token>();
                    }

                    s += ch;

                    mColumn++; mPos++;
                    ch = mData[mPos];

                    if (eof()) {
                        mErrors->emitError(QLatin1String("Unfinished String"), mFileName, mLine, c);
                        return QVector<Token>();
                    }

                } while (ch != '\'');

                mTokens.append(Token(InputStreamRef(mStream, mLine, c), T__STRING, s));
                continue;
            }
            else {
                int c = mColumn;
                QByteArray s;
                do {
                    s += ch;
                    mColumn++; mPos++;
                    if (eof()) { // oper tokens are right at EOF
                        break;
                    }
                    ch = mData[mPos];
                } while ((ch != '_') && (ch != '{') && (ch != '(') && (ch > ' ') &&
                         !(ch >= 'a' && ch <= 'z') &&
                         !(ch >= 'A' && ch <= 'Z') &&
                         !(ch >= '0' && ch <= '9') );

                bool found = false;
                while (s.count() > 0) {
                    TokenType tt = isOperToken(s.constData());
                    if (tt != T__ERROR) {
                        mTokens.append(Token(InputStreamRef(mStream, mLine, c), tt));
                        found = true;
                        break;
                    }
                    mPos--; mColumn--; s = s.left(s.length()-1);
                }

                if (found) {
                    continue;
                }

                mErrors->emitError(QLatin1Literal("Found dizzy stray char '") % QLatin1Char(ch) % QChar(L'\''), mFileName, mLine, c);
                return QVector<Token>();
            }
        }
    }

private:
    bool skipParenComment() {
        while (!eof()) {
            switch(mData[mPos]) {
            case '\n':
                mLine++;
                mColumn = 1;
                mPos++;
                continue;

            case ' ':
            case '\t':
                mColumn++;
            case '\r':
                mPos++;
                continue;

            case '*':
                mColumn++;
                mPos++;
                if (!eof() && mData[mPos] == ')') {
                    mColumn++;
                    mPos++;
                    return true;
                }
                continue;

            default:
                mColumn++;
                break;
            }
        }

        mErrors->emitError(QLatin1String("Unfinished comment"), mFileName, mLine, mColumn);
        return false;
    }

    bool skipBraceComment() {
        while (mPos < mData.count()) {
            switch(mData[mPos]) {
            case '\n':
                mLine++;
                mColumn = 1;
                mPos++;
                continue;

            case ' ':
            case '\t':
                mColumn++;
            case '\r':
                mPos++;
                continue;

            case '}':
                mColumn++;
                mPos++;
                return true;

            default:
                mColumn++;
                break;
            }
        }

        mErrors->emitError(QLatin1String("Unfinished comment"), mFileName, mLine, mColumn);
        return false;
    }

    bool eof() const {
        return mPos >= mData.count();
    }

    void skipSpace() {
        while (!eof()) {
            switch(mData[mPos]) {
            case '\n':
                mLine++;
                mColumn = 1;
                mPos++;
                continue;

            case ' ':
            case '\t':
                mColumn++;
            case '\r':
                mPos++;
                continue;

            default:
                return;
            }
        }
    }

private:
    InputStream::Ptr mStream;
    Errors::Ptr mErrors;
    QByteArray mData;
    QString mFileName;
    int mPos;
    int mLine;
    int mColumn;
    QVector< Token > mTokens;
};

bool LexicalAnalyzer::analyze() {

    Q_ASSERT(!mIsAnalyed);
    Analyzer a;
    mTokens.init(a.run(mInputStream, mErrors));

    return false;
}


Tokens LexicalAnalyzer::tokens() {
    Q_ASSERT(mIsAnalyed);
    return mTokens;
}
