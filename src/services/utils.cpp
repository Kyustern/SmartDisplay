#include <string>
#include <vector>

using namespace std;

void fill_to(vector<int> &targetVector, int placeholder, int treshold)
{
  // size_t vectorSize = targetVector.size();
  if (targetVector.size() < treshold)
  {
    int missingElementsCount = treshold - targetVector.size();
    targetVector.insert(targetVector.begin(), placeholder);
    // for (int i = targetVector.size(); i < treshold + 1; i++)
    // // for (int i = 0; i < treshold; i++)
    // {
    //   targetVector.insert(targetVector.begin(), placeholder);
    //   // targetVector.push(placeholder);
    //   // targetVector
    // }
  }
}

string slice(string input, int start, int end)
{
  const char *cstr = input.c_str();
  char *final = "";
  int inputSize = sizeof(cstr);
  // if (start >= 0 && start <= inputSize || end > start && end <= inputSize)
  // {
  //   throw "Slice parameters are incorrect";
  // }

  int sliceLength = end - start;

  for (int i = 0; i < sliceLength + 1; i++)
  {
    final[i] = cstr[start + i];
  }
  // final[sliceLength] = '\n';
  string fStr = string(final);
  return string(fStr);
}
