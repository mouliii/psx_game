#pragma once

#include "EASTL/vector.h"
#include "EASTL/string_view.h"

#include "core_systems/graphics.h"
#include "core_systems/gamepad.h"

class MenuObject;
enum MenuAction {NONE, POP, PUSH, ACTION};

struct MenuItem
{
    eastl::string_view name;
    eastl::function<void()> callBack;
    MenuObject* childMenu = nullptr;
};

class MenuObject
{
public:
    MenuObject(eastl::string_view menuTitle);
    MenuAction Update(const psyqo::AdvancedPad::Event& event);
    void Draw();
    void AddItem(eastl::string_view name, eastl::function<void()> callback, MenuObject* childMenu);
    void RemoveItem();
    const psyqo::Vec2 GetItemPosition(int index);
    const psyqo::Vec2 GetSelectedPosition();
    MenuObject* GetSelectedMenu(){return items[menuIndex].childMenu;};
    MenuItem& GetSelectedMenuItem(){return items[menuIndex];};
public:
    MenuObject* parentMenu = nullptr;
    eastl::string_view menuTitle;
    psyqo::Vec2 pos;
    eastl::vector<MenuItem> items;
    int16_t menuIndex = 0;
    uint16_t rowGap = 16;
};
