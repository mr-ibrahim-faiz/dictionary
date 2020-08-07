#ifndef PRACTICE_H
#define PRACTICE_H

#include<vector>
using std::vector;

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

#endif