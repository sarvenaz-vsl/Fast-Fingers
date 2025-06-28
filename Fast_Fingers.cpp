#include <iostream>
#include <time.h>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cctype> // for isascii()
#include <vector>


using namespace std;

const int GAME_DURATION = 20;

struct student {
    string name ;
    int all_words;
    double correct_words;
    double incorrect_words;
    double speed;
    time_t start_game_time;
};

//-------------------------------------------- Function declarations --------------------------------------------

// display menu
void show_menu();

// sort leaderboard
void sort_students_by_speed(student a[], int num);

// lowercase converter
string to_lowercase(string s);

// log errors
void update_char_mistake_count(const string& input, const string& word, int char_mistake_count[128]);

// fetch random word
bool get_random_word(string& chosen_word, bool hard_mode);

// main game
void run_main_game(student& current_player, int char_mistake_count[128], bool hard_mode);

// focused practice
void targeted_practice_round(const vector<char>& targeted_chars, student& current_player, int char_mistake_count[128]);

// analyze mistakes
void evaluate_errors_and_practice(student& current_player, int char_mistake_count[128]);

// show results
void show_results(const student& current_player);

// choose difficulty
bool choose_difficulty();

// compare accuracy before and after second round
void check_improvement(int correct_before, int incorrect_before, int correct_after, int incorrect_after);

// start_game game
void start_game();

//-------------------------------------------- Display menu --------------------------------------------

void show_menu() {
    cout << "\nFor <Start> Enter 1 \nFor <Statistics> Enter 2 \nFor <About> Enter 3 \nFor <Exit> Enter 4" << endl;
}

//-------------------------------------------- Sort Leaderboard --------------------------------------------

