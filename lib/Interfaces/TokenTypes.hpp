/*
 * Pas2CTrans - Transcoder for Pascal to plain old C code
 * Copyright (C) 2013 Sascha Cunz <sascha.cunz@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PASC_TOKEN_TYPES_HPP
#define PASC_TOKEN_TYPES_HPP

enum TokenType {

    T_DOT,
    T_COMMA,
    T_SEMICOLON,
    T_COLON,
    T_LEFT_PAREN,
    T_RIGHT_PAREN,
    T_LEFT_ANGLE,
    T_RIGHT_ANGLE,
    T_PLUS,
    T_MINUS,
    T_ASTERIKS,
    T_SLASH,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_AT_SIGN,
    T_CARET,
    T_EQUAL,

    T_ASSIGNMENT,
    T_LESS_THAN_EQUAL,
    T_GREATER_THAN_EQUAL,
    T_INEQUAL,
    T_ELLIPSIS,

    T_BEGIN,
    T_END,
    T_PROGRAM,
    T_INTERFACE,
    T_IMPLEMENTATION,
    T_UNIT,
    T_PROCEDURE,
    T_VAR,
    T_CONST,
    T_TYPE,
    T_FUNCTION,

    T_IF,
    T_CASE,
    T_ELSE,
    T_THEN,
    T_FOR,
    T_WHILE,
    T_DO,
    T_REPEAT,
    T_UNTIL,
    T_TO,
    T_DOWNTO,
    T_WITH,

    T_AND,
    T_OR,
    T_NOT,
    T_IN,
    T_AS,
    T_SHL,
    T_SHR,
    T_DIV,
    T_MOD,

    // Stock Data Types:
    T_STRING,
    T_CHAR,
    T_BOOLEAN,
    T_POINTER,
    T_INTEGER,
    T_FLOAT,
    T_TEXT,
    T_FILE,

    T_RECORD,
    T_ARRAY,
    T_SET,

    T_OF,
    T_USES,

    T__STRING,
    T__NUMBER,
    T__FLOAT,
    //T__CHAR,
    T__IDENTIFIER,

    T__ERROR,
    T__END_OF_STREAM
};

#endif
