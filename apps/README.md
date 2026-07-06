# Applications

`apps` contains runnable executables that exercise repository code.

## `smoke`

`apps/smoke/main.cpp` builds the `smoke` executable. It links `project::core` and prints build information from the current core API.

Use this directory for small runnable entry points only when they are backed by source and CMake targets.
