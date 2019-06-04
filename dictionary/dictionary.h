#ifndef DICTIONARY_H
#define DICTIONARY_H

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

#include<limits>
using std::numeric_limits;

// gets delimiter
string get_delimiter_file();

// gets in-between period
string get_period_file();

// gets end period
string get_end_period_file();

// symbolic names
constexpr char newline { '\n' };
constexpr char whitespace { ' ' };
constexpr char delimiter_dictionary { ':' };
constexpr char exit_character { 'x' };
constexpr char end_of_line { '$' };

const string exit_sequence { ":exit" };
const string INVALID_CHOICE = "0";

// file delimiter and periods
const string delimiter_file = get_delimiter_file();
const string period_file = get_period_file();
const string end_period_file = get_end_period_file();

constexpr size_t minimum_number_of_words { 10 }; // minimum number of words that triggers practice mmode
constexpr size_t INITIAL_POSITION = 0;
constexpr size_t INVALID_POSITION = numeric_limits<unsigned int>::max();

// file names
const string dictionary_filename { "dictionary.txt" };
const string practice_filename { "practice.txt" };
const string resume_filename { "resume.txt" };

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

	// modes
    enum class Mode {
        normal, reverse, practice_normal, practice_reverse, resume_normal, resume_reverse
    };
};

// user-defined type Practice
struct Practice {
	// default constructor
	Practice() noexcept
		: indexes_left{}
		, indexes_right{}{}

	// retry indexes
	vector<size_t> indexes_left;
	vector<size_t> indexes_right;
};

// user-defined type Resume
struct Resume {
	// default constructor
	Resume() noexcept
		: position_left{}
		, position_right{}
		, indexes_left{}
		, indexes_right{}{}

	// updates position
	void update_position(const size_t&, const Dictionary::Mode&);

	// updates indexes
	void update_indexes(const vector<size_t>&, const Dictionary::Mode&);

	// checks if valid
	bool is_valid();

	// positions
	size_t position_left;
	size_t position_right;

	// resume indexes
	vector<size_t> indexes_left;
	vector<size_t> indexes_right;
};

// retrieves dictionary information from file
Dictionary get_dictionary();

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

// sets file
void set_file(const string&, const size_t&);

// updates practice file
void update_practice_file(const Practice&);

// updates resume file
void update_resume_file(const Resume&);

// writes a single element on a file
template<typename T>
void write_single_element(const T& t, const string& filename, const string& period, ios_base::openmode mode = ios_base::out)
// writes a single element on filename
// the writing is ended by the period
{
	fstream file;
	file.open(filename, mode);

	if (file.is_open()) {
		file << t << period;
		file.close();
	}
	else
		cerr << "Error: Unable to open file.\n";
}

// writes elements of a container on a file
template<typename InputIterator>
void write_elements(InputIterator first, InputIterator last, const string& filename, const string& delimiter, const string& period, ios_base::openmode mode = ios_base::out)
// write elements of a container on file
// elements are delimited by the delimiter
// the writing is ended by the period
{
    fstream file;
    file.open(filename, mode);

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

// gets length of a word
size_t get_length(const string&);

// gets position
size_t get_position(const Resume&, const Dictionary::Mode&);

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
Dictionary::Mode get_mode_practice(const Dictionary::Mode&);

// gets indexes practice 
vector<size_t>& get_indexes_practice(Practice&, const Dictionary::Mode&);

// quiz launcher
Practice quiz_launcher(const Dictionary&, const Practice&, const Resume&, const Dictionary::Mode&);

#endif
