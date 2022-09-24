#include "dotenv.h"
#include "tap.h"
#define _POSIX_C_SOURCE 200112L
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_env(void)
{
    int rc = dotenv_load("assets", 1);
    ok(!rc);

    is(getenv("USER"), "admin");
    is(getenv("HOST"), "localhost");
    is(getenv("PORT"), "8080");
    is(getenv("WELCOME"), "Hello, world!");
}

static void test_valid_env(void)
{
    int rc = dotenv_load("assets/test.valid.env", 1);
    ok(!rc);

    is(getenv("FOO1"), "foπo1#ffe");
    is(getenv("FOO2"), "foo2");
    is(getenv("FOO3"), "fo\\\"o3 ");
    is(getenv("FOO4"), "fπ\\oo4\\tfo\\\\o4");
    is(getenv("FOO5"), "foo5\\tfoo\\\\5");
    is(getenv("FOO6"), "6");
    is(getenv("FOO7"), "5.5f");
    is(getenv("FOO8"), "foo8");
    is(getenv("FOO9"), "foo9");
    is(getenv("EMPTY1"), "");
    is(getenv("EMPTY2"), "");
    is(getenv("EMPTY3"), "");
    is(getenv("USER"), "admin");
    is(getenv("EMAIL"), "${USER}@example.org");
    is(getenv("DATABASE_URL"), "postgres://${USER}@localhost/my_database");
    is(getenv("CACHE_DIR"), "${PWD}/cache");
}

static void test_no_file(void)
{
    int rc = dotenv_load("assets/no_file", 1);
    ok(rc == DOTENV_EIO);
}

int main(void)
{
    plan(23);
    test_env();
    test_valid_env();
    test_no_file();
    done_testing();
}
