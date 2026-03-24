#pragma once

class GameObject 
{
public:
	GameObject() = default;
	GameObject(Vec2 pos) : pos(pos) {}
	virtual ~GameObject() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	bool isActive = true;

protected:
	Vec2 pos;
};