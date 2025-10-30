
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <map> 

using namespace std;

struct Song {
    string artist_name;
    string track_name;
    string release_date;
    string genre;
    int len;
    string topic;

    Song* next;

    Song(string artist, string track, string date,
         string g, int length, string t) {
        
        this->artist_name = artist;
        this->track_name = track;
        this->release_date = date;
        this->genre = g;
        this->len = length;
        this->topic = t;
        this->next = nullptr;
    }
};

class Playlist {
private:
    Song* head;
    string name;

public:
    Playlist(string playlist_name);

    void addSong(string artist, string track, string date,
                 string g, int length, string t);

    void deleteSong(string track_name);

    void printPlaylist();

    void mergeTwoplaylist(Playlist& other_playlist); 
    void shuffleMerge(map<string, Playlist>& allPlaylists); 
    
    void sort(string sort_by);
    Playlist filter(string filter_by, string value); 
    
    Song* findSong(string track_name); 
    
    void playInShuffleMode();
    
    string getName() { return name; }
    Song* getHead() { return head; } 
    bool containsSong(string track_name);
};



