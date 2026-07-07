# C++ Style

This document defines the current C++ style for repository code.

## Formatting

- Use `clang-format` with the repository `.clang-format` file.
- Use spaces only. Tabs are not allowed for indentation.
- Use 4 spaces per indentation level.
- Use Allman braces.
- Keep lines at or below 120 columns unless a generated or external format requires otherwise.
- Run `scripts/format.ps1` to check formatting and `scripts/format.ps1 -Fix` to apply formatting.

## Naming

- Use namespaces for project and module organization.
- Use lower_case for namespaces, functions, variables, and private data members.
- Suffix private data members with `_`.
- Use PascalCase for ordinary classes and structs.
- Prefix abstract interfaces with `I`.
- Prefix enum type names with `E`.
- Prefix template type parameters with `T` when a short parameter name is appropriate.
- Name concepts with PascalCase, matching other type-like public constraints.
- Do not use Unreal `U`, `A`, or `F` prefixes for ordinary repository classes and structs.

## Types

- Use standard and native C++ types.
- Use `char`, `wchar_t`, and `char8_t` when those character types are the correct semantic fit.
- Use fixed-width standard integer types such as `std::uint8_t` and `std::int8_t` when fixed width is required.
- Do not use Unreal-style aliases such as `TCHAR`, `uint8`, or `int8`.

## Templates And Concepts

- Prefer C++ concepts for meaningful template constraints when the requirement is part of the API contract.
- Keep concepts small, readable, and close to the constrained template unless they are reused across modules.
- Do not add concepts for unconstrained implementation details or speculative future requirements.
- Prefer standard library concepts such as `std::movable` and `std::same_as` over custom traits when they express the requirement directly.

## Modules And Includes

- Prefer project C++ modules for public project APIs.
- Current `core` consumers import the module with:

```cpp
#include <standard-library-header>

import moldy.core;
```

- Include required standard library headers before importing project modules when the consumer also uses those standard library declarations. This avoids duplicate STL declaration issues on MSVC module builds.
- Do not use header units for required builds.
- Do not use `import std` in required builds yet. Normal includes in the global module fragment are the current approach for standard library declarations used by module units.

## Unreal-Inspired Boundaries

The style intentionally borrows readable layout and selected naming conventions from Unreal-style C++ without adopting Unreal Engine constructs.

Do not use Unreal reflection or property macros, including:

- `UCLASS`
- `UPROPERTY`
- `UFUNCTION`
- `GENERATED_BODY`

Do not add similar reflection/property macro systems without an explicit architecture decision.
