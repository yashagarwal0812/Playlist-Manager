#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>
#include <thread>
#include <random>

using namespace std;

class Song
{
public:
    Song(const string &title, const string &artist, const string &duration)
    {
        this->title = title;
        this->artist = artist;
        this->duration = duration;
        next = nullptr;
    }

    // Getter functions
    string getTitle()
    {
        return title;
    }
    string getArtist()
    {
        return artist;
    }
    string getDuration()
    {
        return duration;
    }
    Song *getNext()
    {
        return next;
    }

    // Setter function for updating the next pointer
    void setNext(Song *song)
    {
        next = song;
    }

private:
    string title;
    string artist;
    string duration;
    Song *next; // Pointer to the next song in the linked list
};

class Playlist
{
public:
    Playlist()
    {
        head = nullptr;
        tail = nullptr;
        currentSong = nullptr;
    }

    void setNull()
    {
        head = nullptr;
        tail = nullptr;
        currentSong = nullptr;
    }

    // Function to add a song to the playlist
    void addSong(const Song &song)
    {
        Song *newSong = new Song(song);
        if (!head)
        {
            head = tail = newSong;
        }
        else
        {
            tail->setNext(newSong);
            tail = newSong;
        }

        // Set the current song to the first song added
        if (!currentSong)
        {
            currentSong = head;
        }
    }

    // Function to shuffle the playlist
    void shufflePlaylist()
    {
        srand(time(0)); // Seed for randomness
        Song *current = head;

        // Count the number of songs in the playlist
        int count = 0, i = 0;
        vector<Song *> songVector;
        while (current != nullptr)
        {
            count++;
            songVector.push_back(current);
            current = current->getNext();
        }

        // Convert the linked list to a vector for shuffling
        current = head;
        // for (int i = 0; i < count; ++i)
        // {
        //     songVector[i] = current;
        //     current = current->getNext();
        // }

        // Shuffle the vector
        shuffle(songVector.begin(), songVector.end(), default_random_engine(rand()));

        // Reconstruct the linked list from the shuffled vector
        head = nullptr;
        tail = nullptr;
        for (int i = 0; i < count; ++i)
        {
            if (!head)
            {
                head = tail = songVector[i];
            }
            else
            {
                tail->setNext(songVector[i]);
                tail = songVector[i];
            }
        }
        tail->setNext(nullptr);
        currentSong = head;
    }

    // Function to repeat the current song n times
    void repeatCurrentSong(int numRepeats)
    {
        if (currentSong)
        {
            Song *temp = currentSong->getNext();
            Song *temp2 = currentSong;
            for (int i = 1; i < numRepeats; ++i)
            {
                Song *repeatedSong = new Song(*currentSong);
                temp2->setNext(repeatedSong);
                temp2 = repeatedSong;
            }
            temp2->setNext(temp);
        }
    }

    // Function to display the playlist with interactive playback
    void displayPlaylist();

    // Function to play the current song
    void playCurrentSong()
    {
        if (currentSong)
        {
            cout << "Now playing: " << currentSong->getTitle() << " by " << currentSong->getArtist() << "\n\n";
        }
        else
        {
            cout << "No current song to play.\n\n";
        }
    }

    // Function to move to the next song in the playlist
    void nextSong()
    {
        if (currentSong && currentSong->getNext())
        {
            currentSong = currentSong->getNext();
            cout << "Switched to the next song: " << currentSong->getTitle() << " by " << currentSong->getArtist() << "\n\n";
        }
        else
        {
            cout << "No next song available. End of playlist.\n\n";
            currentSong = nullptr;
            cout << "Do you want to start it again? \nPress 1 for yes or 2 for no: ";
            char choice1;
            cin >> choice1;
            while (choice1 != '1' && choice1 != '2')
            {
                cout << "Enter a valid choice: ";
                cin >> choice1;
            }
            if (choice1 == '1')
            {
                currentSong = head;
            }
            // Set currentSong to null to indicate the end of the playlist
        }
    }

