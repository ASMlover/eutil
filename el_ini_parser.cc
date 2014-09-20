// Copyright (c) 2014 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "eutil.h"
#include "el_ini_parser.h"

namespace el {

struct Token {
  enum class Type {
    TYPE_ERR = 0, 
    TYPE_EOF, 
    TYPE_COMMENT, 

    TYPE_VALUE, 

    TYPE_LBRACKET, 
    TYPE_RBRACKET, 
    TYPE_ASSIGN,
  };

  Type        type;
  std::string value;
};

class IniLexer : private UnCopyable {
  enum {BSIZE = 256};
  enum class State {
    STATE_BEGIN = 0, 
    STATE_FINISH, 

    STATE_VALUE, 
    STATE_COMMENT, 
  };

  typedef std::shared_ptr<FILE> FilePtr;

  int     lineno_;
  int     bsize_;
  bool    eof_;
  FilePtr file_;
  int     lexpos_;
  char    lexbuf_[BSIZE];
public:
  IniLexer(void);
  ~IniLexer(void);

  bool Open(const char* fname);
  void Close(void);

  Token::Type GetToken(Token& token);
private:
  int GetChar(void);
  void UngetChar(void);

  Token::Type LexerBegin(int c, State& out_state, bool& out_save);
  Token::Type LexerFinish(int c, State& out_state, bool& out_save);
  Token::Type LexerValue(int c, State& out_state, bool& out_save);
  Token::Type LexerComment(int c, State& out_state, bool& out_save);
};

IniLexer::IniLexer(void)
  : lineno_(1)
  , bsize_(0)
  , eof_(false)
  , file_(FilePtr(nullptr))
  , lexpos_(0) {
}

IniLexer::~IniLexer(void) {
}

bool IniLexer::Open(const char* fname) {
  return true;
}

void IniLexer::Close(void) {
}

Token::Type IniLexer::GetToken(Token& token) {
  Token::Type type = Token::Type::TYPE_ERR;

  return type;
}

int IniLexer::GetChar(void) {
  if (lexpos_ >= bsize_) {
    if (nullptr != fgets(lexbuf_, BSIZE, file_.get())) {
      bsize_ = static_cast<int>(strlen(lexbuf_));
      lexpos_ = 0;
    }
    else {
      eof_ = true;
      return EOF;
    }
  }

  return lexbuf_[lexpos_++];
}

void IniLexer::UngetChar(void) {
  if (!eof_)
    --lexpos_;
}

Token::Type IniLexer::LexerBegin(
    int c, State& out_state, bool& out_save) {
  Token::Type type = Token::Type::TYPE_ERR;

  return type;
}

Token::Type IniLexer::LexerFinish(
    int c, State& out_state, bool& out_save) {
  Token::Type type = Token::Type::TYPE_ERR;

  return type;
}

Token::Type IniLexer::LexerValue(
    int c, State& out_state, bool& out_save) {
  Token::Type type = Token::Type::TYPE_ERR;

  return type;
}

Token::Type IniLexer::LexerComment(
    int c, State& out_state, bool& out_save) {
  Token::Type type = Token::Type::TYPE_ERR;

  return type;
}


IniParser::IniParser(void)
  : lexer_(IniLexerPtr(new IniLexer()))
  , section_("") {
}

IniParser::~IniParser(void) {
}

bool IniParser::Open(const char* fname) {
  return true;
}

void IniParser::Close(void) {
}

std::string IniParser::Get(
    const std::string& section, const std::string& key) {
  return "";
}

void IniParser::Parse(void) {
}

void IniParser::ParseSection(void) {
}

void IniParser::ParseValue(void) {
}

}
