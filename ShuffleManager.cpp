#include<vector>
#include<algorithm>
#include"Song.h"
class ShuffleManager {
    std::vector<Song> tempsong;

    ShuffleManager(const std::vector<Song>& playlist): tempsong(playlist){
        std::random_shuffle(tempsong.begin(),tempsong.end());
    }
};