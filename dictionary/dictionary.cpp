#include "dictionary.h"

#include<iostream>
using std::ws;

#include<fstream>
using std::fstream;
using std::ios_base;

#include<stdexcept>
using std::runtime_error;

// retrieves dictionaory information from file
Dictionary get_words_and_translations()
// first retrieves the languages then the words and their translations
// syntax: 
// "language_left"delimiter "language_right"
// "words_left"delimiter "words_right
// ex:
// english: french 
// left: gauche
{
	Dictionary dictionary;
	string& first_language = dictionary.first_language;
	string& second_language = dictionary.second_language;
	vector<string>& words_left = dictionary.words_left;
	vector<string>& words_right = dictionary.words_right;

	fstream file(dictionary_filename);
	if(file.is_open()){
		// gets languages
		getline(file, first_language, file_delimiter);
		file >> ws; // removes whitespaces
		getline(file, second_language);

		// gets words and their translations
		for(string word; getline(file, word, file_delimiter);){
			if(!word.empty()) words_left.push_back(word);
			file >> ws;

			word.clear();
			getline(file, word);
			if(!word.empty()) words_right.push_back(word);
		}
		file.close();

		// checks if the dictionary is valid
		if(words_left.size() != words_right.size()) throw runtime_error("(retrieve words) size mismatch.");
	}
	return dictionary;
}

// creates file if it doesn't exit
void create_file_if(const string& file_address)
// create the file if it doesn't exit
{
    fstream file;
    file.open(file_address, ios_base::in);

    if (file.is_open()) file.close();
    else {
        file.open(file_address, ios_base::out);
        if(file.is_open()) file.close();
    }
}

// displays menu
void display_menu()
// displays main menu
// informs the user about available choices
{
	Dictionary dictionary = get_words_and_translations();
	const string& first_language = dictionary.first_language;
	const string& second_language = dictionary.second_language;

    cout << "[1] " << first_language << "-" << second_language << newline;
    cout << "[2] " << second_language << "-" << first_language << newline;
    cout << "[x] Exit" << newline;
}

// gets a random distribution of unique unsigned integers
vector<size_t> get_random_int_distribution(const size_t& size)
// returns a shuffled vector of unique integers ranging from 0 to size-1
{
    vector<size_t> indexes;
    for (size_t i = 0; i < size; indexes.push_back(i++));

    shuffle_elements(indexes.begin(), indexes.end());

    return indexes;
}

// gets user's answer
string get_answer()
// gets the user's answer
{
	string answer;
	getline(cin, answer);
    return answer;
}

// gets the words to be translated
const vector<string>& get_words_left(const Dictionary& dictionary, const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::normal:
		return dictionary.words_left;

	case Dictionary::Mode::reverse:
		return dictionary.words_right;
	}

	return dictionary.words_left;
}

// gets the translations
const vector<string>& get_words_right(const Dictionary& dictionary, const Dictionary::Mode& mode){
    switch(mode){
    case Dictionary::Mode::normal:
        return dictionary.words_right;

    case Dictionary::Mode::reverse:
        return dictionary.words_left;
    }

	return dictionary.words_right;
}

// gets length of a word
size_t get_length(const string& str)
// returns the length of a word
// covers almost all latin-script alphabets
{
    if(str.empty()) return 0;
    size_t length = 0;
    size_t count = 0; // keeps track of the special characters

    for(const char& c: str){
        if(int(c) < 0 || int(c) > 127) ++count;
        else ++length;
    }
    if(count%2) throw runtime_error("(get_length) expected a 2 bytes character.");
    return length + count/2;
}

// quiz launcher
size_t quiz_launcher(const Dictionary& dictionary, const Dictionary::Mode& mode)
// displays a word, wait for the user's answer,
// if the answer is wrong, displays the right answer
{
	// keeps track of the score
	size_t score { 0 };

    // retrieves dictionary information from file
    const vector<string>& words_left = get_words_left(dictionary, mode);
    const vector<string>& words_right = get_words_right(dictionary, mode);

	const size_t size = words_left.size();
	const vector<size_t> indexes = get_random_int_distribution(size);
	size_t indexes_size = indexes.size();

	for(size_t position = 0; position < indexes_size; ++position){
		const string& word = words_left[indexes[position]];
		cout << word << ": ";
		
		// gets user's answer
		string user_answer = get_answer();
		const string& right_answer = words_right[indexes[position]];

		// checks if the word should be removed from the practice list
		if(user_answer == right_answer){
			// removes the word index from the practice list if present
        	// vector<size_t>::iterator it = find(retry_indexes.begin(), retry_indexes.end(), indexes[position]);
        	// if (it != retry_indexes.end()) retry_indexes.erase(it);
			++score;
		}
		else {
			// displaying right answer
			const size_t length = get_length(word);
			for(size_t i = 0; i < length + 2; ++i) cout << whitespace;
			cout << "\033[33m" << right_answer << newline << "\033[0m";
		}
	}
	return score;
}
