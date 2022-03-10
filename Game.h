#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include "SDL_image.h"
#include "SDL_mixer.h"
using namespace std;//currently resources is not sync, change link variable

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

class Camera
{
public:
    int y = 8, speed = 0;
    void update()
    {
        y += speed;
    }
};

class AudioManager
{
public:
    static Mix_Chunk* notesList[14][8];//can improve to [14][8]
    static Mix_Chunk* winnerChunk;
    static void playNote(const string& note, int channel, int time)
    {
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
public:
    Tile(int width, int height, int stt, int prePos);
    void setNote(string _note, int channel, bool isSecond, int isBass);
    void show();
    void handleInput(int posInput, bool& isRunning);
    void update(bool& isRunning);
    int takePos(){
        return pos;
    }
    string takeNote(int i, int j){
        return note[i][j];
    }
private:
    int w, h, pos, waitingTimeForSecondNote = 250;
    string note[3][2];
    string bass[3][2];
    SDL_Rect srcR, desR;
    bool touched, runSecondTimeForChannel[6];
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning);
void render();
void handleInput(bool& isRunning);
void update(bool& isRunning);
void clean();

class Global
{
public:
    static SDL_Renderer* renderer;
    static int curTileID, tileCount;
    static SDL_Texture *click, *unclick, *bg;
    static Camera camera;
};
#endif // GAME_H_INCLUDED
