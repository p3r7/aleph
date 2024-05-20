// replacement lua `os` module to work w/ mock filesystem

#define loslib_c
#define LUA_LIB

// app
#include "files.h"


// ------------------------------------------------------------------------
// fs

static int os_remove (lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    return luaL_fileresult(L, fl_remove(filename) == 0, filename);
}

static int os_rename (lua_State *L) {
    const char *fromname = luaL_checkstring(L, 1);
    const char *toname = luaL_checkstring(L, 2);

    void* fp;
    u32 size = 0;
    volatile u8* content = NULL;

    app_pause();

    fp = list_open_file_name(&dspList, fromname, "r", &size);

    if ( (fp == NULL) || (size <= 0) ) {
        app_resume();
        return luaL_fileresult(L, false, NULL);
    }

    // RAM buffer malloc
    content = alloc_mem(size);
    fake_fread(content, size, fp);
    fl_fclose(fp);

    fp = fl_fopen(toname, "wb");
    fl_fwrite((const void*)content, size, 1, fp);
    fl_fclose(fp);

    // free
    free_mem(content);

    delay_ms(10);
    app_resume();

    // FIXME: only copying for now
    // will allow remove
    // return luaL_fileresult(L, fl_remove(filename) == 0, NULL);
    return luaL_fileresult(L, true, NULL);
}

#define LUA_TMPNAMBUFSIZE	L_tmpnam
#define lua_tmpnam(b,e)		{ e = (tmpnam(b) == NULL); }

static int os_tmpname (lua_State *L) {
    char buff[LUA_TMPNAMBUFSIZE];
    int err;
    lua_tmpnam(buff, err);
    if (err)
        return luaL_error(L, "unable to generate a unique filename");
    lua_pushstring(L, buff);
    return 1;
}

// ------------------------------------------------------------------------
// weaving

static const luaL_Reg syslib[] = {
    {"remove",    os_remove},
    // {"rename",    os_rename},
    {"tmpname",   os_tmpname},
    {NULL, NULL}
};
