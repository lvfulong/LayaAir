#ifndef __TRACE_H__
#define __TRACE_H__

#if !defined(TRACE_ENABLED)
#define TRACE_NAME_BEGIN(name)
#define TRACE_NAME_END(name)
#define TRACE_NAME_DUMP()
#else
#include <utils/Tracer.h>
#endif

#endif