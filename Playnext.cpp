#include"Playnext.h"

Song Playnext::getNextSong(){
            Song nextsong = nextSonglist.front();
            nextSonglist.pop();
            return nextsong;
        }