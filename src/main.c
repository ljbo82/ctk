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
#include <signal.h>
#include <string.h>
#include <stdlib.h>

typedef void (SigHandler)(int);

static void __install_signal_handler(int signum, SigHandler handler) {
	#if CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT
		signal(signum, handler);
	#else
		struct sigaction sa;
		sa.sa_flags = SA_RESETHAND;
		sa.sa_handler = handler;
		sigemptyset(&(sa.sa_mask));

		int err = sigaddset(&(sa.sa_mask), signum);
		if (err == 0)
			err = sigaction(signum, &sa, NULL);

		if (err != 0) {
			fputs("Error while installing handler", stdout);
			exit(1);
		}
	#endif
}

static void __sig_handler(int signum) {
	__install_signal_handler(signum, __sig_handler);

	switch (signum) {
	case SIGSEGV:
		cthrow(ctk_exception_get_global_context(), SIGSEGV, "Segfault");
		break;

	case SIGFPE:
		cthrow(ctk_exception_get_global_context(), SIGFPE, "Arithmetic error");
		break;

	default:
		break;
	}
}

#if CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
	#include <pthread.h>

	static void __thread_segfault() {
		CtkExceptionContext* globalContext = ctk_exception_get_global_context();

		ctry(globalContext) {
			int* i = 0;
			(void)i;
			*i = 1;
		} ccatch {
			fprintf(stdout, "Exception detected: thread: %d, code: %d, message: %s\n", (int)pthread_self(), globalContext->errCode, globalContext->errMsg);
		}
	}

	static void __thread_ordinary_error() {
		CtkExceptionContext ctx;

		ctry (&ctx) {
			cthrow(&ctx, 129, "Ordinary error");
		} ccatch {
			fprintf(stdout, "Exception detected: thread: %d, code: %d, message: %s\n", (int)pthread_self(), ctx.errCode, ctx.errMsg);
		}
	}

	static void* __thread_fn(void* v) {
		CtkExceptionContext globalContext;
		ctk_exception_set_global_context(&globalContext);

		__thread_segfault();
		__thread_ordinary_error();
		return NULL;
	}

	static int __thread_main(int argc, char** argv) {
		__install_signal_handler(SIGSEGV, __sig_handler);
		__install_signal_handler(SIGFPE, __sig_handler);

    	pthread_t threads[4];

    	for (int i = 0; i < 4; i++) {
    		pthread_create(&threads[i], NULL, __thread_fn, NULL);
    	}

    	for (int i = 0; i < 4; i++) {
			pthread_join(threads[i], NULL);
		}

    	return 0;
	}
#else
	static void __segfault() {
		CtkExceptionContext* globalContext = ctk_exception_get_global_context();

		ctry(globalContext) {
			int* i = 0;
			(void)i;
			*i = 1;
		} ccatch {
			fprintf(stdout, "Exception detected: code: %d, message: %s\n", globalContext->errCode, globalContext->errMsg);
		}
	}

	static void __arithmetic() {
		CtkExceptionContext* globalContext = ctk_exception_get_global_context();

		ctry(globalContext) {
			int b = 0;

			int c;
			(void)c;

			c = 12 / b;
		} ccatch {
			fprintf(stdout, "Exception detected: code: %d, message: %s\n", globalContext->errCode, globalContext->errMsg);
		}
	}

	static void __ordinary_error() {
		CtkExceptionContext ctx;

		ctry (&ctx) {
			cthrow(&ctx, 129, "Ordinary error");
		} ccatch {
			fprintf(stdout, "Exception detected: code: %d, message: %s\n", ctx.errCode, ctx.errMsg);
		}
	}

	static int __main(int argc, char** argv) {
		__install_signal_handler(SIGSEGV, __sig_handler);
		__install_signal_handler(SIGFPE, __sig_handler);

		CtkExceptionContext globalContext;
		ctk_exception_set_global_context(&globalContext);

		for (int i = 0; i < 10; i++) {
			if (i > 0)
				fputs("\n", stdout);

			__segfault();
			__arithmetic();
		}
		__ordinary_error();

		return 0;
	}
#endif

int main(int argc, char** argv) {
	#if CTK_EXCEPTION_ENABLE_THREAD_SUPPORT
		return __thread_main(argc, argv);
	#else
		return __main(argc, argv);
	#endif
}


