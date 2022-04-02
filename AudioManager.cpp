#include "Game.h"

void AudioManager::playNote(const string& note, int channel, int time)
{
    if (note == "")
        return;
    Mix_Chunk* sound;
    if (note[1] >= '0' && note[1] <= '9')
        sound = notesList[note[0] - 'A'][note[1] - '0'];
    else
        sound = notesList[note[0] - 'A' + 7][note[2] - '0'];
    if (time == 0)
        Mix_PlayChannel(channel, sound, 0);
    else
        Mix_PlayChannelTimed(channel, sound, 0, time);
}

void AudioManager::addNote(const string& note)
{
    string s = "PianoPlay/pianoHub/piano-mp3/" + note + ".wav";
    if (note[1] >= '0' && note[1] <= '9')
        notesList[note[0] - 'A'][note[1] - '0'] = Mix_LoadWAV(&s[0]);
    else
        notesList[note[0] - 'A' + 7][note[2] - '0'] = Mix_LoadWAV(&s[0]);
}