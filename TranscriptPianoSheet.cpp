#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

int n, base[2] = {4, 3};
bool down[7], up[7];//chua xu ly up
map<int, string> key0, key1;
map<int, string> key0up, key0down, key1up, key1down;

void setUpDown()//change manually based on the song
{
    //down[0] = down[1] =
    //down[1] = 1;
    up[2] = up[5] = up[0] = up[3] = up[6] = 1;
}

void init()
{
    for (int i = 7; i >= 0; i--){
        key0[i * 5] += 'B' + i <= 'G' ? 'B' + i : 'B' + i - 7;
        key0down[i * 5] = key0[i * 5] + 'b';
        if (i == 7)
            key0up[i * 5] = "Cb";
        else
            key0up[i * 5] = key0down[i * 5 + 5];//down((i + 1) % 7)
    }
    for (int i = 7; i >= 0; i--){
        key1[i * 5] = 'D' + i <= 'G' ? 'D' + i : 'D' + i - 7;
        key1down[i * 5] = key1[i * 5] + 'b';
        if (i == 7)
            key1up[i * 5] = "Cb";
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

int main()
{
    string songName = "YoruNiKakeru_Yoasobi";
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
                num = 0; int cntChar = 0; bool sign = 1, u = 0, d = 0, norm = 0;
                while (isNum(s[pos]) && pos < s.length()){
                    if (s[pos] == 'a')
                        sign = 0;
                    else if (s[pos] == 'u')
                        u = 1;
                    else if (s[pos] == 'd')
                        d = 1;
                    else if (s[pos] == 'n')
                        norm = 1;
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
                changeCircle += num / 35; num %= 35;
                int t = (num / 5 + 1) %7;
                if (!norm && (down[t] || d))
                    fout << key0down[num];
                else if (!norm && (up[t] || u))
                    fout << key0up[num];
                else
                    fout << key0[num];
                if (num == 0)
                    fout << base[0] + changeCircle - 1;
                else
                    fout << base[0] + changeCircle;
            }
            else
                fout << s[pos], pos++;
        }
        fout << s[pos]; pos++;
        while (pos < s.length()){
            if (isNum(s[pos])){
                num = 0; int cntChar = 0; bool sign = 1, u = 0, d = 0, norm = 0;
                while (isNum(s[pos]) && pos < s.length()){
                    if (s[pos] == 'a')
                        sign = 0;
                    else if (s[pos] == 'u')
                        u = 1;
                    else if (s[pos] == 'd')
                        d = 1;
                    else if (s[pos] == 'n')
                        norm = 1;
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
                changeCircle += num / 35; num %= 35;
                int t = (num / 5 + 3) %7;
                if (!norm && (down[t] || d))
                    fout << key1down[num];
                else if (!norm && (up[t] || u))
                    fout << key1up[num];
                else
                    fout << key1[num];
                if (num < 30)
                    fout << base[1] + changeCircle - 1;
                else
                    fout << base[1] + changeCircle;
            }
            else
                fout << s[pos], pos++;
        }
        fout << '\n';
    }
}
