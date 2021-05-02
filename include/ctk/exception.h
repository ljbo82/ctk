/*
     This file is part of ctk
     Copyright (C) 2021 - Leandro José Britto de Oliveira

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <ctk/defs.h>

#include <setjmp.h>

// Disabled on windows and bare metal. On Linux, must be enabled
#ifndef CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT
	#define CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT 1
#endif

#ifndef CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
	#define CTK_EXCEPTION_ENABLE_THREAD_SUPPORT 1
#endif

#define CTK_EXCEPTION_MSG_MAX_LEN 256

#if CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT
	#define ctry(_ctx) if (sigsetjmp((_ctx)->jmpBuf, 1) == 0)
#else
	#define ctry(_ctx) if (setjmp((_ctx)->jmpBuf) == 0)
#endif

#define ccatch else
#define cthrow(_ctx, _errCode, _errMsg, ...) ctk_exception_throw(_ctx, _errCode, _errMsg, ##__VA_ARGS__)

typedef struct CtkExceptionContext {
	#if CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT
		sigjmp_buf jmpBuf;
	#else
		jmp_buf jmpBuf;
	#endif
	int errCode;
	char errMsg[CTK_EXCEPTION_MSG_MAX_LEN];

} CtkExceptionContext;

void ctk_exception_set_global_context(const CtkExceptionContext* context);

CtkExceptionContext* ctk_exception_get_global_context();

void ctk_exception_throw(CtkExceptionContext* context, int errCode, cstring_t errMsg, ...);
