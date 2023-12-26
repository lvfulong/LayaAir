#ifndef __JSShaderPrecisionFormat_H__
#define __JSShaderPrecisionFormat_H__

#include <stdio.h>
#include <string>
#include <map>
#include <binder/JSInterface.h>

namespace laya
{
    class JSShaderPrecisionFormat
    {
    public:

	    static void exportJS(Context& context);

        JSShaderPrecisionFormat();

	    ~JSShaderPrecisionFormat();

        int getRangeMin();

        int getRangeMax();

        int getPrecison();

    public:

        int m_nRangeMin;
        int m_nRangeMax;
        int m_nPrecision;

    };
}
#endif