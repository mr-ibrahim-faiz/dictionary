#include "dictionary.h"

#include<stdexcept>
using std::runtime_error;

int main()
try
{
	// sets required files
	set_required_files();

	// retrieves dictionary information from file
	Dictionary dictionary = get_dictionary();

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
		if (choice.length() != 1) choice = INVALID_CHOICE;

		char& user_choice = choice[0];

		// takes care of the case where choice '3' is not available while the choice '4' is
		// this is needed since the user has to enter '3' to select choice '4'
		if(indexes_left.empty() && !indexes_right.empty()){
			switch(user_choice){
			case '3':
				user_choice = '4';
				break;

			case '4':
				choice = INVALID_CHOICE;
				break;

			default:
				break;
			}
		}
		
		Dictionary::Mode mode = Dictionary::Mode(int(user_choice-'0')-1);
		Dictionary::Mode mode_resume = Dictionary::Mode(int(mode)+4);
		const vector<string>& words = get_words(dictionary, mode);
		const vector<size_t>& indexes = get_indexes_practice(practice, mode);
		size_t position = get_position(resume, mode_resume);

		switch (user_choice) {
		case '1': case '2':
		{
			cout << newline;
			if(!words.empty()){
				if(position == INVALID_POSITION) quiz_launcher(dictionary, practice, resume, mode);
				else quiz_launcher(dictionary, practice, resume, mode_resume);
			}
			else cout << "There's not a single word to display.\n";	
		}
			break;

		case '3': case '4':
		{
            cout << newline;
			if(!indexes.empty()){
				if(!words.empty()) quiz_launcher(dictionary, practice, resume, mode);
				else cout << "There's not a single word to display.\n";
			}
			else cout << "Please enter a valid choice.\n";
		}
            break;

		case exit_character:
			break;

		default:
			cout << "\nPlease enter a valid choice.\n";
			break;
		}

		if (user_choice == exit_character) break;
		else {
			// retrieves dictionary information from file
			dictionary = get_dictionary();

			// retrieves resume information from file
			resume = get_resume();

			// retrieves practice information from file
			practice = get_practice();

			// updates practice file
			practice = update_practice(practice, resume, dictionary);
			update_practice_file(practice);

			// updates resume file
			resume = update_resume(resume, dictionary);
			update_resume_file(resume);

			cout << newline;
			display_menu(dictionary, practice, resume);
		}
	}

	cout << "\nGoodbye !\n";

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
