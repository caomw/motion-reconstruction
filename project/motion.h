#include "ANN\ANN.h"
#pragma once

class motion
{
public:
	motion(void);
	~motion(void);
	int nbframe;
	ANNpointArray * frames;
};

