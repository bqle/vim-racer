#include <string>

using namespace std;
unsigned long hash(const string & str) {
	unsigned long hash = 5381;
	for (size_t i = 0 ; i < str.size(); ++i) 
		hash = 33 * hash + (unsigned char) str[i];
	return hash;
}

size_t hash3(size_t i) {
	size_t tmp = i;
	for (size_t i = 0 ; i < 3; ++i) {
		tmp = 33 * tmp + tmp % 17;
	}
	return tmp;
}