#pragma once

#include "../Model.h"
#include "../Application.h"

class Rasterizer {
public:
	// constructor
	Rasterizer() { lighting = true; }

	// methods
	void draw(const Model &model, const Application &app);

	bool lighting;
private:

};