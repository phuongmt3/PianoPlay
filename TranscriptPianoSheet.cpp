#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

int n, base[2] = {4, 3};
bool down[7], up[7];//chua xu ly up
map<int, string> key0, key1;

void setUpDown()//change manually based on the song
{
    down[0] = down[1] = down[4] = 1;
}

void init()
{
    for (int i = 0; i < 7; i++){
        key0[i * 5] += 'B' + i <= 'G' ? 'B' + i : 'B' + i - 7;
        if (down[(i + 1) % 7])
            key0[i * 5] += 'b';
    }
    for (int i = 0; i < 7; i++){
        key1[i * 5] = 'D' + i <= 'G' ? 'D' + i : 'D' + i - 7;
        if (down[(i + 3) % 7])
            key1[i * 5] += 'b';
    }
}

bool isNum(char c)
{
    if (c == 'a')
        return 1;
    if (c > '9' || c < '0')
        return 0;
    return 1;
}

int main()
{
    string songName = "Test";
    ifstream fin("Raw_" + songName + ".txt");
    ofstream fout(songName + ".txt");
    if (!fin.is_open()){
        cout << "file cannot open";
        return 0;
    }
    fin >> n; fout << n << '\n';
    setUpDown(); init();
    for (int i = 0; i < n; i++){
        string s; fin >> s;
        int num = 0;
        int pos = 0;
        while (s[pos] != ',' && pos < s.length()){//range = [-45,145]
            //translate first channel
            if (isNum(s[pos])){
                num = 0; int cntChar = 0; bool sign = 1;
                while (isNum(s[pos]) && pos < s.length()){
                    if (s[pos] == 'a')
                        sign = 0;
                    else
                        num = num * 10 + s[pos] -'0', cntChar++;
                    pos++;
                }
                if (num % 10 != 5 || (num == 5 && cntChar == 1))
                    num *= 10;
                if (!sign) num = -num;

                int changeCircle = 0;
                while (num < 0)
                    num += 35, changeCircle--;
                changeCircle += num / 35;
                num %= 35;
                if (num == 0)
                    fout << key0[num] << base[0] + changeCircle - 1;
                else
                    fout << key0[num] << base[0] + changeCircle;
            }
            else
                fout << s[pos], pos++;
        }
        fout << s[pos]; pos++;
        while (pos < s.length()){
            if (isNum(s[pos])){
                num = 0; int cntChar = 0; bool sign = 1;
                while (isNum(s[pos]) && pos < s.length()){
                    if (s[pos] == 'a')
                        sign = 0;
                    else
                        num = num * 10 + s[pos] -'0', cntChar++;
                    pos++;
                }
                if (num % 10 != 5 || (num == 5 && cntChar == 1))
                    num *= 10;
                if (!sign) num = -num;

                int changeCircle = 0;
                while (num < 0)
                    num += 35, changeCircle--;
                changeCircle += num / 35;
                num %= 35;
                if (num < 30)
                    fout << key1[num] << base[1] + changeCircle - 1;
                else
                    fout << key1[num] << base[1] + changeCircle;
            }
            else
                fout << s[pos], pos++;
        }
        fout << '\n';
    }
}
