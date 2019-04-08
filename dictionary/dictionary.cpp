#include "dictionary.h"

#include<iostream>
using std::ws;
using std::streamsize;

#include<limits>
using std::numeric_limits;

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

// retrieves practice information from file
Practice get_practice_information()
// retrieves the indexes of questions to retry
// Note: first retrieves the 'left_to_right' indexes then the 'right_to_left' indexes
// syntax:
// idx idx idx idx delimiter
// idx idx idx idx delimiter
{
	Practice practice;
	vector<size_t>& indexes_left = practice.indexes_left;
	vector<size_t>& indexes_right = practice.indexes_right;

	fstream file(practice_filename);
	if(file.is_open()){
		// gets indexes
		for(size_t index = 0; file >> index; indexes_left.push_back(index));
		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		for(size_t index = 0; file >> index; indexes_right.push_back(index));
		file.clear();
        file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		file.close();
	}
	return practice;
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

// sets practice file
void set_practice_file(){
    fstream file;
    file.open(practice_filename, ios_base::in);

    if (file.is_open()) file.close();
    else {
        file.open(practice_filename, ios_base::out);
        if(file.is_open()){
			file << " $\n $";
			file.close();
		}
    }
}

// updates practice file
void update_practice_file(const Practice& practice){
	const vector<size_t>& indexes_left = practice.indexes_left;
	const vector<size_t>& indexes_right = practice.indexes_right;

	// saves indexes
	write_elements(indexes_left.begin(), indexes_left.end(), practice_filename, " ", " $\n");
	write_elements(indexes_right.begin(), indexes_right.end(), practice_filename, " ", " $", ios_base::app);
}

// displays menu
void display_menu(const Dictionary& dictionary, const Practice& practice)
// displays main menu
// informs the user about available choices
{
	const string& first_language = dictionary.first_language;
	const string& second_language = dictionary.second_language;
	const size_t& number_of_questions_left = practice.indexes_left.size();
	const size_t& number_of_questions_right = practice.indexes_right.size();

	cout << "[1] " << first_language << "-" << second_language << newline;
	cout << "[2] " << second_language << "-" << first_language << newline;
	if(number_of_questions_left > 0) 
		cout << "[3] Practice " << first_language << "-" << second_language << " (" << number_of_questions_left << ")" << newline;
	if(number_of_questions_right > 0) 
		cout << ((number_of_questions_left == 0)? "[3]":"[4]") << " Practice " << second_language << "-" 
				<< first_language << " (" << number_of_questions_right << ")" << newline;
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
	case Dictionary::Mode::normal: case Dictionary::Mode::practice_normal:
		return dictionary.words_left;

	case Dictionary::Mode::reverse: case Dictionary::Mode::practice_reverse:
		return dictionary.words_right;
	}

	return dictionary.words_left;
}

// gets the translations
const vector<string>& get_words_right(const Dictionary& dictionary, const Dictionary::Mode& mode){
    switch(mode){
	case Dictionary::Mode::normal: case Dictionary::Mode::practice_normal:
        return dictionary.words_right;

	case Dictionary::Mode::reverse: case Dictionary::Mode::practice_reverse:
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

// gets indexes
vector<size_t> get_indexes(const Dictionary& dictionary, const Practice& practice, const Dictionary::Mode& mode)
// gets indexes based on the mode
{
	vector<size_t> indexes;

	switch(mode){
		case Dictionary::Mode::normal: case Dictionary::Mode::reverse:
		indexes = get_random_int_distribution(dictionary.words_left.size());
		break;

	case Dictionary::Mode::practice_normal:
		indexes = practice.indexes_left;
		break;

	case Dictionary::Mode::practice_reverse:
		indexes = practice.indexes_right;
		break;
	}

	return indexes;
}

// quiz launcher
Practice quiz_launcher(const Dictionary& dictionary, const Practice& practice, const Dictionary::Mode& mode)
// displays a word, wait for the user's answer,
// if the answer is wrong, displays the right answer
{
	//// keeps track of the score
	// size_t score { 0 };

	Practice practice_updated = practice;
	vector<size_t>& indexes_left = practice_updated.indexes_left;
	vector<size_t>& indexes_right = practice_updated.indexes_right;

    // retrieves dictionary information from file
    const vector<string>& words_left = get_words_left(dictionary, mode);
    const vector<string>& words_right = get_words_right(dictionary, mode);

	const vector<size_t> indexes = get_indexes(dictionary, practice, mode);
	size_t indexes_size = indexes.size();

	for(size_t position = 0; position < indexes_size; ++position){
		// calls practice mode if necessary
		switch(mode){
		case Dictionary::Mode::normal:
			if(indexes_left.size() >= minimum_number_of_words){
				cout << ((position != 0)? "\n" : "") << "[Practice]\n\n";
				practice_updated = quiz_launcher(dictionary, practice_updated, Dictionary::Mode::practice_normal);
				cout << "\n[Quiz]\n\n";
			}
			break;

        case Dictionary::Mode::reverse:
            if(indexes_right.size() >= minimum_number_of_words){
                cout << ((position != 0)? "\n" : "") << "[Practice]\n\n";
                practice_updated = quiz_launcher(dictionary, practice_updated, Dictionary::Mode::practice_reverse);
                cout << "\n[Quiz]\n\n";
            }
            break;

		default:
			break;
		}

		const size_t& index = indexes[position];
		const string& word = words_left[index];
		cout << word << ": ";
		
		// gets user's answer
		string user_answer = get_answer();
		const string& right_answer = words_right[index];

		// checks if the word should be removed from the practice list
		if(user_answer == right_answer){
			//++score;

			// removes the word index from the practice list if present
			switch(mode){
			case Dictionary::Mode::normal: case Dictionary::Mode::practice_normal:
			{
				vector<size_t>::iterator it = find(indexes_left.begin(), indexes_left.end(), index);
				if (it != indexes_left.end()) indexes_left.erase(it);
			}
				break;

			case Dictionary::Mode::reverse: case Dictionary::Mode::practice_reverse:
			{
				vector<size_t>::iterator it = find(indexes_right.begin(), indexes_right.end(), index);
				if (it != indexes_right.end()) indexes_right.erase(it);
			}
				break;

			default:
				break;
			}
		}
		else {
			// updates practice indexes
			switch(mode){
			case Dictionary::Mode::normal:
			{
                vector<size_t>::iterator it = find(indexes_left.begin(), indexes_left.end(), index);
                if (it == indexes_left.end()) indexes_left.push_back(index);
			}
				break;
			
			case Dictionary::Mode::reverse:
			{
                vector<size_t>::iterator it = find(indexes_right.begin(), indexes_right.end(), index);
                if (it == indexes_right.end()) indexes_right.push_back(index);
			}
				break;

			default:
				break;
			}

			// displaying right answer
			const size_t length = get_length(word);
			for(size_t i = 0; i < length + 2; ++i) cout << whitespace;
			cout << "\033[33m" << right_answer << newline << "\033[0m";
		}

		// updates practice file
		update_practice_file(practice_updated);
	}
	return practice_updated;
}
