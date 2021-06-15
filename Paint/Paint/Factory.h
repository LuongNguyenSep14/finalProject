#pragma once
#include "Utility.h"

class Factory
{
private:
	static Factory* _instance;
	vector<shared_ptr<Object>> _prototype;

	Factory()
	{
		_prototype.push_back(make_shared<Rect>());
		_prototype.push_back(make_shared<Ellipses>());
		_prototype.push_back(make_shared<Line>());
	}


public:

	shared_ptr<Object> create(int type)
	{
		for (int i = 0; i < totalShapes(); i++)
			if (_prototype[i]->getID() == type)
			{
				shared_ptr<Object> shape = _prototype[i]->nextObject();
				return shape;
			}

		return nullptr;
	}

	int totalShapes()
	{
		return _prototype.size();
	}


	static Factory* instance()
	{
		if (_instance == nullptr)
			_instance = new Factory();

		return _instance;
	}


};

Factory* Factory::_instance = nullptr;
