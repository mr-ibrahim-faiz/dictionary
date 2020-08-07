#ifndef DICTIONARY_H
#define DICTIONARY_H

#include<string>
using std::string;

#include<vector>
using std::vector;

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

#endif