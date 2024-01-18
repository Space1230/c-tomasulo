#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getBoolString(bool boolean);
void optFree(void *item, bool shouldFree);

#endif // INCLUDES_H_
