#ifndef UTILS_H
#define UTILS_H

using namespace std;

#include <string>
#include <vector>

string slice(string input, int start, int end);
void fill_to(vector<int> &targetVector, int placeholder, int treshold);

#endif