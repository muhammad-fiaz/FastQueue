/**
 * @file version.h
 * @brief FastQueue version information.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_VERSION_H
#define FQ_VERSION_H

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def FQ_VERSION_MAJOR
 * @brief Major version (breaking changes).
 */
#define FQ_VERSION_MAJOR 0

/**
 * @def FQ_VERSION_MINOR
 * @brief Minor version (new features, backwards compatible).
 */
#define FQ_VERSION_MINOR 1

/**
 * @def FQ_VERSION_PATCH
 * @brief Patch version (bug fixes).
 */
#define FQ_VERSION_PATCH 0

/**
 * @def FQ_VERSION_PRE
 * @brief Pre-release label (empty for stable releases).
 */
#define FQ_VERSION_PRE ""

/**
 * @def FQ_VERSION_STRING
 * @brief Human-readable version string.
 */
#define FQ_VERSION_STRING "0.1.0"

/**
 * @def FQ_VERSION_HEX
 * @brief Encoded version as a hex integer: 0xMMmmpp.
 */
#define FQ_VERSION_HEX \
    (((unsigned)FQ_VERSION_MAJOR << 16) | ((unsigned)FQ_VERSION_MINOR << 8) | (unsigned)FQ_VERSION_PATCH)

/**
 * @brief Retrieve the runtime version string.
 *
 * @return Pointer to a static string (e.g. "0.1.0").
 */
FQ_API const char *fq_version_string(void);

/**
 * @brief Retrieve the runtime version as a hex integer.
 *
 * @return Encoded version: 0xMMmmpp.
 */
FQ_API int fq_version_hex(void);

#ifdef __cplusplus
}
#endif

#endif /* FQ_VERSION_H */