    // Function to move to the previous song in the playlist
    void previousSong()
    {
        Song *current = head;

        // Find the previous song
        while (current && current->getNext() != currentSong)
        {
            current = current->getNext();
        }

        if (current && current->getNext())
        {
            currentSong = current;
            cout << "Switched to the previous song: " << currentSong->getTitle() << " by " << currentSong->getArtist() << "\n\n";
        }

        else
        {
            cout << "No previous song available.\n\n";
        }
    }

    // Function to remove the current song from the playlist
    void removeCurrentSong()
    {
        if (!head)
        {
            cout << "Playlist is empty.\n\n";
            return;
        }

        Song *temp = currentSong;

        if (currentSong == head)
        {
            head = currentSong->getNext();
            if (!head)
            {
                tail = nullptr;
            }
            currentSong = head;
        }

        else
        {
            Song *prev = head;
            while (prev && prev->getNext() != currentSong)
            {
                prev = prev->getNext();
            }

            if (prev)
            {
                prev->setNext(currentSong->getNext());
                if (prev->getNext() == nullptr)
                {
                    tail = prev;
                }
                currentSong = prev->getNext();
            }
        }

        delete temp;
        cout << "Current song removed from the playlist.\n\n";
    }

    // Function to save the playlist to a CSV file
    void savePlaylistToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file " << filename << " for writing.\n\n";
            return;
        }

        Song *current = head;
        while (current != nullptr)
        {
            file << current->getTitle() << ',' << current->getArtist() << ',' << current->getDuration() << '\n';
            current = current->getNext();
        }

        file.close();
        if (!file)
        {
            cerr << "Error writing to file " << filename << ".\n\n";
            return;
        }

        cout << "Playlist saved to " << filename << ".\n\n";
    }

    // Getter for head
    Song *getHead()
    {
        return head;
    }

    // Getter for tail
    Song *getTail()
    {
        return tail;
    }

    // Getter for currentSong
    Song *getCurrentSong()
    {
        return currentSong;
    }

    // Destructor to free memory
    ~Playlist()
    {
        Song *current = head;
        while (current != nullptr)
        {
            Song *next = current->getNext();
            delete current;
            current = next;
        }
    }

private:
    Song *head;
    Song *tail;
    Song *currentSong;
};

// Function to display the menu and get user input
char displayMenuAndGetInput()
{
    char choice;
    cout << "\n######  #          #    #     # #       ###  #####  #######\n";
    cout << "#     # #         # #    #   #  #        #  #     #    #   \n";
    cout << "#     # #        #   #    # #   #        #  #          #   \n";
    cout << "######  #       #     #    #    #        #   #####     #  \n";
    cout << "#       #       #######    #    #        #        #    # \n";
    cout << "#       #       #     #    #    #        #  #     #    # \n";
    cout << "#       ####### #     #    #    ####### ###  #####     #  \n\n";

    cout << "Menu:\n";
    cout << "1. Display Playlist\n";
    cout << "2. Play Current Song\n";
    cout << "3. Next Song\n";
    cout << "4. Previous Song\n";
    cout << "5. Remove Current Song\n";
    cout << "6. Shuffle Playlist\n";
    cout << "7. Repeat Current Song\n";
    cout << "8. Save Playlist to File\n";
    cout << "q. Quit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cout << endl;
    cout << endl;

    // Clear the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return choice;
}

