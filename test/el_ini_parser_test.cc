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
#include "el_test.h"
#include "../el_ini_parser.h"

UNIT_IMPL(IniParser) {
  el::IniParser parser;

  EL_ASSERT(parser.Open("./test/ini_parser.ini"));

  UNIT_PRINT("SECTION1.key1 = %s\n",
      parser.Get("SECTION1", "key1").c_str());
  UNIT_PRINT("SECTION1.key2 = %s\n",
      parser.Get("SECTION1", "key2").c_str());
  UNIT_PRINT("SECTION1.key3 = %s\n",
      parser.Get("SECTION1", "key3").c_str());
  UNIT_PRINT("SECTION1.key4 = %s\n",
      parser.Get("SECTION1", "key4").c_str());

  UNIT_PRINT("SECTION2.key = %s\n",
      parser.Get("SECTION2", "key").c_str());
}
