#include "dictionary.h"

#include<stdexcept>
using std::runtime_error;

int main()
try
{
	// creates required files
	create_file_if(dictionary_filename);
	// create_file_if(practice_filename_left);
	// create_file_if(practice_filename_right);

	// retrieves dictionary information from file
	Dictionary dictionary = get_words_and_translations();
	vector<string>& words_left =  dictionary.words_left;
	vector<string>& words_right =  dictionary.words_right;
	
	// displays main menu
	display_menu();

	// gets user's choice
	for(string choice; getline(cin, choice);){
		if (choice.length() != 1) choice = INVALID_CHOICE;
		else {
			// retrieves dictionary information from file
			dictionary = get_words_and_translations();
		}

		switch (choice[0]) {
		case '1':
			cout << newline;
			if(!words_left.empty() && words_left.size() == words_right.size()){
				quiz_launcher(dictionary, Dictionary::Mode::normal);
			}
			else {
				if(words_left.empty()) cout << "There's not a single word to display.\n";
				else cout << "The number of words doesn't match.\n";
			}
			break;

		case '2':
            cout << newline;
            if(!words_left.empty() && words_left.size() == words_right.size()){
                quiz_launcher(dictionary, Dictionary::Mode::reverse);
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
