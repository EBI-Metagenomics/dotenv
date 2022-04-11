#include "dotenv.h"
#define _POSIX_C_SOURCE 200112L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum rc
{
    OK,
    ENDFILE,
    E_IO,
    E_PARSE,
    E_SETENV,
};

enum limits
{
    LINE_SIZE = 2048,
};

static char line[LINE_SIZE] = {0};
static char *name = 0;
static char *value = 0;

static int next_line(FILE *fp)
{
    if (!fgets(line, LINE_SIZE - 1, fp))
    {
        if (feof(fp)) return ENDFILE;

        return E_IO;
    }

    size_t ln = strlen(line) - 1;
    if (line[ln] == '\n') line[ln] = 0;
    return OK;
}

static int expect_spaces_or_comment(char const *p)
{
    while (*p)
    {
        if (*p == '#') return OK;
        if (!isspace(*p)) return E_PARSE;
        ++p;
    }
    return OK;
}

static int parse_name(char *p, char **end)
{
    while (isspace(*p))
        ++p;

    if (isalpha(*p) || *p == '_')
    {
        name = p;

        ++p;
        while (isalnum(*p) || *p == '_')
            ++p;

        if (*p != '=') return E_PARSE;

        *p = 0;
        *end = p;
        return OK;
    }

    if (!*p || *p == '#')
    {
        *end = 0;
        return OK;
    }

    return E_PARSE;
}

static int parse_value(char *p)
{
    if (*p == '"')
    {
        ++p;
        value = p;

        int escape = *p == '\\';
        while (!(*p == '"' && !escape))
        {
            escape = *p == '\\';
            ++p;
        }

        if (!*p) return OK;

        *p = 0;
        return expect_spaces_or_comment(p + 1);
    }

    value = p;
    while (*p && !isspace(*p))
        ++p;

    if (!*p) return OK;

    *p = 0;
    return expect_spaces_or_comment(p + 1);
}

static int parse_file(FILE *fp)
{
    int rc = OK;
    while (!(rc = next_line(fp)))
    {
        char *p = 0;
        rc = parse_name(line, &p);
        if (rc) return rc;
        if (!p) continue;

        rc = parse_value(p + 1);
        if (rc) return rc;

        if (setenv(name, value, 1)) return E_SETENV;
    }

    return rc == ENDFILE ? OK : rc;
}

int dotenv_load(char const *path)
{
    FILE *fp = fopen(path, "r");

    int rc = parse_file(fp);

    fclose(fp);
    return rc;
}
