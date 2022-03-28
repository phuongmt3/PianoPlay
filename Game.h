#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 900;
const int GAME_WIDTH = 500;
const int GAME_HEIGHT = 900;

const SDL_Color colorList[] = {
    { 255, 255, 255, 255 }, 
    { 255, 13, 13, 255 },
    { 50, 50, 50, 255 },
    { 80, 80, 80, 255 },
    { 30, 35, 247, 255 },
    { 0, 0, 0, 0},
    { 0, 0, 0, 255},
    { 30, 35, 247, 180}
};
enum Color {
    white, red, darkGrey, lightGrey, blue, transparent, black, blueTranparent
};

class Global;
class Camera
{
public:
    double y = 8, speed = 1.5;
    bool stop;
    void update()
    {
        if (stop)
            y = 0;
        else y = 8;
    }
};

class Text
{
private:
    SDL_Color textColor = colorList[white];
    TTF_Font *gFont = nullptr;
    string text;
    SDL_Texture* texture;
public:
    SDL_Rect desR;
    Text();
    Text(const string& _text, int x, int y, int fontSize, Color textColor);
    void updateText(const string& newText);
    void updateFont(int fontSize);
    void updateColor(Color type);
    void updateTexture();
    void show();
    string takeText() {
        return text;
    }
};

class Block
{
private:
    string name;
    Color colorType;
public:
    Text content;
    SDL_Rect bloR;

    Block();
    Block(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,
          const string& _text, int x, int y, int fontSize, Color textColor);
    void setText(int fontType, Color colorType);
    void setColor(Color newColor);
    void show();
    void update();
    void changePos(int x, int y);
    string getName() {
        return name;
    }
};

class PopUp
{
private:
    Color colorType;
public:
    vector<Block> container;
    SDL_Rect desR;
    int limitMoveUp, limitMoveDown;

    PopUp(int x, int y, int w, int h);
    void addBlock(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,//block so voi Popup, text sv block
          const string& _text, int x, int y, int fontSize, Color textColor);
    void setColor(Color newColor);
    void update();
    void show();
    bool visibleBlock(int i);
    int takeY_BasePopUp(int i);
};

class AudioManager
{
public:
    static Mix_Chunk* notesList[14][8];
    static Mix_Chunk* winnerChunk;
    static void playNote(const string& note, int channel, int time)
    {
        if (note == "")
            return;
        Mix_Chunk* sound;
        if (note[1] >= '0' && note[1] <= '9')
            sound = notesList[note[0]-'A'][note[1]-'0'];
        else
            sound =  notesList[note[0]-'A'+7][note[2]-'0'];
        if (time == 0)
            Mix_PlayChannel(channel, sound, 0);
        else
            Mix_PlayChannelTimed(channel, sound, 0, time);
    }
    static void addNote(const string& note)
    {
        string s = "PianoPlay/pianoHub/piano-mp3/" + note + ".wav";
        if (note[1] >= '0' && note[1] <= '9')
            notesList[note[0]-'A'][note[1]-'0'] = Mix_LoadWAV(&s[0]);
        else
            notesList[note[0]-'A'+7][note[2]-'0'] = Mix_LoadWAV(&s[0]);
    }
};

class TextureManager
{
public:
    static SDL_Texture* takeTexture(const char* link);
    static void drawImage(SDL_Texture* tex, SDL_Rect src, SDL_Rect des);
};

class Tile
{
private:
    int w, h, pos; static Uint32 curTick;
    int channelCount = 4;
    bool touched = 0;
    int runNextTimeForChannel[8] = { 4,4,4,4,4,4,4,4 };//channelCount
public:
    SDL_Rect desR;
    string note[4][4];//~channelCount
    string bass[4][4];
    Tile(int width, int height, int stt, int prePos);
    void setNote(string _note, int channel, int consecutiveNotes, int isBass);
    int duration(int channel, int curpos, bool isNote);
    void playNote(int channel, bool isNote, int noteLength, int notePos);
    void show();
    void handleInput(int posInput, int& fail, PopUp& scoreTxt, PopUp& highScoreTxt,
                     PopUp& failPopUp);
    void update(int& fail, int gobackLength, PopUp& scoreTxt, PopUp& highScoreTxt,
                PopUp& failPopUp, int stt);
    int takePos(){
        return pos;
    }
    bool hadTouched(){
        return touched;
    }
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning, int& fail);
void render(int& fail);
void handleInput(bool& isRunning, int& fail);
void update(bool& isRunning, int& fail);
void clean();

struct Global//set a globle variable
{
    static SDL_Renderer* renderer;
    static int curTileID, tileCount, lastSeenID, score, highScore;
    static SDL_Texture *bg, *gameBg;
    static Camera camera;
    static const int waitingTimeForSecondNote;
    static SDL_Rect wrongRect;
    static bool showWrongKey;
};
#endif // GAME_H_INCLUDED
