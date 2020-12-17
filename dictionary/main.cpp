#include "dictionary_launcher.h"

#include<stdexcept>
using std::runtime_error;

// symbolic names
constexpr char updater_choice { 'u' };

// messages
const string message_empty_list { "There's not a single word to display." };
const string message_invalid_choice { "Please enter a valid choice." };
const string message_exit { "Goodbye !" };

int main()
try
{
	// sets required files
	set_required_files();

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

	const vector<size_t>& indexes_left = practice.indexes_left;
	const vector<size_t>& indexes_right = practice.indexes_right;

	// displays main menu
	display_menu(dictionary, practice, resume);

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if(choice.length() != valid_choice_length) choice = invalid_choice;

		char& user_choice = choice[0];

		// takes care of the case where choice '3' is not available while the choice '4' is
		// this is needed since the user has to enter '3' to select choice '4'
		if(indexes_left.empty() && !indexes_right.empty()){
			switch(user_choice){
			case '3':
				user_choice = '4';
				break;

			case '4':
				choice = invalid_choice;
				break;

			default:
				break;
			}
		}
		
		Dictionary::Mode mode = Dictionary::Mode(int(user_choice-'0')-1);
		Dictionary::Mode mode_resume = Dictionary::Mode(int(mode)+4);
		const vector<string>& words = get_words(dictionary, mode);
		const vector<size_t>& indexes = get_indexes_practice(practice, mode);
		size_t position = get_position(resume, practice, mode_resume);

		// retrieves ignored words
		ignored_words = get_ignored_words(statistics, mode);

		// clears screen
		if(user_choice != exit_choice) [[maybe_unused]] int result = system(clear_command.c_str());

		switch(user_choice){
		case '1': case '2':
		{
			if(!words.empty()){
				if(ignored_words.size() == words.size()) cout << "Well done. The Success threshold has been exceeded!\n";
				if(position == invalid_position) quiz_launcher(dictionary, practice, resume, mode);
				else quiz_launcher(dictionary, practice, resume, mode_resume);
			}
			else cout << message_empty_list << newline;
			cout << newline;
		}
			break;

		case '3': case '4':
		{
			if(!indexes.empty()){
				if(!words.empty()){
					if(position == invalid_position) quiz_launcher(dictionary, practice, resume, mode);
					else quiz_launcher(dictionary, practice, resume, mode_resume);
				}
				else cout << message_empty_list << newline;
			}
			else cout << message_invalid_choice << newline;
			cout << newline;
		}
            break;

		case updater_choice:
		{
			// launches statistics updater
			statistics_updater();

			// clears screen
			[[maybe_unused]] int result = system(clear_command.c_str());
		}
			break;

		case exit_choice:
			break;

		default:
			cout << message_invalid_choice << "\n\n";
			break;
		}

		if(user_choice == exit_choice) break;
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

			display_menu(dictionary, practice, resume);
		}
	}

	cout << newline;
	cout << message_exit << newline;

	return 0;
}
catch (runtime_error& e) {
	cerr << "Error: " << e.what() << newline;
	return 1;
}
catch (...) {
	cerr << "Error: unknown exception caught." << newline;
	return 2;
}
