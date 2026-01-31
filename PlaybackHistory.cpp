#include<stack>
#include"Song.h"
class PlaybackHistory {
    public:
        std::stack<Song> playedSong;

        Song playPreviousSong(){
            Song oldsong = playedSong.top();
            playedSong.pop();
            return oldsong;
        }
};