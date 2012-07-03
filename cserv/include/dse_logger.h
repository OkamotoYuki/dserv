/****************************************************************************
 * Copyright (c) 2012, the Konoha project authors. All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

/* ************************************************************************ */

#ifndef DSE_LOGGER_H_
#define DSE_LOGGER_H_

#include <sys/time.h>
#include "config.h"

#ifdef HAVE_LIBLOGPOOL
#include <logpool/logpool.h>
#else
typedef void logpool_t;

#define LOGPOOL_TRACE 0
#define LOG_NOTICE 0

void logpool_global_init(int mode) {}
logpool_t *logpool_open_trace(logpool_t *parent, char *host, int port) { return parent; }
void logpool_close(logpool_t *p) {}
void logpool_global_exit(void) {}
void logpool_record(logpool_t *logpool, void *args, int priority, char *trace_id, ...) {}

#endif /* HAVE_LIBLOGPOOL */

int getTime(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	D_("%d[s] %d[us]", (int)t.tv_sec % 1000, (int)t.tv_usec);
	return (int)(((int)t.tv_sec % 1000) * 1000 * 1000 + (int)t.tv_usec);
}

#define LOG_END 0
#define LOG_s   1
#define LOG_u   2

#define KEYVALUE_u(K,V)    LOG_u, (K), ((uintptr_t)V)
#define KEYVALUE_s(K,V)    LOG_s, (K), (V)

static void dse_logpool_init(void) {
	logpool_global_init(LOGPOOL_TRACE);
}

logpool_t *dse_openlog(char *ip)
{
	logpool_t *lp = logpool_open_trace(NULL, ip, 14801);
	return lp;
}

static void dse_closelog(logpool_t *lp)
{
	logpool_close(lp);
}

static void dse_logpool_exit()
{
	logpool_global_exit();
}

#define dse_record(lp, args, trace_id, ...) \
	logpool_record(lp, args, LOG_NOTICE, trace_id, \
			KEYVALUE_u("time", getTime()), \
			__VA_ARGS__)



#endif /* DSE_LOGGER_H_ */
