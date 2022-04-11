#include "dotenv.h"
#define _POSIX_C_SOURCE 200112L
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSETS "assets"

static void die(char const *restrict fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);
    exit(EXIT_FAILURE);
}

static void expect(char const *name, char const *expected_value)
{
    char *value = getenv(name);
    if (!value) die("Unexpected `NULL` value.");

    if (strcmp(value, expected_value))
        die("Expected `%s` but got `%s`.", expected_value, value);
}

static void test_env(void)
{
    int rc = dotenv_load(ASSETS);
    if (rc) die("Failed with code %d.", rc);
}

static void test_valid_env(void)
{
    int rc = dotenv_load(ASSETS "/test.valid.env");
    if (rc) die("Failed with code %d.", rc);

    expect("FOO1", "foπo1#ffe");
    expect("FOO2", "foo2");
    expect("FOO3", "fo\\\"o3 ");
    expect("FOO4", "fπ\\oo4\\tfo\\\\o4");
    expect("FOO5", "foo5\\tfoo\\\\5");
    expect("FOO6", "6");
    expect("FOO7", "5.5f");
    expect("FOO8", "foo8");
    expect("FOO9", "foo9");
    expect("EMPTY1", "");
    expect("EMPTY2", "");
    expect("EMPTY3", "");
    expect("USER", "admin");
    expect("EMAIL", "${USER}@example.org");
    expect("DATABASE_URL", "postgres://${USER}@localhost/my_database");
    expect("CACHE_DIR", "${PWD}/cache");
}

int main(void)
{
    test_env();
    test_valid_env();
    printf("All tests have passed!\n");
    return EXIT_SUCCESS;
}
