#include <iostream>
// #include <vector>
// #include <string>
// #include <algorithm>
// #include <cctype>
// #include <locale>
// #include <tuple>
// #include <iomanip>

#include "gstring.h"
#include "subtitle.h"

#include "main.h"

using namespace std;

using namespace gsubtitle;


int main(int argc, char const *argv[])
{
    /* code */
    cout << "Welcome to SUBTITLE\n";

    std::string fnameVTT{"The.Flash.s09e09.vtt"};
    std::string fnameSRT{"example.srt"};

    // menu
    cout << "Menu choices. \n";
    cout << "Convert VTT to SRT enter a: \n";
    cout << "Convert SRT to VTT enter b: \n";
    cout << "Modify VTT time c: \n";
    cout << "Modify SRT time d: \n";
    cout << "\nEnter menu: ";

    auto key = cin.get();

    switch (key)
    {

    case 'a':
        convertVTT2SRT(fnameVTT);
        break;
    case 'b':
        convertSRT2VTT(fnameSRT);
        break;
    case 'c':
        modifyVTTTime(fnameVTT);
        break;
    case 'd':
        modifySRTTime(fnameSRT);
        break;

    default:
        break;
    
    }

    return 0;
}
