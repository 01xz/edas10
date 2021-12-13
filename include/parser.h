#ifndef _PARSER_H
#define _PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "constants.h"

lf * get_lf(FILE * fp);
void * free_lf(lf * f);

#ifdef __cplusplus
}
#endif

#endif
