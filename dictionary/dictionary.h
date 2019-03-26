#pragma once

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<fstream>
using std::fstream;
using std::ios_base;

#include<random>
using std::random_device;

#include<algorithm>
using std::shuffle;

#include<string>
using std::string;

#include<vector>
using std::vector;

// symbolic names
constexpr char newline { '\n' };
constexpr char whitespace { ' ' };
constexpr char file_delimiter { ':' };
constexpr char exit_character { 'x' };

const string INVALID_CHOICE = "0";

// file names
const string dictionary_filename { "dictionary.txt" };
const string practice_filename_left { "practice_left.txt" };
const string practice_filename_right { "practice_right.txt" };

// user-defined type Dictionary
struct Dictionary {
    // default constructor
    Dictionary() noexcept
        : first_language{}
		, second_language{}
		, words_left{}
        , words_right{}{}

	// languages
	string first_language;
	string second_language;

    // words and translations
    vector<string> words_left;
    vector<string> words_right;

	// Modes
    enum class Mode {
        normal, reverse
    };
};

// retrieves dictionaory information from file
Dictionary get_words_and_translations();

// shuffles elements of a container
template<typename InputIterator>
void shuffle_elements(InputIterator first, InputIterator last)
// rearranges the elements of a container randomly
{
    random_device rd;
    shuffle(first, last, rd);
}

// creates file if it doesn't exit
void create_file_if(const string&);

// writes elements of a container on a file
template<typename InputIterator>
void write_elements(InputIterator first, InputIterator last, const string& file_name, const string& delimiter, const string& period, ios_base::openmode mode = ios_base::out)
// write elements of a container on file
// elements are delimited by the delimiter
// the writing is ended by the period
{
    fstream file;
    file.open(file_name, mode);

    if (file.is_open()) { 
        if (first == last) file << period;
        else {
            for (; first != last; ++first){ 
                file << *first << ((first + 1 != last) ? delimiter : period);
            }   
        }   
        file.close();
    }
    else
        cerr << "Error: Unable to open file.\n";
}

// displays menu
void display_menu();

// gets a random distribution of unique unsigned integers
vector<size_t> get_random_int_distribution(const size_t&);

// gets user's answer
string get_answer();

// gets the words to be translated
const vector<string>& get_words_left(const Dictionary&, const Dictionary::Mode&);

// gets the translations
const vector<string>& get_words_right(const Dictionary&, const Dictionary::Mode&);

// gets length of a word
size_t get_length(const string&);

// quiz launcher
size_t quiz_launcher(const Dictionary&, const Dictionary::Mode&);
