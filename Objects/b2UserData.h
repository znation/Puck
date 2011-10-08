enum b2UserDataType
{
	b2UserData_Edge,
	b2UserData_Stick,
	b2UserData_Puck,
	b2UserData_Goal
};

class b2UserData
{
public:
	b2UserData(b2UserDataType t, void *e);
	b2UserDataType type;
	void *extraInfo;
};
