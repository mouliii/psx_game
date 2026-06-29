#include "menu_object.h"

MenuObject::MenuObject(eastl::string_view menuTitle)
    : menuTitle(menuTitle)
{
}

MenuAction MenuObject::Update(const psyqo::AdvancedPad::Event &event)
{
    if (event.type == psyqo::AdvancedPad::Event::ButtonPressed)
    {
        if (event.button == psyqo::AdvancedPad::Cross)
        {
            MenuItem& item = items[menuIndex];
            if (item.childMenu)
            {
                return MenuAction::PUSH;
            }
            if (item.callBack)
            {
                item.callBack();
            }
            
            return MenuAction::ACTION;
            
        }
        else if (event.button == psyqo::AdvancedPad::Triangle)
        {
            return MenuAction::POP;
        }
        if (event.button == psyqo::AdvancedPad::Up)
        {
            if(--menuIndex < 0)
            {
                menuIndex = items.size() - 1;
            }
        }
        else if (event.button == psyqo::AdvancedPad::Down)
        {
            if(++menuIndex >= items.size())
            {
                menuIndex = 0;
            }
        }
    }
    
    return MenuAction::NONE;
}

void MenuObject::Draw()
{
    for (size_t i = 0; i < items.size(); i++)
    {
        Graphics::Instance().DrawText(GetItemPosition(i), {255,255,255}, items[i].name.begin());
    }
}

void MenuObject::AddItem(eastl::string_view name, eastl::function<void()> callback, MenuObject* childMenu)
{
    items.emplace_back(MenuItem{name, eastl::move(callback), childMenu});
}

// NYI
void MenuObject::RemoveItem()
{
}

const psyqo::Vec2 MenuObject::GetItemPosition(int index)
{
    return psyqo::Vec2(pos.x, pos.y + index * rowGap);
}

const psyqo::Vec2 MenuObject::GetSelectedPosition()
{
    return psyqo::Vec2(pos.x, pos.y + menuIndex * rowGap);
}
