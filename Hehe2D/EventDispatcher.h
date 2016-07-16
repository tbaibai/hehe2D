#pragma once
#include <set>
using std::set;
class KeyboardDelegate
{
public:
    virtual void onKeyClicked(unsigned char key, int mouseX, int mouseY) = 0;
};


class EventDispatcher
{
public:
	EventDispatcher(void);
	~EventDispatcher(void);
	static EventDispatcher* instance();

	void dispatchKeyClicked(unsigned char key, int mouseX, int mouseY);
	void addKeyboradDelegate(KeyboardDelegate* delegate);
	void removeKeyboardDelegate(KeyboardDelegate* delegate);

private:
	set<KeyboardDelegate*> keyboardDelegates_;
};

