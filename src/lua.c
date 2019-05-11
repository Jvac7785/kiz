bool check_lua(lua_State *L, int r)
{
    if(r != LUA_OK)
    {
        const char *errormsg = lua_tostring(L, -1);
        log_error("%s\n", errormsg);
        return false;
    }
    return true;
}

void test_lua(lua_State *L)
{
    if(check_lua(L, luaL_dofile(L, "./scripts/test.lua")))
    {
        lua_getglobal(L, "AddStuff");
        if(lua_isfunction(L, -1))
        {
            lua_pushnumber(L, 3.5f);
            lua_pushnumber(L, 4.5f);

            if(check_lua(L, lua_pcall(L, 2, 1, 0)))
            {
                log_info("%f\n", (float)lua_tonumber(L, -1));
            }
        }
    }
}
