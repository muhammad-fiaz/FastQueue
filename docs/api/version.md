# Version API

Header: `<fastqueue/version.h>`

## Macros

| Macro | Value | Description |
|---|---|---|
| `FQ_VERSION_MAJOR` | 0 | Major version |
| `FQ_VERSION_MINOR` | 1 | Minor version |
| `FQ_VERSION_PATCH` | 0 | Patch version |
| `FQ_VERSION_STRING` | "0.1.0" | Human-readable version |
| `FQ_VERSION_HEX` | 0x000100 | Encoded version |

## Functions

### fq_version_string

```c
const char *fq_version_string(void);
```

Returns the runtime version string.

### fq_version_hex

```c
int fq_version_hex(void);
```

Returns the encoded version as `0xMMmmpp`.
