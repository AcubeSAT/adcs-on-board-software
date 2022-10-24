#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <Albedo.hpp>
#include <vector>
#include <iterator>
#include <filesystem>
using namespace albedo;
using namespace std;
// Adapted from https://github.com/jefftrull/json_spirit/blob/develop/fuzzing/fuzz_onecase.cpp



// int fuzz(std::string const &s) {
//     for (auto s_begin = s.begin(); s_begin != s.end(); ++s_begin) {
//         if (*s_begin & 0x80) {
//             // Non-ASCII characters.
//             abort();
//         }
//         if (*s_begin == '"') {
//             // A string begins here.
//             auto s_current = s_begin + 1;
//             while ((s_current != s.end()) && (*s_current != '"')) {
//                 if (*s_current == 'f') {
//                     abort();
//                 }
//                 s_current++;
//             }
//             if (s_current == s.end()) {
//                 // We arrived at the end of the string without finding a terminating double quote.
//                 abort();
//             }
//             s_begin = s_current; // Move forward.
//         }
//     }
//     return 0;
// }

vector<string> split(const string& s)
{
	vector<string> ret;
	typedef string::size_type string_size;
	string_size i = 0;

    //invariant: we have processed characters [original value of i, i)
	while (i != s.size()){
		// ignore leading blanks
		// invariant: characters in range [original i, current i)
	while (i != s.size() && isspace(s[i]))
		++i;

	// find end of next word
	string_size j = i;
	// invariant: none of the characters in range [original j, current j)
	while (j != s.size() && !isspace(s[j]))
		j++;
		// if we found some nonwhitespace characters
		if(i != j){
			//copy from s starting at i and taking j-i chars
			ret.push_back(s.substr(i, j - i));
			i = j;
		}
	}

    std::vector<int> argument_list;

    std::transform(ret.begin(),ret.end(),std::back_inserter(argument_list),[&](std::string s){return stoi(s);});
	calculateCellArea(argument_list[0],argument_list[1]);


	return ret;
}


int main() {

// Deffered initialization not needed for now.
// See https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.persistent_mode.md#3-deferred-initialization
// for more.

// #ifdef __AFL_HAVE_MANUAL_CONTROL
//   __AFL_INIT();
// #endif

    // Persistent mode also not needed.
    // See https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.persistent_mode.md#4-persistent-mode
    // for more.

    // If using persistent mode, we can also take advantage of shared memory fuzzing.
    // See https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.persistent_mode.md#5-shared-memory-fuzzing
    // for more.
    // Note that modifications for this to work with C++ will be required.

    // while (__AFL_LOOP(1000)) {
    // std::string s = std::string(
    //     std::istreambuf_iterator(std::cin), {});   
    // split(s);

    string s;
    getline(cin, s);
    vector<string> v = split(s);

    return 0;
}
