#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

int n, base[2] = {4, 3};//default: 4,3
bool down[7], up[7];
map<int, string> key0, key1;
map<int, string> key0up, key0down, key1up, key1down;
string s;

string songName = "MelodyOfTheNight5_ShiJin";
ifstream fin("Raw_" + songName + ".txt");
ofstream fout(songName + ".txt");

void setUpDown()//change manually based on the song
{
    //down[1] = down[4] = down[0] = down[3] = down[6] = down[2] = down[5] = 1;
    down[1] = down[4] = down[0] = 1;
    //up[5] = up[2] = up[6] = up[3] = up[0] = up[4] = up[1] =  1;
    //up[5] = up[2] = up[6] = up[3] = up[0] = up[4] = 1;
}

void init()
{
    for (int i = 7; i >= 0; i--){
        key0[i * 5] += 'B' + i <= 'G' ? 'B' + i : 'B' + i - 7;
        key0down[i * 5] = key0[i * 5] + 'b';
        if (i == 1)
            key0down[i * 5] = "B";
        else if (i == 4)
            key0down[i * 5] = "E";
        if (i == 7 || i == 0)
            key0up[i * 5] = "C";
        else if (i == 3)
            key0up[i * 5] = "F";
        else
            key0up[i * 5] = key0down[i * 5 + 5];//down((i + 1) % 7)
    }
    for (int i = 7; i >= 0; i--){
        key1[i * 5] = 'D' + i <= 'G' ? 'D' + i : 'D' + i - 7;
        key1down[i * 5] = key1[i * 5] + 'b';
        if (i == 6)
            key1down[i * 5] = "B";
        else if (i == 2)
            key1down[i * 5] = "E";
        if (i == 7)
            key1up[i * 5] = "Eb";
        else if (i == 1)
            key1up[i * 5] = "F";
        else if (i == 5)
            key1up[i * 5] = "C";
        else
            key1up[i * 5] = key1down[i * 5 + 5];
        //if (down[(i + 3) % 7])
    }
}

bool isNum(char c)
{
    if (c == 'a' || c == 'u' || c == 'd' || c == 'n')
        return 1;
    if (c > '9' || c < '0')
        return 0;
    return 1;
}

inline void processInput(int& pos, int& num, int& changeCircle, bool& u, bool& d, bool& norm) {
    int cntChar = 0; bool sign = 1;
    while (isNum(s[pos]) && pos < s.length()) {
        if (s[pos] == 'a')
            sign = 0;
        else if (s[pos] == 'u')
            u = 1;
        else if (s[pos] == 'd')
            d = 1;
        else if (s[pos] == 'n')
            norm = 1;
        else
            num = num * 10 + s[pos] - '0', cntChar++;
        pos++;
    }
    if (num % 10 != 5 || (num == 5 && cntChar == 1))
        num *= 10;
    if (!sign) num = -num;

    while (num < 0)
        num += 35, changeCircle--;
    changeCircle += num / 35; num %= 35;
}

void runNote(int& pos)
{
    while (pos < s.length() && s[pos] != ','){//range = [-45,145]
            //translate first channel
            if (isNum(s[pos])){
                int num = 0; bool u = 0, d = 0, norm = 0;
                int changeCircle = 0;
                processInput(pos,num,changeCircle,u,d,norm);
                int t = (num / 5 + 1) %7;
                int quang = num == 0 ? base[0] + changeCircle - 1 : base[0] + changeCircle;
                if (!norm && (down[t] || d)) {
                    fout << key0down[num];
                    if (key0down[num] == "B")
                        quang--;
                }
                else if (!norm && (up[t] || u)) {
                    fout << key0up[num];
                    if (key0up[num] == "C")
                        quang++;
                }
                else
                    fout << key0[num];
                fout << quang;
            }
            else
                fout << s[pos], pos++;
        }
}

void runBass(int& pos)
{
    while (pos < s.length()){
            if (isNum(s[pos])){
                int num = 0; bool u = 0, d = 0, norm = 0;
                int changeCircle = 0;
                processInput(pos, num, changeCircle, u, d, norm);
                int t = (num / 5 + 3) %7;
                int quang = num < 30 ? base[1] + changeCircle - 1 : base[1] + changeCircle;
                if (!norm && (down[t] || d)){
                    fout << key1down[num];
                    if (key1down[num] == "B")
                        quang--;
                }
                else if (!norm && (up[t] || u)) {
                    fout << key1up[num];
                    if (key1up[num] == "C")
                        quang++;
                }
                else
                    fout << key1[num];
                fout << quang;
            }
            else
                fout << s[pos], pos++;
        }
}

int main()
{
    if (!fin.is_open()){
        cout << "file cannot open";
        return 0;
    }
    fin >> n; fout << n << '\n';
    setUpDown(); init();
    for (int i = 0; i < n; i++){
        fin >> s;
        int num = 0;
        int pos = 0;
        base[0] = 4; base[1] = 3;
        runNote(pos);
        //runBass(pos);
        fout << s[pos]; pos++;
        base[0] = 4; base[1] = 3;
        //runNote(pos);
        runBass(pos);
        fout << '\n';
    }
}
