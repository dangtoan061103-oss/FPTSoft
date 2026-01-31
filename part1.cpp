#include<iostream>
#include<memory>    // for std::unique_ptr
#include<string>
#include<vector>    // for std::vector
#include<list>      // for std::list
#include<unordered_map> // for std::unordered_map
#include<map> // fot std::map
// Song struct
#include"Song.h"

// 1.Music library class    
class MusicLibrary {
    public:
        std::vector<Song> songs;
        std::unordered_map<int, Song*>songIndexByID;
        std::map<std::string, Song*> songIndexByTitle;
        std::unordered_map<std::string, std::vector<Song*>> songsIndexByartist;

        Song* findSongByID(int id){
            //return songIndexByID.at(id);
            auto i = songIndexByID.find(id);
            if(i != songIndexByID.end()){
                return i ->second;
            }
            return nullptr;
        }

        // Part 2.2 Find Song by title
        Song* findSongByTitle(const std::string& title){
            auto i = songIndexByTitle.find(title);
            if(i != songIndexByTitle.end()){
                return i->second;
            }
            return nullptr;
        }

        std::vector<Song*> findSongByArtist(const std::string& artist){
            auto i = songsIndexByartist.find(artist);
            if(i != songsIndexByartist.end()){
                std::cout<<"find songs of "<< artist <<"\n";
                for(auto s : i->second){
                    std::cout<<s->title<<"\n";
                }
                return i->second;
            }
        }
};

// 2. PlaybackQueue Class
class PlaybackQueue{
    public:
        std::list<Song> currentsong;
        void addSong(const Song& song){
            currentsong.push_back(song);
        }

        void removeSong(int songID){
            for(auto it = currentsong.begin(); it != currentsong.end();++it){
                if(it->id == songID){
                    currentsong.erase(it);
                }
            }
        }
        
        Song& getCurrentSong(){
            if(!currentsong.empty()){
                return currentsong.front();
            }
        }

        void playNext(){
            if(!currentsong.empty()){
                currentsong.pop_front();
            }
        }

        void display(){
            for(auto &i : currentsong){
                if(!currentsong.empty()){
                    std::cout<<i.id<<", "<<i.title<<", "<<i.artist<<", "
                             <<i.album<<", "<<i.duration<<"\n";
                }
            }
        }
};

// Add album to queue
void addAlbumToQueue(const std::string& albumName, const MusicLibrary& library, PlaybackQueue& queue){
    for(auto &it : library.songs){
        if(it.album == albumName){
            std::cout<<"Add the song has ID: "<<it.id<<" to queue\n";
            queue.addSong(it);
        }
    }
}

//testgit
int main(){
    MusicLibrary lib;
    lib.songs ={
        {1,"A","Jack","Album 1",30},
        {2,"B","MTP","Album 2",15},
        {3,"C","Jack","Album 2",12},
        {4,"D","DenVau","Album 3",20},
        {5,"E","Toan","Album 2",15}
    };
    PlaybackQueue queue;
    addAlbumToQueue("Album 2",lib,queue);
    std::cout<<"List Song in Queue:\n";
    queue.display();
}

// 1. Justification for Using std::vector to store songs
// std::vector is an appropriate container for storing the music library because it matches the system’s
// usage pattern and performance requirements.

// First, the music library requires fast random access by index (e.g., selecting or iterating through songs).
// std::vector provides O(1) indexed access, which is more efficient than linked containers.

// Second, std::vector uses contiguous memory, resulting in low memory overhead and good cache locality, making
// it efficient for storing a large number of music objects.

// Finally, additions and deletions are infrequent in the music library. Since most operations are read-heavy,
// the occasional reallocation cost of std::vector is acceptable.

// Therefore, std::vector provides the best balance of performance, memory efficiency, and simplicity for this design.
//------------------------------------------------------------------------------------------------------//

// 2. Justification for Using std::list for the Playback Queue

// std::list is an appropriate container for the playback queue because it supports frequent modifications while
//maintaining stable references to elements.

// First, the playback queue often requires insertion and removal of songs in the middle of the queue, for example 
// when users reorder tracks or skip songs. std::list allows O(1) insertion and deletion at any position once an iterator
// is available, while std::vector would require shifting elements, resulting in O(n) complexity.

// Second, std::list ensures that pointers and iterators to existing elements are not invalidated when the list is 
// modified. This is important because the currently playing song may be referenced while other songs are added or removed from the queue.

// // Therefore, due to efficient middle insertions/removals and stable iterators, std::list is a suitable choice for 
// managing the playback queue.

// 3. Unordered map
// std::map is a suitable data structure for indexing songs by title because it provides efficient lookup and maintains sorted order automatically.

// First, std::map is typically implemented as a self-balancing binary search tree, which guarantees O(log n) time complexity for insertion,
// deletion, and lookup. This makes searching for a song by its title efficient even when the music library grows large.

// Second, std::map automatically keeps its keys sorted in alphabetical order. This property is useful for features such as displaying a sorted 
// tracklist or browsing songs by title without requiring additional sorting logic.

// Finally, the number of insertions into the map is limited to the library initialization phase, while lookups are frequent during runtime.
// This usage pattern aligns well with the performance characteristics of std::map