#include "Exports.h"
#include <cstring>

int main(int argc, char *argv[])
{
    conchSetHandleMessageCallback([](const char *eventName, const char *data) -> void {},
                                  [](const char *eventName, const char *data) -> void {});

    return conchMain(argc, argv);
}
