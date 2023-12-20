#ifndef __JSCommandEncoder_H__
#define __JSCommandEncoder_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/3D/temp/CommandEncoder.h>

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