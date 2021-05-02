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

#include <ctk/exception.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#if CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
	#include <pthread.h>

	static pthread_key_t  __globalKey;
	static pthread_once_t __keyOnce = PTHREAD_ONCE_INIT;

	static void __ctk_exception_create_key() {
	    (void) pthread_key_create(&__globalKey, NULL);
	}
#else
	static CtkExceptionContext* globalContext = NULL;
#endif

void ctk_exception_set_global_context(const CtkExceptionContext* context) {
	#if CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
		int result;
		if ((result = pthread_once(&__keyOnce, __ctk_exception_create_key)) != 0)
			cthrow(NULL, 0, "pthread_once() error: %d", result);

		if ((result = pthread_setspecific(__globalKey, context)) != 0 )
			cthrow(NULL, 0, "pthread_setspecific() error: %d", result);
	#else
		globalContext = (CtkExceptionContext*) context;
	#endif
}

CtkExceptionContext* ctk_exception_get_global_context() {
	#if CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
		int result;
		if ((result = pthread_once(&__keyOnce, __ctk_exception_create_key)) != 0)
			cthrow(NULL, 0, "pthread_once() error: %d", result);

		return pthread_getspecific(__globalKey);
	#else
		return globalContext;
	#endif
}

void ctk_exception_throw(CtkExceptionContext* context, int errCode, cstring_t errMsg, ...) {
	va_list args;
	va_start(args, errMsg);

	char msg[CTK_EXCEPTION_MSG_MAX_LEN];
	vsnprintf(msg, sizeof(msg), errMsg, args);
    va_end(args);

	if (context == NULL) {
		fprintf(stderr, "Fatal error (code: %d, message: %s)\n", errCode, msg);
		exit(1);
	} else {
		context->errCode = errCode;
		strcpy(context->errMsg, msg);
		#if CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT
			siglongjmp(context->jmpBuf, 1);
		#else
			longjmp(context->jmpBuf, 1);
		#endif
	}
}

