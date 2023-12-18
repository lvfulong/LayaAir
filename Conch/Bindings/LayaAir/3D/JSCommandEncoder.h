#ifndef __JSCommandEncoder_H__
#define __JSCommandEncoder_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/CommandEncoder.h"

namespace laya
{
	class JSCommandEncoder
	{
	public:

		static void exportJS(Context& context);
		JSCommandEncoder();
		JSCommandEncoder(CommandEncoder* pCommandEncoder);
		~JSCommandEncoder();

	public:

	public:
		CommandEncoder* m_pCommandEncoder;
	};
}
#endif //__JSCommandEncoder_H__