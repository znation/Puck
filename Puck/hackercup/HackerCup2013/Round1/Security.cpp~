#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <array>
#include <algorithm>
#include <vector>

using namespace std;

typedef unsigned long ulong;

string MakeKey(string k1, string k2, int k2offset, int length)
{
	string ret(k1);
	for (int i=0; i<length; i++)
	{
		if (k1[i] != '?')
		{
			// keep the char from k1
		}
		else if (k2[(i+k2offset)%length] != '?')
		{
			ret[i] = k2[(i+k2offset)%length];
		}
		else
		{
			// don't know! use an 'a'?
			ret[i] = 'a';
		}
	}
	return ret;
}

bool strSort(string x, string y)
{
	int commonLength = min(x.length(), y.length());
	for (int i=0; i<commonLength; i++)
	{
		if (x[i] < y[i])
		{
			return true;
		}
		if (x[i] > y[i])
		{
			return false;
		}
	}
	if (x.length() == commonLength)
	{
		return true;
	}
	return false;
}

int main(int argc, char **argv)
{
	ifstream input("Security_example_input.txt", ifstream::in);
	
	const int BUF_SIZE = 101;
	char buf[BUF_SIZE];

	// get input line count
	input.getline(buf, BUF_SIZE);
	int T = atoi(buf);

	for (short i=0; i<T; i++)
	{
		input.getline(buf, BUF_SIZE);
		int m = atoi(buf);
		
		input.getline(buf, BUF_SIZE);
		string k1(buf);

		input.getline(buf, BUF_SIZE);
		string k2(buf);

		int kl = k1.length();
		int l = kl / m;

		/*
		printf("DEBUG: k1 is %s, k2 is %s, m is %d, kl is %d\n",
				k1.data(),
				k2.data(),
				m,
				kl);
		*/

		vector<string> possibleKeys;

		for (int j=0; j<m; j++)
		{
			int offset = j * l;
			bool maybeMatch = true;

			for (int k=0; k<kl; k++)
			{
				int k1idx = k;
				int k2idx = (k + offset) % kl;

				//printf("DEBUG: comparing %c to %c -- ", k1[k1idx], k2[k2idx]);

				if (k1[k1idx] == '?' ||
					k2[k2idx] == '?' ||
					k1[k1idx] == k2[k2idx])
				{
					// might be a match
					//printf("maybe...\n");
				}
				else
				{
					//printf("NO!\n");
					maybeMatch = false;
					break;
				}
			}
			if (maybeMatch)
			{
				possibleKeys.push_back(MakeKey(k1, k2, offset, kl));
			}
		}

		std::sort(possibleKeys.begin(), possibleKeys.end(), strSort);
		if (possibleKeys.size() > 0)
		{
			cout << "Case #" << (i+1) << ": " << possibleKeys[0].data() << endl;
		}
		else
		{
			cout << "Case #" << (i+1) << ": " << "IMPOSSIBLE" << endl;
		}
	}
}
