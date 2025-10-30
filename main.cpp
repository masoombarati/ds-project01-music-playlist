#include "playlist.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <map> 

void loadCsvData(Playlist& playlist, const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "!!! could not open file: " << filename << endl;
        return;
    }

    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string artist, track, date, genre, topic, len_str;
        int len;

        getline(ss, artist, ',');
        getline(ss, track, ',');
        getline(ss, date, ',');
        getline(ss, genre, ',');
        getline(ss, len_str, ',');
        getline(ss, topic);

        try {
            len = stoi(len_str);
        } catch (...) {
            len = 0;
        }
        playlist.addSong(artist, track, date, genre, len, topic);
    }
    file.close();
    cout << "Songs successfully loaded into 'All Songs' playlist." << endl;
}

void showMenu() {
    cout  << endl << "****** Music Playlist Manager ******" << endl;
    cout << "1. Create a new playlist" << endl;
    cout << "2. Delete a playlist" << endl;
    cout << "3. List all playlists" << endl;
    cout << "4. Add a song to a playlist" << endl;
    cout << "5. Delete a song from a playlist" << endl;
    cout << "6. Merge two playlists (A = A + B)" << endl;
    cout << "7. Shuffle merge playlists (New = A + B...)" << endl; 
    cout << "8. Filter a playlist" << endl; 
    cout << "9. Sort a playlist" << endl;
    cout << "10. Play a playlist (Normal)" << endl; 
    cout << "11. Play a playlist (Shuffle)" << endl;
    cout << "12. Like a song" << endl; 
    cout << "13. Dislike a song" << endl;
    cout << "q. Quit" << endl << endl;
    cout << "Enter your choice: ";
}

Playlist* promptAndFindPlaylist(map<string, Playlist>& playlists) {
    string name;
    cout << "Enter playlist name: ";
    getline(cin, name);
    
    if (playlists.find(name) == playlists.end()) {
        cout << "Error: Playlist '" << name << "' not found." << endl;
        return nullptr;
    }
    
    return &playlists.at(name);
}

Playlist* findPlaylist(map<string, Playlist>& playlists, const string& name) {
     if (playlists.find(name) == playlists.end()) {
        cout << "Error: Playlist '" << name << "' not found." << endl;
        return nullptr;
    }
    return &playlists.at(name);
}


void createPlaylist(map<string, Playlist>& playlists) {
    string name;
    cout << "--- Create New Playlist ---" << endl;
    cout << "Enter new playlist name: ";
    getline(cin, name);

    if (playlists.find(name) != playlists.end()) {
        cout << "Error: A playlist with this name already exists." << endl;
        return;
    }
    
    playlists.emplace(name, Playlist(name));
    cout << "Playlist '" << name << "' created." << endl;
}

void deletePlaylist(map<string, Playlist>& playlists) {
    string name;
    cout << "--- Delete Playlist ---" << endl;
    cout << "Enter playlist name to delete: ";
    getline(cin, name);

    if (name == "All Songs" || name == "Liked") {
        cout << "Error: Cannot delete the default 'All Songs' or 'Liked' playlists." << endl;
        return;
    }

    if (playlists.find(name) == playlists.end()) {
        cout << "Error: Playlist not found." << endl;
        return;
    }

    playlists.erase(name); 
    cout << "Playlist '" << name << "' deleted." << endl;
}

void listPlaylists(map<string, Playlist>& playlists) {
    cout << "--- All Playlists ---" << endl;
    for (auto const& [name, playlist] : playlists) {
        cout << "- " << name << endl;
    }
    cout << "---------------------" << endl;
}

