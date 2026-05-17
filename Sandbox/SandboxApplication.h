#include "XTEngine2d.h"
#include <iostream>


class Sandbox : public XTEngine2d::Application
{
private:
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}

	void Init() override;
	void Run() override;
};
