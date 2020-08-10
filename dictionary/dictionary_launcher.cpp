#include "dictionary_launcher.h"

#include<iostream>
using std::ws;
using std::streamsize;

#include<stdexcept>
using std::runtime_error;

#include<fstream>
using std::fstream;

// symbolic names
constexpr char whitespace { ' ' };
constexpr char delimiter_dictionary { ':' };
constexpr char end_of_line { '$' };

constexpr size_t minimum_number_of_words { 10 }; // minimum number of words that triggers practice mmode
constexpr size_t initial_position { 0 };

const string exit_sequence { ":exit" };

// file names
const string dictionary_filename { "dictionary.txt" };
const string practice_filename { "practice.txt" };
const string resume_filename { "resume.txt" };

// gets delimiter
string get_delimiter_file()
// gets practice and resume delimiter
{
	string delimiter;
	delimiter.push_back(whitespace);
	return delimiter;
}

// gets in-between period
string get_period_file()
// gets practice and resume in-between period
{
	string period = get_end_period_file();
	period.push_back(newline);
	return period;
}

// gets end period
string get_end_period_file()
// gets practice and resume end period
{
	string period;
	period.push_back(whitespace);
	period.push_back(end_of_line);
	return period;
}

// updates position
void Resume::update_position(const size_t& position, const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::normal: case Dictionary::Mode::normal_resume: case Dictionary::Mode::practice_normal: case Dictionary::Mode::practice_normal_resume:
		position_left = position;
		break;

	case Dictionary::Mode::reverse: case Dictionary::Mode::reverse_resume: case Dictionary::Mode::practice_reverse: case Dictionary::Mode::practice_reverse_resume:
		position_right = position;
		break;

	default:
		break;
	}
}

// updates indexes
void Resume::update_indexes(const vector<size_t>& indexes, const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::normal:
		indexes_left = indexes;
		break;

	case Dictionary::Mode::reverse:
		indexes_right = indexes;
		break;

	case Dictionary::Mode::practice_normal:
		indexes_left = indexes;
		break;

	case Dictionary::Mode::practice_reverse:
		indexes_right = indexes;
		break;

	default:
		break;
	}
}

// checks if valid
bool Resume::is_valid(){
	if((position_left != invalid_position) && position_left >= indexes_left.size()) return false;
	if((position_right != invalid_position) && position_right >= indexes_right.size()) return false;	
	return true;
}

