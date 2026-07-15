# Contributing to FastQueue

Thank you for your interest in contributing! This document provides guidelines for contributing to FastQueue.

## Getting Started

1. Fork the repository
2. Clone your fork
3. Create a feature branch
4. Make your changes
5. Submit a pull request

## Development Setup

```bash
git clone https://github.com/youruser/fastqueue.git
cd fastqueue
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DFQ_BUILD_TESTS=ON -DFQ_BUILD_EXAMPLES=ON
cmake --build build
ctest --test-dir build
```

## Code Style

- **C23 standard** — Use modern C features
- **Naming** — `fq_<module>_<verb>()` for functions, `fq_<name>_t` for types
- **Error handling** — Return `fq_status_t`, never use `assert` for public API
- **Documentation** — Doxygen comments on every public symbol
- **Formatting** — 4-space indentation, no tabs
- **Line length** — Aim for 100 characters max

## Commit Messages

Use clear, descriptive commit messages:

```
module: brief description

Detailed explanation of the change if needed.
```

Examples:
- `queue: fix race condition in MPMC pop`
- `scheduler: add work-stealing support`
- `docs: update API reference for futures`

## Testing

- All changes must include tests
- Run the full test suite before submitting
- Sanitiser builds must pass (ASAN, UBSAN, TSAN)

## Pull Requests

- One feature/fix per PR
- Include a description of the change
- Reference any related issues
- Ensure CI passes
- Request review from maintainers

## Reporting Issues

Use GitHub Issues with:
- Clear title and description
- Minimal reproducible example
- Compiler/platform information
- Expected vs actual behavior

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
