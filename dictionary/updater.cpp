#include "updater.h"
#include "dictionary_launcher.h"

#include<iostream>
using std::cin;
using std::cout;

// messages
const string message_list_cleared { "The ignored words list has been cleared." };
const string message_invalid_choice { "Please enter a valid choice." };

// displays updater menu
void display_updater_menu(){
	// retrieves dictionary information from file
	Dictionary dictionary = get_dictionary();
	const string& first_language = dictionary.first_language;
	const string& second_language = dictionary.second_language;

	cout << "[1] Include ignored words " << first_language << "-" << second_language << newline;
	cout << "[2] Include ignored words " << second_language << "-" << first_language << newline;
	cout << "[x] Exit\n";
}

// updater
void updater(){
	// retrieves dictionary information from file
	Dictionary dictionary = get_dictionary();

	// retrieves statistics information from file
	Statistics statistics = get_statistics();
	statistics = update_statistics(statistics, dictionary);
	update_statistics_file(statistics);

	vector<size_t> ignored_words;

	// retrieves resume information from file
	Resume resume = get_resume();

	// retrieves practice information from file
	Practice practice = get_practice();

	// updates practice file
	practice = update_practice(practice, resume, dictionary);
	update_practice_file(practice);

	// updates resume file
	resume = update_resume(resume, dictionary);
	update_resume_file(resume);

	display_updater_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		const char& user_choice = choice[0];

		// retrieves mode
		Dictionary::Mode mode = Dictionary::Mode(int(user_choice-'0')-1);

		if (choice.length() != valid_choice_length) choice = invalid_choice;
		else {
			// retrieves dictionary information from file
			dictionary = get_dictionary();

			// retrieves statistics information from file
			statistics = get_statistics();

			// retrieves resume information from file
			resume = get_resume();

			// retrieves practice information from file
			practice = get_practice();

			// updates statistics file
			statistics = update_statistics(statistics, dictionary);
			update_statistics_file(statistics);

			// updates practice file
			practice = update_practice(practice, resume, dictionary);
			update_practice_file(practice);

			// updates resume file
			resume = update_resume(resume, dictionary);
			update_resume_file(resume);

			// retrieves ignored words
			ignored_words = get_ignored_words(statistics, mode);
		}

		// clears screen
		if(user_choice != exit_choice) [[maybe_unused]] int result = system(clear_command.c_str());

		switch (user_choice) {
		case '1': case '2':
			// clears the ignored words list by reducing the success counts.
			while(ignored_words.size()){
				vector<pair<size_t, size_t>>& successes = statistics.successes;
				if(user_choice == '1'){
					for(const size_t& index: ignored_words){
						pair<size_t, size_t>& success = successes[index];
						success.first = success.first - 1;
					}
				}
				else {
					for(const size_t& index: ignored_words){
						pair<size_t, size_t>& success = successes[index];
						success.second = success.second - 1;
					}
				}

				// retrieves ignored words
				ignored_words = get_ignored_words(statistics, mode);
			}
			update_statistics_file(statistics);
			cout << message_list_cleared << newline;
			cout << newline;
			break;

		case exit_choice:
			break;

		default:
			cout << message_invalid_choice << newline;
			cout << newline;
			break;
		}

		if (user_choice == exit_choice) break;
		else display_updater_menu();
	}
}