void sort_students_by_speed( student a[], int num ) {
	student temp ;
    for(int i = 0 ; i < num ; i++) {
        for(int j = 0; j < num - i -1; j++) {
            if(a[j].speed < a[j + 1].speed) {
                temp = a[j] ;
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

//-------------------------------------------- Lowercase converter --------------------------------------------

string to_lowercase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

//-------------------------------------------- Log errors --------------------------------------------

void update_char_mistake_count(const string& input, const string& word, int char_mistake_count[128]) {
    int minLen = min(input.length(), word.length());

    for (int i = 0; i < minLen; i++) {
        if (tolower(input[i]) != tolower(word[i]) && isascii(word[i])) {
            char lower = tolower(word[i]);
            char_mistake_count[(int)lower]++;
        }
    }
    for (int i = minLen; i < word.length(); i++) {
        if (isascii(word[i])) {
            char lower = tolower(word[i]);
            char_mistake_count[(int)lower]++;
        }
    }
}

//-------------------------------------------- Fetch random word --------------------------------------------

bool get_random_word(string& chosen_word, bool hard_mode) {
    ifstream wordsFile("dataset.txt", ios::in);
    if (!wordsFile) {
        cerr << "Error opening dataset.txt\n";
        return false;
    }

    vector<string> word_list;
    string line;
    while (getline(wordsFile, line)) {
        if (!hard_mode || line.length() > 6) {
            word_list.push_back(line);
        }
    }
    wordsFile.close();

    if (word_list.empty()) return false;

    int random = rand() % word_list.size();
    chosen_word = word_list[random];
    return true;
}

//-------------------------------------------- Show results --------------------------------------------

void show_results(const student& current_player) {
    cout << "\n========= Your Typing Summary =========" << endl;
    cout << "Name: " << current_player.name << endl;
    cout << "Total Words Attempted: " << current_player.all_words << endl;
    cout << "Correct Words: " << current_player.correct_words << endl;
    cout << "Incorrect Words: " << current_player.incorrect_words << endl;
    cout << "Typing Speed (words/min): " << current_player.speed << endl;
    cout << "-----------------------------------------" << endl;
}

//-------------------------------------------- Choose difficulty --------------------------------------------

bool choose_difficulty() {
    int difficulty_choice;
    cout << "Select Difficulty:\n1. Normal\n2. Difficult" << endl;
    cin >> difficulty_choice;
    return (difficulty_choice == 2);
}

//-------------------------------------------- Check Improvement --------------------------------------------

void check_improvement(int correct_before, int incorrect_before, int correct_after, int incorrect_after) {
    int total_before = correct_before + incorrect_before;
    int total_after = correct_after + incorrect_after;

    double accuracy_before = (total_before > 0) ? (double)correct_before / total_before : 0;
    double accuracy_after = (total_after > 0) ? (double)correct_after / total_after : 0;

    cout << "\n=========== Improvement Summary ===========\n";
    cout << "Accuracy Before: " << accuracy_before * 100 << "%" << endl;
    cout << "Accuracy After:  " << accuracy_after * 100 << "%" << endl;

    if (accuracy_after > accuracy_before) {
        cout << "Great job! Your accuracy improved after the focused practice round!\n";
    } else if (accuracy_after < accuracy_before) {
        cout << "Looks like your accuracy dropped slightly. Keep practicing!\n";
    } else {
        cout << "Your accuracy stayed the same. Still a solid effort!\n";
    }
    cout << "===========================================\n";
}

//-------------------------------------------- Main game --------------------------------------------

void run_main_game(student& current_player, int char_mistake_count[128], bool hard_mode) {
    time_t begin = time(0);
    string input;

    while ((time(0) - begin) < GAME_DURATION) {
        string word;
        if (!get_random_word(word, hard_mode))
            return;

        cout << word << endl;
        cin >> input;

        int remaining_time = GAME_DURATION - (time(0) - begin);

        string lower_input = to_lowercase(input);
        string lower_word = to_lowercase(word);

        if (lower_input == lower_word) {
            cout << "remaining time : " << remaining_time << endl;
            current_player.correct_words++;
        } else {
            cout << "\a" << "remaining time : " << remaining_time << endl;
            current_player.incorrect_words++;
            update_char_mistake_count(input, word, char_mistake_count);
        }

        current_player.all_words++;

        if ((GAME_DURATION - (time(0) - begin)) < 0) {
            cout << "The last word was failed!" << endl;
            current_player.all_words--;
            if (lower_input == lower_word)
                current_player.correct_words--;
            else
                current_player.incorrect_words--;
        }
    }

    // show results after game ends
    current_player.speed = current_player.correct_words / (GAME_DURATION / 60.0);
    show_results(current_player);
}

//-------------------------------------------- Analyze mistakes --------------------------------------------

void evaluate_errors_and_practice(student& current_player, int char_mistake_count[128]) {
    bool anyMistakeLogged = false;
    for (int i = 0; i < 128; i++) {
        if (char_mistake_count[i] > 0) {
            anyMistakeLogged = true;
            break;
        }
    }

    if (!anyMistakeLogged) {
        cout << "Great job! You didn't mistype any specific letters.\n\n";
        return;
    }

    cout << "Your most mistyped letters:\n";
    int top3[3] = {-1, -1, -1};
    int top3Count[3] = {0, 0, 0};

    for (int i = 97; i <= 122; i++) {
        for (int j = 0; j < 3; j++) {
            if (char_mistake_count[i] > top3Count[j]) {
                for (int k = 2; k > j; k--) {
                    top3Count[k] = top3Count[k - 1];
                    top3[k] = top3[k - 1];
                }
                top3Count[j] = char_mistake_count[i];
                top3[j] = i;
                break;
            }
        }
    }

    vector<char> targeted_chars;
    for (int i = 0; i < 3; i++) {
        if (top3[i] != -1 && top3Count[i] > 0) {
            cout << "- '" << (char)top3[i] << "' was mistyped " << top3Count[i] << " times\n";
            targeted_chars.push_back((char)top3[i]);
        }
    }

    cout << "=======================================\n";

    if (!targeted_chars.empty()) {
        char response;
        cout << "\nWould you like to practice the letters you mistyped the most? (y/n): ";
        cin >> response;

        if (tolower(response) == 'y') {
            int correct_before = current_player.correct_words;
            int incorrect_before = current_player.incorrect_words;

            targeted_practice_round(targeted_chars, current_player, char_mistake_count);

            int correct_after = current_player.correct_words;
            int incorrect_after = current_player.incorrect_words;

            check_improvement(correct_before, incorrect_before, correct_after, incorrect_after);
        }
    }
}

//-------------------------------------------- start_game game --------------------------------------------

void start_game() {
    student current_player;
    int char_mistake_count[128] = {0};
    ofstream result("result.txt", ios::app);

    cout << "Enter your name" << endl;
    cin >> current_player.name;

    current_player.correct_words = 0;
    current_player.incorrect_words = 0;
    current_player.all_words = 0;
    current_player.start_game_time = time(0);
    srand(time(0));

    bool hard_mode = choose_difficulty();  // Ask user for difficulty
    run_main_game(current_player, char_mistake_count, hard_mode); 
    evaluate_errors_and_practice(current_player, char_mistake_count);

    if (!result)
    {
        cout << "file doesn't exist." << endl;
        exit(1);
    }

    current_player.speed = current_player.correct_words / 60;
    result << current_player.name << '\t' << current_player.all_words << '\t';
    result << current_player.correct_words << '\t' << current_player.incorrect_words;
    result << '\t' << current_player.speed << '\t' << endl;
    result.close();
}

//--------------------------------------------------------------------------------------------------------------

void targeted_practice_round(const vector<char>& targeted_chars, student& current_player, int char_mistake_count[128]) {
    cout << "\nstart_gameing focused practice round for your most mistyped letters:\n";

    vector<string> word_bank;
    string word;
    
    ifstream wordsFile("dataset.txt");
    if (!wordsFile) {
        cerr << "Error opening dataset.txt\n";
        return;
    }
    
    while (getline(wordsFile, word)) {
        word_bank.push_back(word);
    }
    wordsFile.close();

    time_t second_begin = time(0);
    string input;

    while ((time(0) - second_begin) < GAME_DURATION) {
        string selected_word;
        bool found = false;

        for (int attempt = 0; attempt < 50; ++attempt) {
            int idx = rand() % word_bank.size();
            selected_word = word_bank[idx];
            for (char c : targeted_chars) {
                if (selected_word.find(c) != string::npos) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) break;

        cout << selected_word << endl;
        cin >> input;

        int remaining_time = (GAME_DURATION - (time(0) - second_begin));
        if (input == selected_word) {
            cout << "remaining time : " << remaining_time << endl;
            current_player.correct_words++;
        } else {
            cout << "\a" << "remaining time : " << remaining_time << endl;
            current_player.incorrect_words++;

            int minLen = min(input.length(), selected_word.length());
            for (int i = 0; i < minLen; i++) {
                if (input[i] != selected_word[i] && isascii(selected_word[i])) {
                    char lower = tolower(selected_word[i]);
                    char_mistake_count[(int)lower]++;
                }
            }
            for (int i = minLen; i < selected_word.length(); i++) {
                if (isascii(selected_word[i])) {
                    char lower = tolower(selected_word[i]);
                    char_mistake_count[(int)lower]++;
                }
            }
        }
        current_player.all_words++;
    }
    //Show results after focused practice
    current_player.speed = current_player.correct_words / (GAME_DURATION / 60.0);
    show_results(current_player);
}

//------------------------------------------------------------------------------------------------------------------
void statistics()
{
    ifstream current_player("result.txt",ios::in);
    student s[1000];
    int num;
    int i = 0;
	while(current_player >> s[i].name >> s[i].all_words >> s[i].speed >> s[i].correct_words >> s[i].incorrect_words)
    {
        i++;
    }
	current_player.close();
	num = i;
    sort_students_by_speed (s, num) ;
    
    ofstream write_to_file("result.txt");
    for( i = 0; i <= num; i++)
    {
        write_to_file << s[i].name << '\t' << s[i].all_words << '\t' << s[i].speed << '\t' 
        << s[i].correct_words << '\t' << s[i].incorrect_words<< endl ;
    }
	write_to_file.close();
    
    for(i = 1; i < num; i++)
    {
        cout << i << endl;
        cout << "Name : " << s[i].name << endl << "All words : " << s[i].all_words << endl;
        cout << "Correct words : " << s[i].correct_words << endl << "Incorrect words : " << s[i].incorrect_words <<endl;
        cout <<"Speed : " << s[i].speed << endl;
    }
}

//--------------------------------------------------------------------------------------------------------------
int main()
{
    int choice;
    do
    {
        show_menu();
        cin >> choice;
        
        switch(choice)
        {
            case 1:
            {
                start_game();
                break;
            }
            
            case 2:
            {
				statistics () ;
                break;
            }
            
            case 3:
            {
                cout << "dar ebteda be karbar menuii neshan dade mi shavad ke bayad entekhab konad" << endl;
                cout << "agar varede avali shavad, namash az oo khaste mishavad" << endl;
                cout << "dar modat zamane 60 sanie bayad kalamati ke ruye desktop zaher mishavad ra type konad" << endl;
                cout << "agar dorost type konad, be tedad kalamate doroste oo ezafe mi shavad" << endl;
                cout << "agar eshtebah type konad sedaye bugh mishenavad" << endl;
                cout << "pas az type har kalame niz zamane baghi mande ruye desktop neshan dade mi shavad" << endl;
                cout << "dar nahayat sorate type vey mohasebe mishavad" << endl;
                cout << "agar varede case dovom shavad, moaqayeseii beyne sorate afrad anjam mi shavad" << endl;
                cout << "va afrad bar asase sorateshan rade bandi mi shavand." << endl;
                
                break;
            }

            case 4:
            {
                cout << "Goodbye!" << endl;
                break;
            }
            default:
            cout << "Invalid input. Try again." << endl;
        }
    }
    while(choice != 4);
    return 0;        
}
//------------------------------------------------------------------------------------------------------------------
