
#if BUILDING_CONCH_FROME_SOURCE
#include "Exports.h"
#else
#include "ExportsShared.h"
#endif

int main(int argc, char* argv[])
{
	return conchMain(argc, argv);
}

