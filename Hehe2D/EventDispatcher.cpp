#include "EventDispatcher.h"

EventDispatcher::EventDispatcher(void)
{
}

EventDispatcher::~EventDispatcher(void)
{
}

EventDispatcher* EventDispatcher::instance()
{
    static EventDispatcher instance;
    return &instance;
}

void EventDispatcher::dispatchKeyClicked(unsigned char key, int mouseX, int mouseY)
{
    for(set<KeyboardDelegate*>::iterator iter = keyboardDelegates_.begin(); iter != keyboardDelegates_.end(); ++iter)
    {
        if(*iter)
        {
            (*iter)->onKeyClicked(key, mouseX, mouseY);
        }
    }
}

void EventDispatcher::addKeyboradDelegate( KeyboardDelegate* delegate )
{
    if(delegate)
    {
        keyboardDelegates_.insert(delegate);
    }
}

void EventDispatcher::removeKeyboardDelegate( KeyboardDelegate* delegate )
{
    if(delegate)
    {
        keyboardDelegates_.erase(delegate);
    }
}
