#ifndef RESUME_H
#define RESUME_H

#include "dictionary.h"

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

#endif