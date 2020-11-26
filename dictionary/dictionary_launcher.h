#ifndef DICTIONARY_LAUNCHER_H
#define DICTIONARY_LAUNCHER_H

#include "dictionary.h"
#include "practice.h"
#include "resume.h"
#include "statistics.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;

#include<fstream>
using std::ios_base;

#include<random>
using std::random_device;

#include<algorithm>
using std::shuffle;

#include<string>
using std::wstring;

#include<limits>
using std::numeric_limits;

// converts a wstring to an UTF8 string
string to_u8string(const wstring&);

// converts an UTF8 string to a wstring
wstring to_wstring(const string&);

// converts an UTF8 string to a string
string to_string(const string&);

// gets delimiter
string get_delimiter_file();

// gets in-between period
string get_period_file();

// gets end period
string get_end_period_file();

// symbolic names
constexpr char newline { '\n' };
constexpr size_t invalid_position = numeric_limits<unsigned int>::max();

// file delimiter and periods
const string delimiter_file = get_delimiter_file();
const string period_file = get_period_file();
const string end_period_file = get_end_period_file();

// retrieves dictionary information from file
Dictionary get_dictionary();

// retrieves statistics information from file
Statistics get_statistics();

// retrieves practice information from file
Practice get_practice();

// retrieves resume information from file
Resume get_resume();

// shuffles elements of a container
template<typename InputIterator>
void shuffle_elements(InputIterator first, InputIterator last)
// rearranges the elements of a container randomly
{
    random_device rd;
    shuffle(first, last, rd);
}

// sets required files 
void set_required_files();

// sets dictionary file
void set_dictionary_file();

// sets practice file
void set_practice_file();

// sets resume file
void set_resume_file();

// sets statistics file
void set_statistics_file();

// sets file
void set_file(const string&, const size_t&);

// creates file if it doesn't exit
void create_file_if(const string&);

// updates the statistics data
Statistics update_statistics(const Statistics&, const Dictionary&);

// updates the practice data
Practice update_practice(const Practice&, const Resume&, const Dictionary&);

// updates the resume data
Resume update_resume(const Resume&, const Dictionary&);

// updates the statistics file
void update_statistics_file(const Statistics&);

// updates practice file
void update_practice_file(const Practice&);

// updates resume file
void update_resume_file(const Resume&);

// writes a single element on a file
template<typename T>
void write_single_element(const T&, const string&, const string& period, const ios_base::openmode& = ios_base::out | ios_base::binary);

// writes elements of a container on a file
template<typename InputIterator>
void write_elements(InputIterator, const InputIterator, const string&, const string&, const string&, const ios_base::openmode& = ios_base::out | ios_base::binary);

// displays menu
void display_menu(const Dictionary&, const Practice&, const Resume&);

// gets a distribution of unique unsigned integers
vector<size_t> get_int_distribution(const size_t&);

// gets user's answer
string get_answer();

// gets words
vector<string>& get_words(Dictionary&, const Dictionary::Mode&);

// gets the words to be translated
const vector<string>& get_words_left(const Dictionary&, const Dictionary::Mode&);

// gets the translations
const vector<string>& get_words_right(const Dictionary&, const Dictionary::Mode&);

// gets position
size_t get_position(const Resume&, const Practice&, const Dictionary::Mode&);

// gets indexes
vector<size_t> get_indexes(const Dictionary&, const Practice&, const Resume&, const Dictionary::Mode&);

// checks if it is practice mode
bool is_practice_mode(const Dictionary::Mode&);

// checks if it is resume mode
bool is_resume_mode(const Dictionary::Mode&);

// checks if it is normal mode
bool is_normal_mode(const Dictionary::Mode&);

// checks if it is reverse mode
bool is_reverse_mode(const Dictionary::Mode&);

// gets indexes size
size_t get_indexes_size_practice(const Practice&, const Dictionary::Mode&);

// gets mode practice
Dictionary::Mode get_mode_practice(const Practice&, const Dictionary::Mode&);

// gets indexes practice 
vector<size_t>& get_indexes_practice(Practice&, const Dictionary::Mode&);

// quiz launcher
Practice quiz_launcher(const Dictionary&, const Practice&, const Resume&, const Dictionary::Mode&);

#endif
