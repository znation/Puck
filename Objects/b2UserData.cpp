#include "pch.h"
#include "b2UserData.h"

b2UserData::b2UserData(b2UserDataType t, void *e)
{
	type = t;
	extraInfo = e;
}
