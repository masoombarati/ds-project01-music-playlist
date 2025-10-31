#include "playlist.h"
#include <iostream>

Playlist::Playlist(string playlist_name) {
    head = nullptr;
    name = playlist_name;
}

void Playlist::addSong(string artist, string track, string date,
    string genre, int len, string topic) {

    Song* newSong = new Song(artist, track, date, genre, len, topic);

    if (head == nullptr) {
        head = newSong;
    }
    else {
        Song* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newSong;
    }
}

void Playlist::deleteSong(string track_name) {
    if (head == nullptr) {
        cout << "Playlist is empty! Cannot delete." << endl;
        return;
    }

    if (head->track_name == track_name) {
        Song* temp = head;
        head = head->next;
        delete temp;
        cout << "Song successfully deleted" << endl;
        return;
    }

    Song* prev = head;
    Song* current = head->next;

    while (current != nullptr && current->track_name != track_name) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Song not found in the playlist." << endl;
        return;
    }

    prev->next = current->next;
    delete current;
    cout << "Song successfully deleted." << endl;
}

void Playlist::printPlaylist() {
    cout << endl << "--- Displaying Playlist: " << this->name << " ---" << endl;

    Song* temp = head;
    int count = 1;

    if (head == nullptr) {
        cout << "Playlist is empty." << endl;
        cout << "-----------------------------------------------" << endl;
        return;
    }

    while (temp != nullptr) {
        cout << count << ". " << temp->track_name << " by "
            << temp->artist_name << endl;
        cout << "   Genre: " << temp->genre << " | Released: "
            << temp->release_date << endl;

        temp = temp->next;
        count++;
    }
    cout << "-----------------------------------------------" << endl;
}

void Playlist::sort(string sort_by) {
    if (head == nullptr || head->next == nullptr) {
        cout << "Playlist is empty or has only one song, no need to sort." << endl;
        return;
    }

    vector<Song*> songs;
    Song* temp = head;
    while (temp != nullptr) {
        songs.push_back(temp);
        temp = temp->next;
    }

    std::sort(songs.begin(), songs.end(), [sort_by](Song* a, Song* b) {
        if (sort_by == "track") {
            return a->track_name < b->track_name;
        }
        else if (sort_by == "artist") {
            if (a->artist_name != b->artist_name) {
                return a->artist_name < b->artist_name;
            }
            else {
                return a->track_name < b->track_name;
            }
        }
        else if (sort_by == "year") {
            return a->release_date > b->release_date;
        }
        return false;
        });

    head = songs[0];
    for (size_t i = 0; i < songs.size() - 1; ++i) {
        songs[i]->next = songs[i + 1];
    }
    songs.back()->next = nullptr;

    cout << "Playlist successfully sorted by " << sort_by << "." << endl;
    printPlaylist();
}

void Playlist::playInShuffleMode() {
    if (head == nullptr) {
        cout << "Playlist is empty! Cannot shuffle." << endl;
        return;
    }

    vector<Song*> songs;
    Song* temp = head;
    while (temp != nullptr) {
        songs.push_back(temp);
        temp = temp->next;
    }

    auto rng = std::default_random_engine(std::random_device{}());
    std::shuffle(songs.begin(), songs.end(), rng);

    cout << endl << "--- Playing Playlist: " << this->name << " (Shuffle Mode) ---" << endl;
    for (size_t i = 0; i < songs.size(); ++i) {
        cout << (i + 1) << ". " << songs[i]->track_name << " by "
            << songs[i]->artist_name << endl;
    }
    cout << "-----------------------------------------------" << endl;
}

bool Playlist::containsSong(string track_name) {
    Song* temp = head;
    while (temp != nullptr) {
        if (temp->track_name == track_name) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void Playlist::mergeTwoplaylist(Playlist& other_playlist, bool silent) {
    Song* otherHead = other_playlist.getHead();
    int count = 0;

    while (otherHead != nullptr) {
        if (!this->containsSong(otherHead->track_name)) {
            this->addSong(otherHead->artist_name, otherHead->track_name,
                otherHead->release_date, otherHead->genre,
                otherHead->len, otherHead->topic);
            count++;
        }
        otherHead = otherHead->next;
    }

    if (!silent) {
        cout << "Successfully merged " << count << " new songs into " << this->name << "." << endl;
        printPlaylist();
    }
}

Playlist Playlist::filter(string filter_by, string value) {
    Playlist filteredList("Filtered Results");
    Song* temp = head;
    bool match = false;

    while (temp != nullptr) {
        match = false;

        if (filter_by == "genre" && temp->genre == value) {
            match = true;
        }
        else if (filter_by == "artist" && temp->artist_name == value) {
            match = true;
        }
        else if (filter_by == "year" && temp->release_date == value) {
            match = true;
        }

        if (match) {
            filteredList.addSong(temp->artist_name, temp->track_name,
                temp->release_date, temp->genre,
                temp->len, temp->topic);
        }
        temp = temp->next;
    }

    cout << "Filter complete. New playlist created with results." << endl;
    filteredList.printPlaylist();
    return filteredList;
}

Song* Playlist::findSong(string track_name) {
    Song* temp = head;
    while (temp != nullptr) {
        if (temp->track_name == track_name) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}


void Playlist::shuffleMerge(map<string, Playlist>& allPlaylists) {
    string nameToAdd;
    while (true) {
        cout << "Enter a playlist name to add (or 'done' to finish): ";
        getline(cin, nameToAdd);
        if (nameToAdd == "done") break;

        if (allPlaylists.find(nameToAdd) == allPlaylists.end()) {
            cout << "Error: Playlist '" << nameToAdd << "' not found." << endl;
        }
        else {
            Playlist& p = allPlaylists.at(nameToAdd);

            if (p.getName() == this->name) {
                cout << "Skipping: Cannot merge a playlist with itself." << endl;
            }
            else {
                this->mergeTwoplaylist(p, true);
                cout << "Merged '" << p.getName() << "'." << endl;
            }
        }
    }

    cout << "Shuffle merge complete for playlist '" << this->name << "'." << endl;
    this->printPlaylist();
}

