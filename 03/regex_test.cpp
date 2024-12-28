//
// Created by fabian on 12/3/24.
//


#include <bits/stdc++.h>
using namespace std;
int main()
{
    string sp("geeksforgeeks");
    regex re("(geeks)(.*)");

    // flag type for determining the matching behavior
    // && here it is for matches on strings.
    smatch match;

    if(std::regex_match(sp, match, re)) {
        for (auto const &i : match) {
            std::cout << i << std::endl;
        }
    }
    return 0;
}
