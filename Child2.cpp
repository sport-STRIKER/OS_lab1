#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) 
{
    string word = argv[1];
    for (int i = 0; i < word.length(); ++i)
        if (word[i] == ' ')
            word[i] = '_';    
    
    word += '\n';
    
    cout << word;

    return 0;
}