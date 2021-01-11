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

	// removed words
	vector<size_t> removed;

	// modes
	enum class Mode {
		normal, reverse, practice_normal, practice_reverse, normal_resume, reverse_resume, practice_normal_resume, practice_reverse_resume
	};
};

#endif