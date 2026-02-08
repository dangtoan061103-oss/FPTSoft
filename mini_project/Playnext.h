#include<queue>
#include"Song.h"
class Playnext{
    private:
        std::queue<Song> nextSonglist;
    public:
        Song getNextSong();
};