void addSongToPlaylist(map<string, Playlist>& playlists) {
    cout << "--- Add Song to Playlist ---" << endl;
    Playlist* playlist = promptAndFindPlaylist(playlists);
    if (playlist == nullptr) return;

    string artist, track, date, genre, topic, len_str;
    int len;
    
    cout << "Press ENTER to begin...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter track name: ";
    getline(cin, track);
    cout << "Enter artist name: ";
    getline(cin, artist);
    cout << "Enter release date (year): ";
    getline(cin, date);
    cout << "Enter genre: ";
    getline(cin, genre);
    cout << "Enter topic: ";
    getline(cin, topic);
    cout << "Enter length (in seconds): ";
    getline(cin, len_str);

    try { len = stoi(len_str); } 
    catch (...) { len = 0; }

    playlist->addSong(artist, track, date, genre, len, topic);
    cout << "Song '" << track << "' added to '" << playlist->getName() << "'." << endl;
}

void deleteSongFromPlaylist(map<string, Playlist>& playlists) {
    cout << "--- Delete Song from Playlist ---" << endl;
    Playlist* playlist = promptAndFindPlaylist(playlists); 
    if (playlist == nullptr) return;

    string trackName;

    cout << "Enter the exact track name to delete: ";
    getline(cin, trackName);
    
    playlist->deleteSong(trackName);
}

void mergePlaylists(map<string, Playlist>& playlists) {
    cout << "--- Merge Two Playlists ---" << endl;
    cout << "Enter name of playlist to merge INTO (e.g., Playlist A): " << endl;
    Playlist* pA = promptAndFindPlaylist(playlists); 
    if (pA == nullptr) return;

    cout << "Enter name of playlist to merge FROM (e.g., Playlist B): " << endl;
    Playlist* pB = promptAndFindPlaylist(playlists); 
    if (pB == nullptr) return;

    if (pA->getName() == pB->getName()) {
        cout << "Error: Cannot merge a playlist with itself." << endl;
        return;
    }

    pA->mergeTwoplaylist(*pB); 
}

void shuffleMergePlaylists(map<string, Playlist>& playlists) {
    cout << "--- Shuffle Merge Playlists ---" << endl;
    string newName;
    cout << "Enter a name for your NEW merged playlist: ";
    getline(cin, newName);

    if (playlists.find(newName) != playlists.end()) {
        cout << "Error: A playlist with this name already exists." << endl;
        return;
    }
    
    playlists.emplace(newName, Playlist(newName));
    Playlist* newPlaylist = &playlists.at(newName);
    cout << "New playlist '" << newName << "' created." << endl;

    newPlaylist->shuffleMerge(playlists);
}

void filterPlaylist(map<string, Playlist>& playlists) {
    cout << "--- Filter a Playlist ---" << endl;
    Playlist* playlist = promptAndFindPlaylist(playlists); 
    if (playlist == nullptr) return;

    string filter_by, value;
    cout << "Filter by (1) genre, (2) artist, or (3) year: ";
    getline(cin, filter_by);

    if (filter_by == "1") filter_by = "genre";
    else if (filter_by == "2") filter_by = "artist";
    else if (filter_by == "3") filter_by = "year";
    else {
        cout << "Invalid choice." << endl;
        return;
    }

    cout << "Enter the exact value to filter for (e.g., Pop, Van Halen, 1984): ";
    getline(cin, value);

    Playlist filtered = playlist->filter(filter_by, value);

    cout << "Enter a name for this new filtered playlist: ";
    string newName;
    getline(cin, newName);

    if (playlists.find(newName) != playlists.end()) {
        cout << "A playlist with this name already exists. Overwriting." << endl;
        playlists.at(newName) = filtered;
    } else {
        playlists.emplace(newName, filtered);
    }
    cout << "Filtered playlist saved as '" << newName << "'." << endl;
}

void sortPlaylist(map<string, Playlist>& playlists) {
    cout << "--- Sort Playlist ---" << endl;
    Playlist* playlist = promptAndFindPlaylist(playlists); 
    if (playlist == nullptr) return;
    
    string sort_choice;
    cout << "Sort by (1) track, (2) artist, or (3) year (newest): ";
    getline(cin, sort_choice);

    if (sort_choice == "1") playlist->sort("track");
    else if (sort_choice == "2") playlist->sort("artist");
    else if (sort_choice == "3") playlist->sort("year");
    else cout << "Invalid choice." << endl;
}

