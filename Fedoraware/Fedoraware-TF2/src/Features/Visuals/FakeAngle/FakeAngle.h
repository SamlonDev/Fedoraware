#pragma once
#include "../../Feature.h"

#pragma warning ( disable : 4091 )

class CFakeAngle
{
public:
	void Run();

	matrix3x4 BoneMatrix[128];
	bool BonesSetup = false;

	bool DrawChams = false;
};

ADD_FEATURE(CFakeAngle, FakeAngle)