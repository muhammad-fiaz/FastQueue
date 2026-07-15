/**
 * @file version.c
 * @brief Version information.
 */

#include "fastqueue/version.h"

const char *fq_version_string(void)
{
    return FQ_VERSION_STRING;
}

int fq_version_hex(void)
{
    return FQ_VERSION_HEX;
}
