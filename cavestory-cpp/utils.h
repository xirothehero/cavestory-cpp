#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <string>
#include <vector>

class Utils {

public:
	/* unsigned int split
	* Split a string <txt> everywhere a certain character <ch> is found
	* Store the resulting substrings in a vector <strs>
	* Return the size of the vector
	*/
	static unsigned int split(const std::string& txt, std::vector<std::string>& strs, char ch) {
		strs.clear();
		int initialPos = 0;
		int pos = txt.find(ch);
		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, (pos - initialPos + 1)));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

		return strs.size();
	}

};

#endif // !UTILS_H
