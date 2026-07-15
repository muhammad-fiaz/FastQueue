---
title: Version API
description: FastQueue version information and compatibility API reference.
keywords: version, fq_version, compatibility
---

# Version API

Version information and compatibility checks.

## Constants

```c
FQ_VERSION_MAJOR   // e.g. 0
FQ_VERSION_MINOR   // e.g. 1
FQ_VERSION_PATCH   // e.g. 0
FQ_VERSION_STRING  // e.g. "0.1.0"
```

## Functions

```c
const char *fq_version_string(void);
```
Get version string at runtime.

```c
unsigned fq_version_major(void);
unsigned fq_version_minor(void);
unsigned fq_version_patch(void);
```
Get version components at runtime.

```c
unsigned fq_version_hex(void);
```
Get version as hex integer (e.g. `0x000100` for 0.1.0).

## Example

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

int main(void)
{
    printf("FastQueue v%s\n", fq_version_string());
    printf("Version: %u.%u.%u\n",
           fq_version_major(),
           fq_version_minor(),
           fq_version_patch());
    return 0;
}
```
