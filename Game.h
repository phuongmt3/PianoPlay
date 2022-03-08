#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <fstream>
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
    static Mix_Chunk* notesList[7][8];//can improve to [14][8]
    static Mix_Chunk* winnerChunk;
    static void playNote(string note, int channel)
    {
        Mix_PlayChannel(channel, notesList[note[0]-'A'][note[1]-'0'], 0);
    }
    static void addNote(string note)
    {
        string s = "pianoHub/piano-mp3/" + note + ".wav";
        notesList[note[0]-'A'][note[1]-'0'] =
        Mix_LoadWAV(&s[0]);
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
    Tile(int width, int height, int stt, int prePos, string _note);
    void show();
    void handleInput(int posInput, bool& isRunning);
    void update(bool& isRunning);
    int takePos(){
        return pos;
    }
private:
    int w, h, pos;
    string note;
    SDL_Rect srcR, desR;
    bool touched = 0;
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
