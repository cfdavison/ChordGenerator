// ChordGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

enum Note {
    A,
    ASHARP,
    B,
    C,
    CSHARP,
    D,
    DSHARP,
    E,
    F,
    FSHARP,
    G,
    GSHARP
};
static const string note_strings[] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };

enum Mode {
    IONIAN,     // Major
    DORIAN,     // flat 3, flat 7
    PHRYGIAN,   // flat 2, flat 3, flat 6, flat 7
    LYDIAN,     // sharp 4
    MIXOLYDIAN, // flat 7
    AEOLIAN,    // flat 3, flat 6, flat 7
    LOCRIAN,    // flat 2, flat 3, flat 5, flat 6, flat 7
    BLUES,      
};
static const string mode_strings[] = { "ionian", "dorian", "phrygian", "lydian", "mixolydian", "aeolian", "locrian", "blues"};

enum ChordType {
    MAJ,
    MIN,
    DIM
};
static const string chord_type_string[] = { "Maj", "min", "dim" };



//shift the values of v n to the left
template<typename T>
vector<T> shiftVector(vector<T> v, int n) {
    vector<T> new_v;
    for (int i = 0; i < v.size(); i++) 
    {
        new_v.push_back(v[(n + i) % v.size()]);
    }

    return new_v;
}

vector<Note> getNotesInKey(Note key, Mode mode) 
{
    vector<int> notePattern = { 2, 2, 1, 2, 2, 2, 1 };
    if (mode == BLUES) notePattern = { 2, 2, 1, 2, 2, 2, 1 };
    else {
        notePattern = shiftVector(notePattern, static_cast<int>(mode));
    }
    vector<Note> notes;
    notes.push_back(key);
    int int_key = static_cast<int>(key);
    for (int i = 0; i < notePattern.size() - 1; i++) 
    {
        int_key = (int_key + notePattern[i]) % 12;

        notes.push_back(static_cast<Note>(int_key));
    }
    return notes;
}

vector<pair<Note, ChordType>> getChordsInKey(Note key, Mode mode) {
    vector<Note> notes = getNotesInKey(key, mode);
    vector<ChordType> chordPattern = { MAJ, MIN, MIN, MAJ, MAJ, MIN, DIM };
    chordPattern = shiftVector(chordPattern, static_cast<int>(mode));


    vector<pair<Note, ChordType>> chords;
    for (int i = 0; i < chordPattern.size(); i++)
    {
        pair<Note, ChordType> chord = { notes[i], chordPattern[i] };
        chords.push_back(chord);
    }

    return chords;
}

vector<vector<Note>> generateNeck(vector<Note> tuning = { E,A,D,G,B,E }) 
{
    //Each string is represented by a vector<Note>
    vector<vector<Note>> neck;

    for (int s = 0; s < tuning.size(); s++) 
    {
        vector<Note> str;
        for (int f = 0; f < 12; f++) 
        {
            Note open = tuning[tuning.size() - s -1];
            str.push_back(Note((open + f) % 12));
        }
        neck.push_back(str);
    }

    return neck;
}

string drawNeck(vector<vector<Note>> neck) {
    const int fret_spacing = 4;
    const int nut_spacing = 2;

    string nut = "||";
    string fret = "|";
    string ret = "";

    for(int s = 0; s < neck.size(); s++) 
    {
        string open = note_strings[neck[s][0]];
        ret.append(open);
        ret.append(string(nut_spacing - open.size() + 1, ' '));
        ret.append(nut);
        for (int f = 1; f < 12; f++)
        {
            string note = note_strings[neck[s][f]];
            ret.append(string(fret_spacing, ' '));
            ret.append(note);
            //Strings of size 1 (C) will get 4 spaces, size 2 (C#) will get 3
            ret.append(string(fret_spacing - note.size() + 1 , ' '));
            ret.append(fret);
        }
        ret.append("\n");
    }

    //Fret nums
    ret.append(string(nut_spacing + 1 + nut.size(), ' '));
    for (int f = 1; f < 12; f++) {
        string fret_num = "(" + to_string(f) + ")";
        ret.append(string(fret_spacing - 1, ' '));
        ret.append(fret_num);
        ret.append(string(fret_spacing - fret_num.size() + 2 + fret.size(), ' '));
    }
    

    return ret;
}

string to_string(vector<Note> notes) 
{
    string s = "";
    for (int i = 0; i < notes.size(); i++) {
        s.append(note_strings[static_cast<int>(notes[i])]);
        s.append(" ");
    }
    return s;
}

string to_string(vector<pair<Note, ChordType>> chords) {
    string s = "";
    for (int i = 0; i < chords.size(); i++) {
        s.append(note_strings[static_cast<int>(chords[i].first)]);
        
        s.append(chord_type_string[static_cast<int>(chords[i].second)]);
        s.append(" ");
    }
    return s;
}

int main()
{
    Note key = GSHARP;
    Mode mode = LOCRIAN;
    auto notes = getNotesInKey(key, mode);
    auto chords = getChordsInKey(key, mode);
    cout << note_strings[key] << " " << mode_strings[mode] << ": " << to_string(notes) << endl;
    cout << note_strings[key] << " " << mode_strings[mode] << ": " << to_string(chords) << endl;
    cout << endl;
    cout << drawNeck(generateNeck());

}
