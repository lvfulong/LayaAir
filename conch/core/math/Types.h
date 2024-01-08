#ifndef __Types_H__
#define __Types_H__

namespace laya
{
typedef float F32;
typedef double F64;
#if defined(USE_DOUBLE_PRECISION)
typedef double Real;
#else
typedef float Real;
#endif
} // namespace laya
#endif
