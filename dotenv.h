#ifndef DOTENV_H
#define DOTENV_H

enum dotenv_rc
{
    DOTENV_OK,
    DOTENV_EIO,
    DOTENV_EPARSE,
    DOTENV_ESETENV,
    DOTENV_ELONGPATH,
};

enum dotenv_rc dotenv_load(char const *path);

#endif
