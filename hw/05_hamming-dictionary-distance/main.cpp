#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
   // Load the input
   int dictionary_size, minumum_dictionary_word_size;
   cin >> dictionary_size >> minumum_dictionary_word_size;
   vector<string> dictionary(dictionary_size);
   for (int i = 0; i < dictionary_size; ++i) {
      cin >> dictionary[i];
   }
   string text;
   cin >> text;

   return 0;
}
