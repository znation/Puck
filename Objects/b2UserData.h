enum b2UserDataType
{
	b2UserData_Edge
};

class b2UserData
{
public:
	b2UserData(b2UserDataType t, void *e);
	b2UserDataType type;
	void *extraInfo;
};
