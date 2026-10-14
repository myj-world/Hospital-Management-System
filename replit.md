# Running on Replit

This is a C console program, not a web application.

Press **Run** to compile `main.c` and execute it in the Console. The configured
workflow uses the installed C toolchain:

```sh
mkdir -p build && clang -std=c11 -Wall -Wextra -Wpedantic main.c -o build/hospital && ./build/hospital
```

The current program prints `Hello, World!` and exits successfully. It defines
hospital record types, but the menus and features described in `README.md` are
not implemented yet.

No secrets, external services, or third-party libraries are required.
`test.c` is a separate standalone program with its own `main`; do not compile it
together with `main.c`. Generated binaries are kept in the ignored `build/`
directory.
