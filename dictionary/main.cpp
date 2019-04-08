#include "dictionary.h"

#include<stdexcept>
using std::runtime_error;

int main()
try
{
	// creates required files
	create_file_if(dictionary_filename);
	set_practice_file();

	// retrieves dictionary information from file
	Dictionary dictionary = get_words_and_translations();
	vector<string>& words_left =  dictionary.words_left;
	vector<string>& words_right =  dictionary.words_right;

	// retrieves practice information from file
	Practice practice = get_practice_information();
	vector<size_t>& indexes_left = practice.indexes_left;
	vector<size_t>& indexes_right = practice.indexes_right;

	// displays main menu
	display_menu(dictionary, practice);

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != 1) choice = INVALID_CHOICE;

		char& user_choice = choice[0];
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

		switch (user_choice) {
		case '1':
			cout << newline;
			if(!words_left.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::normal);
			else cout << "There's not a single word to display.\n";
			break;

		case '2':
            cout << newline;
            if(!words_right.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::reverse);
            else cout << "There's not a single word to display.\n";
			break;

        case '3':
            cout << newline;
			if(!indexes_left.empty()){
				if(!words_left.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::practice_normal);
				else cout << "There's not a single word to display.\n";
			}
			else cout << "Please enter a valid choice.\n";
            break;

        case '4':
            cout << newline;
            if(!indexes_right.empty()){
				if(!words_right.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::practice_reverse);
				else cout << "There's not a single word to display.\n";
            }
			else cout << "Please enter a valid choice.\n";
            break;

		case exit_character:
			break;

		default:
			cout << "\nPlease enter a valid choice.\n";
			break;
		}

		if (user_choice == exit_character)
			break;
		else {
			// retrieves dictionary information from file
			dictionary = get_words_and_translations();

			// retrieves practice information from file
			practice = get_practice_information();

			cout << '\n';
			display_menu(dictionary, practice);
		}
	}

	cout << "\nGoodbye !\n";

	return 0;
}
catch (runtime_error& e) {
	cerr << "Error: " << e.what() << '\n';
	return 1;
}
catch (...) {
	cerr << "Error: unknown exception caught." << '\n';
	return 2;
}
