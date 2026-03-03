#include "ScriptSystem.h"
#include <iostream>
#include "Scene.h"
#include "SceneNode.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

lua_State* ScriptSystem::L = nullptr;

Scene* ScriptSystem::sScene = nullptr;

std::vector<SceneNode*> ScriptSystem::sScriptedNodes;

void ScriptSystem::SetScene(Scene* scene) {
    sScene = scene;
}

static int l_Log(lua_State* L)
{
    const char* msg = lua_tostring(L, 1);
    std::cout << "[Lua] " << (msg ? msg : "(null)") << "\n";
    return 0;
}

static int l_Rotate(lua_State* L)
{
    const char* nodeName = lua_tostring(L, 1);
    float dRx = (float)luaL_checknumber(L, 2);
    float dRy = (float)luaL_checknumber(L, 3);
    float dRz = (float)luaL_checknumber(L, 4);

    if (!nodeName || !ScriptSystem::sScene) return 0;

    SceneNode* node = ScriptSystem::sScene->FindNodeByName(nodeName);

    if (!node) return 0;

    node->transform.rotation.x += dRx;
    node->transform.rotation.y += dRy;
    node->transform.rotation.z += dRz;

    return 0;
}

void ScriptSystem::BindAPI()
{
    lua_newtable(L);

    lua_pushcfunction(L, l_Log);
    lua_setfield(L, -2, "Log");

    lua_pushcfunction(L, l_Rotate);
    lua_setfield(L, -2, "Rotate");

    lua_setglobal(L, "engine");
}

void ScriptSystem::Init()
{
    L = luaL_newstate(); // creeaza virtual machineul Lua
    luaL_openlibs(L); // incarca librariile standard
    BindAPI(); // incarc API ul
}

void ScriptSystem::Shutdown()
{
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }
}

bool ScriptSystem::LoadScript(const std::string& path)
{
    if (!L) return false;
    if (luaL_dofile(L, path.c_str()) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        std::cerr << "[Lua] Error loading script: " << (err ? err : "(unknown)") << "\n";
        lua_pop(L, 1);
        return false;
    }
    return true;
}

void ScriptSystem::HandleEvent(const Event& e)
{
    if (!L) return;

    lua_getglobal(L, e.name.c_str());

    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
        std::cout << "[Lua] No handler for event: " << e.name << "\n";
        return;
    }
    

    int nargs = 0;
    for (const auto& a : e.args)
    {
        if (std::holds_alternative<int>(a))
            lua_pushinteger(L, std::get<int>(a));
        else if (std::holds_alternative<float>(a))
            lua_pushnumber(L, std::get<float>(a));
        else 
            lua_pushstring(L, std::get<std::string>(a).c_str());        
        nargs++;
    }

    if (lua_pcall(L, nargs, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        std::cerr << "[Lua] Error calling " << e.name << ": " << (err ? err : "(unknown)") << "\n";
        lua_pop(L, 1);
    }
}

int ScriptSystem::LoadScriptObject(const std::string& path)
{
    if (!L) return -1;


    if (luaL_loadfile(L, path.c_str()) != LUA_OK) {
        std::cerr << "[Lua] loadfile failed: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
        return -1;
    }


    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        std::cerr << "[Lua] pcall failed: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
        return -1;
    }

    if (!lua_istable(L, -1)) {
        std::cerr << "[Lua] Script must return a table: " << path << "\n";
        lua_pop(L, 1);
        return -1;
    }

    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return ref;
}

void ScriptSystem::AttachNodeScripts(SceneNode* root)
{
    if (!root) return;
    sScriptedNodes.clear();
    AttachNodeScriptsRecursive(root);
}

void ScriptSystem::AttachNodeScriptsRecursive(SceneNode* node)
{
    if (!node) return;

    if (!node->scriptPath.empty())
    {
        node->scriptRef = LoadScriptObject(node->scriptPath);
        if (node->scriptRef != -1)
        {
            sScriptedNodes.push_back(node);

            std::cout << "[Lua] Attached script " << node->scriptPath << " to node " << node->name << "\n";
        }
    }

    for (auto& c : node->children)
        AttachNodeScriptsRecursive(c.get());
}

void ScriptSystem::UpdateSingleNodeScript(SceneNode* node, float dt)
{
    if (!node || node->scriptRef == -1) return;

    lua_rawgeti(L, LUA_REGISTRYINDEX, node->scriptRef);
    lua_getfield(L, -1, "Update");

    if (lua_isfunction(L, -1))
    {
        lua_pushvalue(L, -2);
        lua_pushstring(L, node->name.c_str());
        lua_pushnumber(L, dt);

        if (lua_pcall(L, 3, 0, 0) != LUA_OK)
        {
            std::cerr << "[Lua] Update error: " << lua_tostring(L, -1) << "\n";
            lua_pop(L, 1);
        }
    }
    else
    {
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void ScriptSystem::UpdateAllScripts(float dt)
{
    for (SceneNode* node : sScriptedNodes)
        UpdateSingleNodeScript(node, dt);
}



