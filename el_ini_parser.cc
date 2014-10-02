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

enum class Type {
  TYPE_ERR = 0,
  TYPE_EOF,
  TYPE_COMMENT,

  TYPE_VALUE,

  TYPE_LBRACKET,
  TYPE_RBRACKET,
  TYPE_ASSIGN,
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

  Type GetToken(std::string& token);
private:
  int GetChar(void);
  void UngetChar(void);

  Type LexerBegin(int c, State& out_state, bool& out_save);
  Type LexerFinish(int c, State& out_state, bool& out_save);
  Type LexerValue(int c, State& out_state, bool& out_save);
  Type LexerComment(int c, State& out_state, bool& out_save);
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
  file_.reset(fopen(fname, "r"), fclose);
  if (!file_)
    return false;

  lineno_ = 1;
  bsize_  = 0;
  eof_    = false;
  lexpos_ = 0;

  return true;
}

void IniLexer::Close(void) {
}

Type IniLexer::GetToken(std::string& token) {
  Type type = Type::TYPE_ERR;
  State state = State::STATE_BEGIN;
  bool  save;
  int   c;

  token.clear();
  while (State::STATE_FINISH != state) {
    c = GetChar();
    save = true;

    switch (state) {
    case State::STATE_BEGIN:
      type = LexerBegin(c, state, save);
      break;
    case State::STATE_FINISH:
      type = LexerFinish(c, state, save);
      break;
    case State::STATE_VALUE:
      type = LexerValue(c, state, save);
      break;
    case State::STATE_COMMENT:
      type = LexerComment(c, state, save);
      break;
    }

    if (save)
      token += static_cast<char>(c);
  }

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

Type IniLexer::LexerBegin(
    int c, State& out_state, bool& out_save) {
  Type type = Type::TYPE_ERR;

  if (' ' == c || '\t' == c) {
    out_save = false;
  }
  else if ('\n' == c) {
    out_save = false;
    ++lineno_;
  }
  else if ('#' == c) {
    out_state = State::STATE_COMMENT;
    out_save = false;
  }
  else if ('[' == c) {
    out_state = State::STATE_FINISH;
    type = Type::TYPE_LBRACKET;
  }
  else if (']' == c) {
    out_state = State::STATE_FINISH;
    type = Type::TYPE_RBRACKET;
  }
  else if ('=' == c) {
    out_state = State::STATE_FINISH;
    type = Type::TYPE_ASSIGN;
  }
  else if (EOF == c) {
    out_state = State::STATE_FINISH;
    out_save = false;
    type = Type::TYPE_EOF;
  }
  else {
    out_state = State::STATE_VALUE;
  }

  return type;
}

Type IniLexer::LexerFinish(
    int c, State& out_state, bool& out_save) {
  out_state = State::STATE_FINISH;
  out_save = false;

  return Type::TYPE_ERR;;
}

Type IniLexer::LexerValue(
    int c, State& out_state, bool& out_save) {
  if (' ' == c || '\t' == c || '\n' == c
      || '#' == c || '=' == c || ']' == c) {
    if ('\n' == c || '#' == c || '=' == c || ']' == c)
      UngetChar();

    out_state = State::STATE_FINISH;
    out_save = false;

    return Type::TYPE_VALUE;
  }

  return Type::TYPE_ERR;
}

Type IniLexer::LexerComment(
    int c, State& out_state, bool& out_save) {
  out_save = false;
  if (EOF == c) {
    out_state = State::STATE_FINISH;
    return Type::TYPE_EOF;
  }
  else if ('\n' == c) {
    ++lineno_;
    out_state = State::STATE_BEGIN;
  }

  return Type::TYPE_ERR;
}


IniParser::IniParser(void)
  : lexer_(IniLexerPtr(new IniLexer()))
  , section_("") {
}

IniParser::~IniParser(void) {
}

bool IniParser::Open(const char* fname) {
  if (!lexer_->Open(fname))
    return false;
  section_ = "";
  values_.clear();

  Parse();

  return true;
}

void IniParser::Close(void) {
  if (lexer_) {
    lexer_->Close();

    section_ = "";
    values_.clear();
  }
}

std::string IniParser::Get(
    const std::string& section, const std::string& key) {
  std::string value_key(section + key);

  IniValueMap::iterator found(values_.find(value_key));
  if (found != values_.end())
    return (*found).second;

  return std::string("");
}

void IniParser::Parse(void) {
  std::string token;
  Type type;
  do {
    type = lexer_->GetToken(token);
    if (Type::TYPE_LBRACKET == type)
      ParseSection();
    else if (Type::TYPE_VALUE == type)
      ParseValue(token);
  } while(Type::TYPE_EOF != type);
}

void IniParser::ParseSection(void) {
  std::string token;
  Type type = lexer_->GetToken(token);
  EL_ASSERT(Type::TYPE_VALUE == type);

  section_ = token;
  EL_ASSERT(Type::TYPE_RBRACKET == lexer_->GetToken(token));
}

void IniParser::ParseValue(const std::string& value) {
  std::string token;
  EL_ASSERT(Type::TYPE_ASSIGN == lexer_->GetToken(token));

  EL_ASSERT(Type::TYPE_VALUE == lexer_->GetToken(token));

  std::string key(section_ + value);
  values_.insert(std::make_pair(key, token));
}

}