// Function definition for displaying the playlist with interactive playback
void Playlist::displayPlaylist()
{
    Song *current = head;
    while (current != nullptr)
    {
        cout << "Title: " << current->getTitle() << "\nArtist: " << current->getArtist()
             << "\nDuration: " << current->getDuration() << "\n\n";

        cout << "Options:\n";
        cout << "1. Next Song\n";
        cout << "2. Stop Display\n";
        // cout << "3. Shuffle Playlist\n";
        // cout << "4. Repeat Current Song\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        cout << endl;
        cout << endl;
        while (!(choice == '1' || choice == '2'))
        {
            cout << "Enter a valid choice: ";
            cin >> choice;
            cout << endl;
            cout << endl;
        }

        switch (choice)
        {

        case '1':
            current = current->getNext();
            break;

        case '2':
            cout << "Display stopped.\n\n";
            return;

        case '3':
            shufflePlaylist();
            cout << "Playlist shuffled.\n\n";
            current = head;
            break;

        case '4':
            int numRepeats;
            cout << "Enter the number of times to repeat the current song: ";
            cin >> numRepeats;
            repeatCurrentSong(numRepeats);
            cout << "Current song repeated " << numRepeats << " times.\n";
            break;

        default:
            cout << "Invalid choice. Continuing playback.\n\n";
        }
    }
    cout << "End of playlist.\n\n";
}

void createPlaylist(string filename, Playlist *&playlist)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string title, artist, duration;

            // Read each column of the CSV
            getline(ss, title, ',');
            getline(ss, artist, ',');
            getline(ss, duration, ',');

            // Add the song to the playlist
            playlist->addSong(Song(title, artist, duration));
        }

        // Close the file
        file.close();
        cout << filename << " Playlist opened" << endl;
    }
    else
    {
        cerr << "Error opening the file." << endl;
        return;
    }
}

// Main function
int main()
{
    // Create a playlist
    Playlist *playlist = new Playlist();
    string filename = "songs";

    // Read songs from the CSV file
    createPlaylist(filename, playlist);

    char choice;
    do
    {
        cout << "\n######  #          #    #     # #       ###  #####  #######\n";
        cout << "#     # #         # #    #   #  #        #  #     #    #   \n";
        cout << "#     # #        #   #    # #   #        #  #          #   \n";
        cout << "######  #       #     #    #    #        #   #####     #  \n";
        cout << "#       #       #######    #    #        #        #    # \n";
        cout << "#       #       #     #    #    #        #  #     #    # \n";
        cout << "#       ####### #     #    #    ####### ###  #####     #  \n\n";

        cout << "Menu:\n";
        cout << "1. Display Playlist\n";
        cout << "2. Play Current Song\n";
        cout << "3. Next Song\n";
        cout << "4. Previous Song\n";
        cout << "5. Remove Current Song\n";
        cout << "6. Shuffle Playlist\n";
        cout << "7. Repeat Current Song\n";
        cout << "8. Save Playlist to File\n";
        cout << "9. Open Another Playlist\n";
        cout << "q. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        cout << endl;

        switch (choice)
        {
        case '1':
            playlist->displayPlaylist();
            break;
        case '2':
            playlist->playCurrentSong();
            break;
        case '3':
            playlist->nextSong();
            break;
        case '4':
            playlist->previousSong();
            break;
        case '5':
            playlist->removeCurrentSong();
            break;
        case '6':
            playlist->shufflePlaylist();
            cout << "Playlist shuffled.\n";
            break;
        case '7':
            int numRepeats;
            cout << "Enter the number of times to repeat the current song: ";
            cin >> numRepeats;
            playlist->repeatCurrentSong(numRepeats);
            cout << "Current song repeated " << numRepeats << " times.\n";
            break;
        case '8':
        {
            cin.ignore(); // Clear the newline character left in the buffer
            string filename;
            cout << "Enter the filename to save the playlist: ";
            getline(cin, filename);

            playlist->savePlaylistToFile(filename);
            break;
        }
        case '9':
        {
            playlist->setNull();
            cout << "Enter name of Playlist:";
            cin >> filename;
            createPlaylist(filename, playlist);
            break;
        }
        case 'q':
            cout << "Exiting the program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        this_thread::sleep_for(chrono::seconds(1)); // 1-second delay
    } while (choice != 'q');

    return 0;
}