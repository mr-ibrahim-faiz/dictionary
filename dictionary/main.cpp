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

	// displays main menu
	display_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != 1) choice = INVALID_CHOICE;
		else {
			// retrieves dictionary information from file
			dictionary = get_words_and_translations();

			// retrieves practice information from file
			practice = get_practice_information();
		}

		switch (choice[0]) {
		case '1':
			cout << newline;
			if(!words_left.empty() && words_left.size() == words_right.size()){
				quiz_launcher(dictionary, practice, Dictionary::Mode::normal);
			}
			else {
				if(words_left.empty()) cout << "There's not a single word to display.\n";
				else cout << "The number of words doesn't match.\n";
			}
			break;

		case '2':
            cout << newline;
            if(!words_left.empty() && words_left.size() == words_right.size()){
                quiz_launcher(dictionary, practice, Dictionary::Mode::reverse);
            }
            else {
                if(words_left.empty()) cout << "There's not a single word to display.\n";
                else cout << "The number of words doesn't match.\n";
            }
			break;

        case '3':
            cout << newline;
            if(!words_left.empty() && words_left.size() == words_right.size()){
				if(!practice.indexes_left.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::practice_normal);
				else cout << "The practice list is empty.\n";
            }
            else {
                if(words_left.empty()) cout << "There's not a single word to display.\n";
                else cout << "The number of words doesn't match.\n";
            }
            break;

        case '4':
            cout << newline;
            if(!words_left.empty() && words_left.size() == words_right.size()){
                if(!practice.indexes_right.empty()) quiz_launcher(dictionary, practice, Dictionary::Mode::practice_reverse);
				else cout << "The practice list is empty.\n";
            }
            else {
                if(words_left.empty()) cout << "There's not a single word to display.\n";
                else cout << "The number of words doesn't match.\n";
            }
            break;

		case exit_character:
			break;

		default:
			cout << "\nPlease enter a valid choice.\n";
			break;
		}

		if (choice[0] == exit_character)
			break;
		else {
			cout << '\n';
			display_menu();
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
