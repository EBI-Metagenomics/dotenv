#ifndef DOTENV_H
#define DOTENV_H

#include "dotenv/export.h"
#include <stdbool.h>

enum dotenv_rc
{
    DOTENV_OK,
    DOTENV_EIO,
    DOTENV_EPARSE,
    DOTENV_ESETENV,
    DOTENV_ELONGPATH,
};

DOTENV_EXPORT enum dotenv_rc dotenv_load(char const *path, bool override);

#endif
