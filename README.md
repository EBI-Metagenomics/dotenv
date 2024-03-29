# dotenv

C library that reads key-value pairs from a `.env` file.

## Description

You want to setup your program using either environment variables
and/or a very simple textual file like

```
USER=admin
HOST=localhost
PORT=8080
# Welcome message
WELCOME="Hello, world!"
```

This is the library your are looking for!

## Usage

Add the files `dotenv.c` and `dotenv.h` into your C/C++ project.

Suppose you have a `.env` like this one:

```
# .env file
HELLO="world!"
```

Running

```c
#include "dotenv.h"
#include <stdio.h>
// For getenv
#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
    dotenv_load(".", true);
    printf("Hello, %s\n", getenv("HELLO"));
    return 0;
}
```

should print

```
Hello, world!
```

## API

It consists in a single function:

```c
int dotenv_load(char const *path, bool override);
```

- `path` can be either a directory path, in which case it will try to read from
  file `.env` inside it, or a direct path to an actual file.
- `override` determine whether existing environment variables will be overwritten
  by the ones in the `.env` file.

The returning codes will be one of the following:

- `DOTENV_OK`: Success!
- `DOTENV_EIO`: I/O error.
- `DOTENV_EPARSE`: Failed to parse `.env` file.
- `DOTENV_ESETENV`: Failed to set environment variable.
- `DOTENV_ELONGPATH`: Path is too long.

## Author

- [Danilo Horta](https://github.com/horta)

## License

This project is licensed under the [MIT License](https://raw.githubusercontent.com/EBI-Metagenomics/dotenv/main/LICENSE).

## Acknowledgments

- [dotenv for c](https://github.com/Isty001/dotenv-c)
- [python-dotenv](https://github.com/theskumar/python-dotenv)
