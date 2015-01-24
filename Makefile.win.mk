# Copyright (c) 2014 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

OUT_LIB	= libeutil.lib
OUT_BIN	= eutil.exe
OUT	= $(OUT_LIB) $(OUT_BIN)
RM	= del 
CC	= cl -c -nologo
AR	= lib -nologo
MT	= mt -nologo
LINK	= link -nologo
CFLAGS	= -O2 -W3 -MDd -GS -Zi -Fd"vc.pdb" -EHsc -D_DEBUG\
	-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -wd4290
LDFLAGS	= -INCREMENTAL -DEBUG -PDB:$(OUT_BIN).pdb -manifest\
	-manifestfile:$(OUT_BIN).manifest -manifestuac:no\
	winmm.lib libeutil.lib
OBJS_LIB= el_win_io.obj el_win_condition.obj\
	\
	el_io.obj el_time.obj el_thread_pool.obj el_logging.obj el_unit.obj\
	el_symbol_table.obj el_ini_parser.obj el_mini_buffer.obj
OBJS_BIN= el_main.obj el_locker_test.obj el_condition_test.obj\
	el_singleton_test.obj el_object_pool_test.obj el_object_mgr_test.obj\
	el_thread_test.obj el_thread_pool_test.obj el_logging_test.obj\
	el_time_test.obj el_symbol_table_test.obj el_io_test.obj\
	el_ini_parser_test.obj
OBJS	= $(OBJS_LIB) $(OBJS_BIN)





all: $(OUT)

rebuild: clean all

clean:
	$(RM) $(OUT) $(OBJS) *.pdb *.ilk *.manifest





$(OUT_LIB): $(OBJS_LIB)
	$(AR) -out:$(OUT_LIB) $(OBJS_LIB)

$(OUT_BIN): $(OBJS_BIN)
	$(LINK) -out:$(OUT_BIN) $(OBJS_BIN) $(LDFLAGS)
	$(MT) -manifest $(OUT_BIN).manifest -outputresource:$(OUT_BIN);1

.cc.obj:
	$(CC) $(CFLAGS) $<

{.\win}.cc{}.obj:
	$(CC) $(CFLAGS) $<

{.\test}.cc{}.obj:
	$(CC) $(CFLAGS) $<
