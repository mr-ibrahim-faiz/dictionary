#ifndef STATISTICS_H
#define STATISTICS_H

#include<iostream>
using std::pair;

#include<vector>
using std::vector;

// user-defined type Statistics
struct Statistics {
	// default constructor
	Statistics() noexcept
		: successes {}
		, failures {}{}

	// constructor taking number of successes and failures as arguments
	explicit Statistics(const vector<size_t>& successes, const vector<size_t>& failures) noexcept
		: successes { successes }
		, failures { failures }{}

	// statistics information
	vector<size_t> successes; // number of times words were well translated
	vector<size_t> failures; // number of times words were not well translated
};

#endif
