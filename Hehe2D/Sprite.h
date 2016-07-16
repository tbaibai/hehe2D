#pragma once
#include "node.h"
NS_HEHE2D_BEGIN
class Sprite :
	public Node
{
public:
	Sprite(void);
	virtual ~Sprite(void);
	void setTextureFile(const char* filePath);
	explicit Sprite(const char* filePath);
	virtual void draw();
private:
	uint8* image_;
};

NS_HEHE2D_END