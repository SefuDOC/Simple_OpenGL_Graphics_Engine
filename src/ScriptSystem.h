#pragma once
#include <vector>
#include <string>
#include "EventBus.h"
#include "SceneNode.h"
#include "Scene.h"
struct lua_State;

class ScriptSystem
{
public:
    static void SetScene(Scene* scene);
    static void Init();
    static void BindAPI();
    static void Shutdown();

    static void HandleEvent(const Event& e);
    
    static bool LoadScript(const std::string& path);
    static void AttachNodeScripts(SceneNode* root);
    static void UpdateAllScripts(float dt);

    static Scene* sScene;
    static lua_State* L;
private:
    
    static std::vector<SceneNode*> sScriptedNodes;
    static void UpdateSingleNodeScript(SceneNode* node, float dt);
    static int LoadScriptObject(const std::string& path);
    static void AttachNodeScriptsRecursive(SceneNode* node);

    
};