void likeASong(map<string, Playlist>& playlists) {
    cout << "--- Like a Song ---" << endl;
    cout << "First, find the song in a playlist." << endl;
    Playlist* playlist = promptAndFindPlaylist(playlists);
    if (playlist == nullptr) return;

    string trackName;
    cout << "Enter the exact track name to LIKE: ";
    getline(cin, trackName);

    Song* song = playlist->findSong(trackName);
    if (song == nullptr) {
        cout << "Error: Song '" << trackName << "' not found in '" << playlist->getName() << "'." << endl;
        return;
    }

    Playlist& liked = playlists.at("Liked");
    if (liked.containsSong(trackName)) {
        cout << "Song is already in your 'Liked' playlist." << endl;
    } else {
        liked.addSong(song->artist_name, song->track_name, song->release_date,
                      song->genre, song->len, song->topic);
        cout << "'" << trackName << "' added to 'Liked' playlist." << endl;
    }
}

void dislikeASong(map<string, Playlist>& playlists) {
    cout << "--- Dislike a Song ---" << endl;
    cout << "This will remove a song from your 'Liked' playlist." << endl;
    Playlist& liked = playlists.at("Liked");

    string trackName;
    cout << "Enter the exact track name to DISLIKE: ";
    getline(cin, trackName);

    liked.deleteSong(trackName);
}


int main() {
  
    map<string, Playlist> allPlaylists;
    
    allPlaylists.emplace("All Songs", Playlist("All Songs"));
    allPlaylists.emplace("Liked", Playlist("Liked"));
    
    loadCsvData(allPlaylists.at("All Songs"), "music.csv");

    string choice_str;
    char choice;

    while (true) {
        showMenu();
        getline(cin, choice_str);
        
        if (choice_str == "1") choice = '1';
        else if (choice_str == "2") choice = '2';
        else if (choice_str == "3") choice = '3';
        else if (choice_str == "4") choice = '4';
        else if (choice_str == "5") choice = '5';
        else if (choice_str == "6") choice = '6';
        else if (choice_str == "7") choice = '7'; 
        else if (choice_str == "8") choice = '8'; 
        else if (choice_str == "9") choice = '9'; 
        else if (choice_str == "10") choice = '10'; 
        else if (choice_str == "11") choice = '11'; 
        else if (choice_str == "12") choice = '12';
        else if (choice_str == "13") choice = '13'; 
        else if (choice_str == "q" || choice_str == "Q") choice = 'q';
        else choice = ' '; 

        cin.clear();

        switch (choice) {
            case '1': createPlaylist(allPlaylists); break;
            case '2': deletePlaylist(allPlaylists); break;
            case '3': listPlaylists(allPlaylists); break;
            case '4': addSongToPlaylist(allPlaylists); break;
            case '5': deleteSongFromPlaylist(allPlaylists); break; 
            case '6': mergePlaylists(allPlaylists); break;
            case '7': shuffleMergePlaylists(allPlaylists); break;
            case '8': filterPlaylist(allPlaylists); break; 
            case '9': sortPlaylist(allPlaylists); break; 
            case '10': { 
                cout << "--- Play Playlist (Normal) ---" << endl;
                Playlist* p = promptAndFindPlaylist(allPlaylists); 
                if (p) p->printPlaylist();
                break;
            }
            case '11': { 
                cout << "--- Play Playlist (Shuffle) ---" << endl;
                Playlist* p = promptAndFindPlaylist(allPlaylists); 
                if (p) p->playInShuffleMode();
                break;
            }
            case '12': likeASong(allPlaylists); break; 
            case '13': dislikeASong(allPlaylists); break; 
            case 'q':
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    return 0;
}


