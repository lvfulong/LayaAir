#include "stdafx.h"
#if BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif

int main(int argc, _TCHAR* argv[])
{
	return conchMain(argc, argv);
}
