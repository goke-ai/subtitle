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

void test(std::string &fnameVTT, std::string &fnameSRT)
{
    gcore::rtrim_chars(fnameVTT);
    cout << fnameVTT << endl;
    gcore::ltrim_chars(fnameVTT);
    cout << fnameVTT << endl;

    gcore::rtrim(fnameSRT);
    cout << fnameSRT << endl;
    gcore::ltrim(fnameSRT);
    cout << fnameSRT << endl;
}

int main(int argc, char const *argv[])
{
    /* code */
    cout << "Welcome to SUBTITLE\n";

    std::string fnameVTT{"   The.Flash.s09e09.vtt   "};
    std::string fnameSRT{"   example.srt    "};

    test(fnameVTT, fnameSRT);

    std::string folder_path{"vtts"};
    double time{0.0};

    while (true)
    {

        // menu
        cout << "\nMenu choices. \n";
        cout << "Convert VTT to SRT enter a: \n";
        cout << "Convert SRT to VTT enter b: \n";
        cout << "Modify VTT time c: \n";
        cout << "Modify SRT time d: \n";
        cout << "Convert all VTT to SRT enter e: \n";
        cout << "Convert all SRT to VTT enter f: \n";
        cout << "Quit enter q: \n";
        cout << "\nEnter menu: ";

        auto key = cin.get();
        cin.ignore(1000, '\n');

        switch (key)
        {
        case 'a':
            cout << "Enter VTT file name: ";
            getline(cin, fnameVTT);
            convertVTT2SRT(fnameVTT);
            break;
        case 'b':
            cout << "Enter SRT file name: ";
            getline(cin, fnameSRT);
            convertSRT2VTT(fnameSRT);
            break;
        case 'c':
            cout << "Enter VTT file name: ";
            getline(cin, fnameVTT);
            cout << "Enter time to modify: ";
            cin >> time;
            modifyVTTTime(fnameVTT, time);
            break;
        case 'd':
            cout << "Enter SRT file name: ";
            getline(cin, fnameSRT);
            cout << "Enter time to modify: ";
            cin >> time;
            modifySRTTime(fnameSRT, 10.0);
            break;
        case 'e':
            cout << "Enter VTT folder path: ";
            getline(cin, folder_path);
            convertAllVTT2SRT(folder_path);
            break;
        case 'f':
            cout << "Enter SRT folder path: ";
            getline(cin, folder_path);
            convertAllSRT2VTT(folder_path);
            break;

        default:
            break;
        }

        if (key == 'q')
            break;
    }

    return 0;
}
