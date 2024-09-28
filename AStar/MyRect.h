#pragma once

class MyRect
{
private:
	double x;
	double y;
	
	double width;
	double height;
	
	double left;
	double right;
	double top;
	double bottom;

public:
	MyRect();
	~MyRect();

	void Init();
	void Update();

	void Draw();
};

