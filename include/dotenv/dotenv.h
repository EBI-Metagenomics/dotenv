#ifndef DOTENV_H
#define DOTENV_H

#include "dotenv/export.h"
#include <stdbool.h>

typedef enum
{
    DOTENV_OK,
    DOTENV_EIO,
    DOTENV_EPARSE,
    DOTENV_ESETENV,
    DOTENV_ELONGPATH,
} dotenv_rc;

DOTENV_EXPORT dotenv_rc dotenv_load(char const *path, bool override_env);

#endif
