#include "utils.h"

char* getBoolString(bool boolean) {
    if (boolean) {
        return "yes";
    }
    return "no";
}

void optFree(void *item, bool shouldFree) {
    if (shouldFree) { free(item); }
}