// retrieves dictionary information from file
Dictionary get_dictionary()
// first retrieves the languages then the words and their translations
// syntax: 
// "first_language"delimiter "seconde_language"
// "words_left"delimiter "words_right
// ex:
// (delimiter==:)
// english: french 
// left: gauche
{
	Dictionary dictionary;
	string& first_language = dictionary.first_language;
	string& second_language = dictionary.second_language;
	vector<string>& words_left = dictionary.words_left;
	vector<string>& words_right = dictionary.words_right;

	fstream file(dictionary_filename, ios_base::in | ios_base::binary);
	if(file.is_open()){
		// gets languages
		getline(file, first_language, delimiter_dictionary);
		file >> ws; // removes whitespaces
		getline(file, second_language);

		// gets words and their translations
		for(string word; getline(file, word, delimiter_dictionary);){
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
Practice get_practice()
// retrieves the indexes of questions to practice
// Note: first retrieves the 'left_to_right' indexes then the 'right_to_left' indexes
// syntax:
// (delimiter==$)
// position_left $
// position_right $
// idx idx... $
// idx idx... $
{
	Practice practice;
	size_t& position_left = practice.position_left;
	size_t& position_right = practice.position_right;
	vector<size_t>& indexes_left = practice.indexes_left;
	vector<size_t>& indexes_right = practice.indexes_right;

	fstream file(practice_filename, ios_base::in | ios_base::binary);
	if(file.is_open()){
		// retrieves current positions
		if(file >> position_left){}
		else position_left = invalid_position;

		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		if(file >> position_right){}
		else position_right = invalid_position;

		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		// gets indexes
		for (size_t index = 0; file >> index; indexes_left.push_back(index));
		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		for (size_t index = 0; file >> index; indexes_right.push_back(index));
		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		file.close();
	}

	// checks if practice is valid
	if(!practice.is_valid()) throw runtime_error("(retrieves practice) inconsistent data.");

	return practice;
}

// retrieves resume information from file
Resume get_resume()
// retrieves positions and indexes of questions to resume
// Note: first retrieves the positions then the indexes
// syntax:
// (delimiter==$)
// position_left $
// position_right $
// idx idx... $
// idx idx... $
{
	Resume resume;
	size_t& position_left = resume.position_left;
	size_t& position_right = resume.position_right;
	vector<size_t>& indexes_left = resume.indexes_left;  
	vector<size_t>& indexes_right = resume.indexes_right;  

	fstream file(resume_filename, ios_base::in | ios_base::binary);
	if(file.is_open()){
		// retrieves current positions
		if(file >> position_left){}
		else position_left = invalid_position;

		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		if(file >> position_right){}
		else position_right = invalid_position;

		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		// gets indexes
		for(size_t index = 0; file >> index; indexes_left.push_back(index));
		file.clear();
		file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		for(size_t index = 0; file >> index; indexes_right.push_back(index));
		file.clear();
        file.ignore(numeric_limits<streamsize>::max(), end_of_line);

		file.close();
	}
	
	// checks if resume is valid
	if(!resume.is_valid()) throw runtime_error("(retrieves resume) inconsistent data.");

	return resume;
}

// sets required files
void set_required_files()
{
	set_dictionary_file();
	set_practice_file();
	set_resume_file();
}

// sets dictionary file
void set_dictionary_file(){
	fstream file;
	file.open(dictionary_filename, ios_base::in | ios_base::binary);

	if(file.is_open()) file.close();
	else{
		file.open(dictionary_filename, ios_base::out | ios_base::binary);
		if(file.is_open()){
			file << "English: French\n";
			file << "worthwhile: digne d'intérêt\n";
			file << "passing fad: mode passagère\n";
			file << "bewildering: déconcertant\n";
			file.close();
		}
	}
}

// sets practice file
void set_practice_file(){
	set_file(practice_filename, 4);
}

// sets resume file
void set_resume_file(){
	set_file(resume_filename, 4);
}

// sets file
void set_file(const string& filename, const size_t& number_of_lines)
// used to set practice or resume file
// practice file contains 2 lines
// resume file contains 4 lines
{
    fstream file;
    file.open(filename, ios_base::in | ios_base::binary);

    if(file.is_open()) file.close();
    else{
        file.open(filename, ios_base::out | ios_base::binary);
        if(file.is_open()){
			for(size_t i = 0; i < number_of_lines; ++i) file << ((i != number_of_lines - 1)? period_file : end_period_file);
			file.close();
		}
    }
}

// updates the practice data
Practice update_practice(const Practice& practice, const Resume& resume, const Dictionary& dictionary){
	Practice updated_practice;
	updated_practice.position_left = practice.position_left;
	updated_practice.position_right = practice.position_right;

	vector<size_t> indexes_left = practice.indexes_left;
	vector<size_t> indexes_right = practice.indexes_right;

	// adds newly added words
	const size_t dictionary_size = dictionary.words_left.size();
	const size_t indexes_left_size = resume.indexes_left.size();
	const size_t indexes_right_size = resume.indexes_right.size();

	if(!resume.indexes_left.empty() && (dictionary_size > indexes_left_size)){
		const size_t new_words = dictionary_size - indexes_left_size;
		for (size_t i { 0 }; i < new_words; ++i){
			indexes_left.push_back(indexes_left_size + i);
		}
	}

	if(!resume.indexes_right.empty() && (dictionary_size > indexes_right_size)){
		const size_t new_words = dictionary_size - indexes_right_size;
		for (size_t i { 0 }; i < new_words; ++i){
			indexes_right.push_back(indexes_right_size + i);
		}
	}

	updated_practice.indexes_left = indexes_left;
	updated_practice.indexes_right = indexes_right;

	return updated_practice;
}

// updates the resume data
Resume update_resume(const Resume& resume, const Dictionary& dictionary){
	Resume updated_resume;
	updated_resume.position_left = resume.position_left;
	updated_resume.position_right = resume.position_right;

	vector<size_t> indexes_left = resume.indexes_left;
	vector<size_t> indexes_right = resume.indexes_right;

	// adds newly added words
	const size_t dictionary_size = dictionary.words_left.size();
	const size_t indexes_left_size = indexes_left.size();
	const size_t indexes_right_size = indexes_right.size();

	if(!indexes_left.empty() && (dictionary_size > indexes_left_size)){
		const size_t new_words = dictionary_size - indexes_left_size;
		for (size_t i { 0 }; i < new_words; ++i){
			indexes_left.push_back(indexes_left_size + i);
		}
	}

	if(!indexes_right.empty() && (dictionary_size > indexes_right_size)){
		const size_t new_words = dictionary_size - indexes_right_size;
		for (size_t i { 0 }; i < new_words; ++i){
			indexes_right.push_back(indexes_right_size + i);
		}
	}

	updated_resume.indexes_left = indexes_left;
	updated_resume.indexes_right = indexes_right;

	return updated_resume;
}

// updates practice file
void update_practice_file(const Practice& practice){
	// gets positions and indexes
	const size_t& position_left = practice.position_left;
	const size_t& position_right = practice.position_right;
	const vector<size_t>& indexes_left = practice.indexes_left;
	const vector<size_t>& indexes_right = practice.indexes_right;

	// saves positions
	write_single_element(position_left, practice_filename, period_file);
	write_single_element(position_right, practice_filename, period_file, ios_base::app | ios_base::binary);

	// saves indexes
	write_elements(indexes_left.begin(), indexes_left.end(), practice_filename, delimiter_file, period_file, ios_base::app | ios_base::binary);
	write_elements(indexes_right.begin(), indexes_right.end(), practice_filename, delimiter_file, end_period_file, ios_base::app | ios_base::binary);
}

// updates resume file
void update_resume_file(const Resume& resume){
	// gets positions and indexes
	const size_t& position_left = resume.position_left;
	const size_t& position_right = resume.position_right;

	const vector<size_t>& indexes_left = resume.indexes_left;
	const vector<size_t>& indexes_right = resume.indexes_right;

	// saves positions
	write_single_element(position_left, resume_filename, period_file);
	write_single_element(position_right, resume_filename, period_file, ios_base::app | ios_base::binary);
	
	// saves indexes
	write_elements(indexes_left.begin(), indexes_left.end(), resume_filename, delimiter_file, period_file, ios_base::app | ios_base::binary);
	write_elements(indexes_right.begin(), indexes_right.end(), resume_filename, delimiter_file, end_period_file, ios_base::app | ios_base::binary);
}

// writes a single element on a file
template<typename T>
void write_single_element(const T& t, const string& filename, const string& period, const ios_base::openmode& mode)
// writes a single element on filename
// the writing is ended by the period
{
	fstream file;
	file.open(filename, mode);

	if(file.is_open()){
		file << t << period;
		file.close();
	}
	else
		cerr << "Error: Unable to open file.\n";
}

// writes elements of a container on a file
template<typename InputIterator>
void write_elements(InputIterator first, const InputIterator last, const string& filename, const string& delimiter, const string& period, const ios_base::openmode& mode)
// write elements of a container on file
// elements are delimited by the delimiter
// the writing is ended by the period
{
    fstream file;
    file.open(filename, mode);

    if(file.is_open()){
        if(first == last) file << period;
        else{
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
void display_menu(const Dictionary& dictionary, const Practice& practice, const Resume& resume)
// displays main menu
// informs the user about available choices
{
	const string& first_language = dictionary.first_language;
	const string& second_language = dictionary.second_language;
	const size_t& number_of_questions_left = practice.indexes_left.size();
	const size_t& number_of_questions_right = practice.indexes_right.size();

	const size_t& position_left_resume = resume.position_left;
	const size_t& position_right_resume = resume.position_right;

	const size_t& position_left_practice = practice.position_left;
	const size_t& position_right_practice = practice.position_right;

	cout << "[1] " << ((position_left_resume != invalid_position)? "Resume " : "") << first_language << "-" << second_language << newline;
	cout << "[2] " << ((position_right_resume != invalid_position)? "Resume " : "") << second_language << "-" << first_language << newline;
	if(number_of_questions_left > 0){
		cout << "[3] " << ((position_left_practice != invalid_position) ? "Resume " : "");
		cout << "Practice " << first_language << "-" << second_language << " (" << number_of_questions_left << ")" << newline;
	}
	if(number_of_questions_right > 0){
		cout << ((number_of_questions_left == 0) ? "[3] " : "[4] ") << ((position_right_practice != invalid_position) ? "Resume " : "");
		cout << "Practice " << second_language << "-" << first_language << " (" << number_of_questions_right << ")" << newline;
	}
	cout << "[x] Exit" << newline;
}

// gets a distribution of unique unsigned integers
vector<size_t> get_int_distribution(const size_t& size)
// returns a vector of unique integers ranging from 0 to size-1
{
    vector<size_t> indexes;
    for (size_t i = 0; i < size; indexes.push_back(i++));

    return indexes;
}

// gets user's answer
string get_answer()
{
	string answer;
	getline(cin, answer);
    return answer;
}

// gets words
vector<string>& get_words(Dictionary& dictionary, const Dictionary::Mode& mode){
	if(is_normal_mode(mode)) return dictionary.words_left;
	return dictionary.words_right;
}

// gets the words to be translated
const vector<string>& get_words_left(const Dictionary& dictionary, const Dictionary::Mode& mode){
	if(is_normal_mode(mode)) return dictionary.words_left;
	return dictionary.words_right;
}

// gets the translations
const vector<string>& get_words_right(const Dictionary& dictionary, const Dictionary::Mode& mode){
	if(is_normal_mode(mode)) return dictionary.words_right;
	return dictionary.words_left;
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

// gets position
size_t get_position(const Resume& resume, const Practice& practice, const Dictionary::Mode& mode)
// gets position based on the mode
{
	size_t position = initial_position;
	
	switch(mode){
	case Dictionary::Mode::normal_resume:
		position = resume.position_left;
		break;

	case Dictionary::Mode::reverse_resume:
		position = resume.position_right;
		break;

	case Dictionary::Mode::practice_normal_resume:
		position = practice.position_left;
		break;

	case Dictionary::Mode::practice_reverse_resume:
		position = practice.position_right;
		break;

	default:
		break;
	}

	return position;
}	

// gets indexes
vector<size_t> get_indexes(const Dictionary& dictionary, const Practice& practice, const Resume& resume, const Dictionary::Mode& mode)
// gets indexes based on the mode
// Note: the indexes are shuffled based on the mode
{
	vector<size_t> indexes;

	switch(mode){
	case Dictionary::Mode::normal: case Dictionary::Mode::reverse:
		indexes = get_int_distribution(dictionary.words_left.size());
		break;

	case Dictionary::Mode::practice_normal:
		indexes = practice.indexes_left;
		break;

	case Dictionary::Mode::practice_reverse:
		indexes = practice.indexes_right;
		break;

	case Dictionary::Mode::normal_resume:
		indexes = resume.indexes_left;
		break;

	case Dictionary::Mode::reverse_resume:
		indexes = resume.indexes_right;
		break;

	case Dictionary::Mode::practice_normal_resume:
		indexes = practice.indexes_left;
		break;

	case Dictionary::Mode::practice_reverse_resume:
		indexes = practice.indexes_right;
		break;
	}

	if(!is_resume_mode(mode)) shuffle_elements(indexes.begin(), indexes.end());
	
	return indexes;
}

// checks if it is practice mode
bool is_practice_mode(const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::practice_normal: case Dictionary::Mode::practice_reverse:
	case Dictionary::Mode::practice_normal_resume: case Dictionary::Mode::practice_reverse_resume:
		return true;

	default:
		return false;
	}
}

// checks if it is resume mode
bool is_resume_mode(const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::normal_resume: case Dictionary::Mode::reverse_resume:
	case Dictionary::Mode::practice_normal_resume: case Dictionary::Mode::practice_reverse_resume:
		return true;

	default:
		return false;
	}
}

// checks if it is normal mode
bool is_normal_mode(const Dictionary::Mode& mode){
	switch(mode){
	case Dictionary::Mode::normal: case Dictionary::Mode::normal_resume:
	case Dictionary::Mode::practice_normal: case Dictionary::Mode::practice_normal_resume:
		return true;
	
	default:
		return false;
	}
}

// checks if it is reverse mode
bool is_reverse_mode(const Dictionary::Mode& mode){
	return !is_normal_mode(mode);
}

// gets indexes size
size_t get_indexes_size_practice(const Practice& practice, const Dictionary::Mode& mode){
	if(is_reverse_mode(mode)) return practice.indexes_right.size();
	return practice.indexes_left.size();
}

// gets mode practice
Dictionary::Mode get_mode_practice(const Practice& practice, const Dictionary::Mode& mode){
	Dictionary::Mode mode_resume = is_normal_mode(mode) ? Dictionary::Mode::practice_normal_resume : Dictionary::Mode::practice_reverse_resume;
	const Resume resume;
	const size_t& position = get_position(resume, practice, mode_resume);

	if(is_reverse_mode(mode)){
		if(position != invalid_position) return Dictionary::Mode::practice_reverse_resume;
		else return Dictionary::Mode::practice_reverse;
	}
	else{
		if(position != invalid_position) return Dictionary::Mode::practice_normal_resume;
		else return Dictionary::Mode::practice_normal;
	}
}

// gets indexes practice 
vector<size_t>& get_indexes_practice(Practice& practice, const Dictionary::Mode& mode){
	if(is_normal_mode(mode)) return practice.indexes_left;
	return practice.indexes_right;
}

// quiz launcher
Practice quiz_launcher(const Dictionary& dictionary, const Practice& practice, const Resume& resume, const Dictionary::Mode& mode)
// displays a word, wait for the user's answer,
// if the answer is wrong, displays the right answer
{
	// retrieves practice information
	Practice practice_updated = practice;
	vector<size_t>& indexes_practice = get_indexes_practice(practice_updated, mode);

    // retrieves dictionary information
    const vector<string>& words_left = get_words_left(dictionary, mode);
    const vector<string>& words_right = get_words_right(dictionary, mode);

	// retrieves resume information
	Resume resume_updated = resume;

	const vector<size_t> indexes = get_indexes(dictionary, practice, resume, mode);

	size_t indexes_size = indexes.size();
	size_t position = get_position(resume, practice, mode);
	size_t number_of_consecutive_words { 0 }; 

	// updates resume and practice files
	if(is_practice_mode(mode)){
		practice_updated.update_indexes(indexes, mode);
		update_practice_file(practice_updated);
	}
	else{
		resume_updated.update_indexes(indexes, mode);
		update_resume_file(resume_updated);
	}

	size_t indexes_size_practice = get_indexes_size_practice(practice_updated, mode);
	size_t factor = indexes_size_practice / minimum_number_of_words + 1;

	for(; position < indexes_size; ++position){
		// updates resume and practice files
		if(!is_practice_mode(mode)){
			resume_updated.update_position(position, mode);
			update_resume_file(resume_updated);
		}
		else{
			practice_updated.update_position(position, mode);
			update_practice_file(practice_updated);
		}

		// calls practice mode if necessary
		if(!is_practice_mode(mode)){
			indexes_size_practice = get_indexes_size_practice(practice_updated, mode); // updates practice indexes size

			if(indexes_size_practice >= (minimum_number_of_words*factor)){
				cout << ((position != initial_position)? "\n" : "") << "[Practice]\n\n";
				number_of_consecutive_words = 0;

				Dictionary::Mode mode_practice = get_mode_practice(practice_updated, mode);

				practice_updated = quiz_launcher(dictionary, practice_updated, resume_updated, mode_practice);
				cout << "\n[Quiz]\n\n";
			}

			factor = indexes_size_practice / minimum_number_of_words + 1; // updates factor
		}

		const size_t& index = indexes[position];
		const string& word = words_left[index];

		const size_t maximum_number_of_words = 2*minimum_number_of_words;
		bool must_add_newline = !(number_of_consecutive_words%maximum_number_of_words) && (number_of_consecutive_words != 0);
		++number_of_consecutive_words;

		cout << ((must_add_newline)? "\n" : "") << word << ": ";
		
		// gets user's answer
		string user_answer = get_answer();

		// quits program
		if(user_answer == exit_sequence) return practice_updated;

		const string& right_answer = words_right[index];

		// checks if the word should be removed from the practice list
		if(user_answer == right_answer){
			// removes the word index from the practice list if present
			vector<size_t>::iterator it = find(indexes_practice.begin(), indexes_practice.end(), index);
			if(it != indexes_practice.end()) indexes_practice.erase(it);
		}
		else{
			// updates practice indexes
			if(!is_practice_mode(mode)){
				vector<size_t>::iterator it = find(indexes_practice.begin(), indexes_practice.end(), index);
				if(it == indexes_practice.end()) indexes_practice.push_back(index);
			}

			// displays right answer
			const size_t length = get_length(word);
			for(size_t i = 0; i < length + 2; ++i) cout << whitespace;
			cout << "\033[33m" << right_answer << newline << "\033[0m";
		}

		// updates practice file
		update_practice_file(practice_updated);
	}

	// updates resume and practice files
	if(!is_practice_mode(mode)){
		resume_updated.update_position(invalid_position, mode);
		update_resume_file(resume_updated);
	}
	else{
		practice_updated.update_position(invalid_position, mode);
		update_practice_file(practice_updated);
	}

	return practice_updated;
}
