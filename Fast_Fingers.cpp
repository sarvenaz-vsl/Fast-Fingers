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

struct player {
    string name ;
    int all_words;
    int correct_words;
    int incorrect_words;
    double speed;
    time_t start_game_time;
};

//-------------------------------------------- Function declarations --------------------------------------------

// display menu
void show_menu();

// sort leaderboard
void sort_players_by_speed(player a[], int num);

// lowercase converter
string to_lowercase(string s);

// log errors
void update_char_mistake_count(const string& input, const string& word, int char_mistake_count[128]);

// fetch random word
bool get_random_word(string& chosen_word, bool hard_mode);

// main game
void run_main_game(player& current_player, int char_mistake_count[128], bool hard_mode);

// focused practice
void targeted_practice_round(const vector<char>& targeted_chars, player& current_player, int char_mistake_count[128]);

// analyze mistakes
bool evaluate_errors_and_practice(player& current_player, int char_mistake_count[128]);

// show results
void show_results(const player& current_player);

// choose difficulty
bool choose_difficulty();

// compare accuracy before and after second round
void check_improvement(int correct_before, int incorrect_before, int correct_after, int incorrect_after);

// save the average result
void save_average_result(const player& current_player);

// second round summary
void show_second_round_summary(const player& before, const player& after);

// computing difference of each round
player get_round_difference(const player& first, const player& second, const string& name);

// computing average of rounds
player get_average_result(const player& first, const player& second, const string& name);

// checking the validity of input name
string get_valid_name(const string& prompt = "Enter your name: ");

// start_game game
void start_game();

//-------------------------------------------- Display menu --------------------------------------------
// Display the main menu options to the user

void show_menu() {
    cout << "\nFor <Start> Enter 1 \nFor <Statistics> Enter 2 \nFor <About> Enter 3 \nFor <Exit> Enter 4" << endl;
}

//-------------------------------------------- Sort Leaderboard --------------------------------------------
//Sort the array of player structs by speed (descending)

void sort_players_by_speed( player a[], int num ) {
	player temp ;
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
//Convert a string to all lowercase letters

string to_lowercase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

//-------------------------------------------- Log errors --------------------------------------------
//Update per-character mistake counts based on user input vs the correct word

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
//Fetch a random word from the dataset, filtered by difficulty

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

    if (word_list.empty()) {
        cerr << "No valid words found in dataset.txt .\n";
        return false;
    }

    int random = rand() % word_list.size();
    chosen_word = word_list[random];
    return true;
}

//-------------------------------------------- Show results --------------------------------------------
//Print the typing summary/results for a player after a round

void show_results(const player& current_player) {
    cout << "\n========= Your Typing Summary =========" << endl;
    cout << "Name: " << current_player.name << endl;
    cout << "Total Words Attempted: " << current_player.all_words << endl;
    cout << "Correct Words: " << current_player.correct_words << endl;
    cout << "Incorrect Words: " << current_player.incorrect_words << endl;
    cout << "Typing Speed (words/min): " << current_player.speed << endl;
    cout << "-----------------------------------------" << endl;
}

//-------------------------------------------- Choose difficulty --------------------------------------------
//Prompt user for difficulty and validate input (1 or 2 only)

bool choose_difficulty() {
    int difficulty_choice;
    cout << "Select Difficulty:\n1. Normal\n2. Difficult" << endl;
    cout << "Enter your choice (1 or 2): \n";
    while (!(cin >> difficulty_choice) || (difficulty_choice != 1 && difficulty_choice != 2)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter 1 for Normal or 2 for Difficult: ";
    }
    return (difficulty_choice == 2);
}

//-------------------------------------------- Check Improvement --------------------------------------------
//Print an improvement summary comparing before and after practice rounds

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

//-------------------------------------------- Save average result --------------------------------------------
//Save the player's average result to the leaderboard file

void save_average_result(const player& current_player) {
    ifstream infile("result.txt");
    vector<player> all_results;
    player temp;

    while (infile >> temp.name >> temp.all_words >> temp.correct_words >> temp.incorrect_words >> temp.speed) {
        all_results.push_back(temp);
    }
    infile.close();

    bool updated = false;
    for (auto& entry : all_results) {
        if (entry.name == current_player.name) {
            entry = current_player; // Always overwrite with new average result
            updated = true;
            break;
        }
    }

    if (!updated) {
        all_results.push_back(current_player);
    }

    ofstream outfile("result.txt");
    for (const auto& entry : all_results) {
        outfile << entry.name << '\t' << entry.all_words << '\t'
                << entry.correct_words << '\t' << entry.incorrect_words << '\t'
                << entry.speed << endl;
    }
    outfile.close();
}

//-------------------------------------------- Keep log of each round --------------------------------------------
//Log each game round (first and second) to a full results file

void log_all_rounds(const player& current_player, const string& label, const player* before = nullptr)
{
    ofstream full_log("full_results.txt", ios::app);

    player result = current_player;

    // If before is provided, log only the second round
    if (before != nullptr) {
        result.all_words -= before->all_words;
        result.correct_words -= before->correct_words;
        result.incorrect_words -= before->incorrect_words;
        result.speed = result.correct_words / (GAME_DURATION / 60.0);
    }

    if (full_log) {
        full_log << result.name << "_" << label << '\t'
                 << result.all_words << '\t'
                 << result.correct_words << '\t'
                 << result.incorrect_words << '\t'
                 << result.speed << endl;
        full_log.close();
    }
}

//-------------------------------------------- Summary for 2nd round --------------------------------------------
//Display a focused practice summary for the second round

void show_second_round_summary(const player& before, const player& after) {
    player second_round;
    second_round.name = after.name;
    second_round.all_words = after.all_words - before.all_words;
    second_round.correct_words = after.correct_words - before.correct_words;
    second_round.incorrect_words = after.incorrect_words - before.incorrect_words;
    second_round.speed = second_round.correct_words / (GAME_DURATION / 60.0);

    cout << "\n========= Focused Practice Summary =========" << endl;
    cout << "Name: " << second_round.name << endl;
    cout << "Words Practiced: " << second_round.all_words << endl;
    cout << "Correct Words: " << second_round.correct_words << endl;
    cout << "Incorrect Words: " << second_round.incorrect_words << endl;
    cout << "Typing Speed (words/min): " << second_round.speed << endl;
    cout << "---------------------------------------------" << endl;
}

//-------------------------------------------- Computing difference of each round --------------------------------------------
//Compute the difference between two rounds for a player

player get_round_difference(const player& first, const player& second, const string& name) {
    player diff;
    diff.name = name;
    diff.correct_words = second.correct_words - first.correct_words;
    diff.incorrect_words = second.incorrect_words - first.incorrect_words;
    diff.all_words = second.all_words - first.all_words;
    diff.speed = (GAME_DURATION > 0) ? (diff.correct_words / (GAME_DURATION / 60.0)) : 0;
    return diff;
}

//-------------------------------------------- Computing average of rounds --------------------------------------------
//Compute the average result of two rounds for a player

player get_average_result(const player& first, const player& second, const string& name) {
    player avg;
    avg.name = name;
    avg.correct_words = int((first.correct_words + second.correct_words) / 2.0);
    avg.incorrect_words = int((first.incorrect_words + second.incorrect_words) / 2.0);
    avg.all_words = int((first.all_words + second.all_words) / 2.0);
    avg.speed = (first.speed + second.speed) / 2.0;
    return avg;
}

//-------------------------------------------- Check the validity of name --------------------------------------------
// Validate and return a player's name (not empty, trims spaces/tabs)

string get_valid_name(const string& prompt) {
    string name;
    cout << prompt;
    getline(cin, name);

    while (true) {
        while (!name.empty() && (name[0] == ' ' || name[0] == '\t'))
            name.erase(0, 1);
        while (!name.empty() && (name[name.length() - 1] == ' ' || name[name.length() - 1] == '\t'))
            name.erase(name.length() - 1, 1);

        if (!name.empty())
            break;

        cout << "Name cannot be empty. Please enter your name: ";
        getline(cin, name);
    }
    return name;
}

//-------------------------------------------- Main game --------------------------------------------
//Main game logic for a round of typing

void run_main_game(player& current_player, int char_mistake_count[128], bool hard_mode) {
    time_t begin = time(0);
    string input;

    while ((time(0) - begin) < GAME_DURATION) {
        string word;
        if (!get_random_word(word, hard_mode))
            return;

        cout << word << endl;
        
        //Prevent late input after time runs out
        if ((time(0) - begin) >= GAME_DURATION) {
            cout << "Time's up before input!" << endl;
            break;
        }

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
//Analyze character mistakes and, if needed, launch a focused practice round

bool evaluate_errors_and_practice(player& current_player, int char_mistake_count[128]) {
    bool anyMistakeLogged = false;
    for (int i = 0; i < 128; i++) {
        if (char_mistake_count[i] > 0) {
            anyMistakeLogged = true;
            break;
        }
    }

    if (!anyMistakeLogged) {
        cout << "Great job! You didn't mistype any specific letters.\n\n";
        return false;
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

        while (tolower(response) != 'y' && tolower(response) != 'n') {
            cout << "Please enter 'y' or 'n': ";
            cin >> response;
        }

        if (tolower(response) == 'y') {
            player before = current_player;

            targeted_practice_round(targeted_chars, current_player, char_mistake_count);

            check_improvement(before.correct_words, before.incorrect_words,
                            current_player.correct_words, current_player.incorrect_words);

            show_second_round_summary(before, current_player);
            log_all_rounds(current_player, "second", &before);

            return true; // second round played!
        }
    }
    return false; // second round NOT played
}

//-------------------------------------------- Start game --------------------------------------------
//The entry point for a full session for one player 

void start_game() {
    player current_player;
    int char_mistake_count[128] = {0};

    cin.ignore(); // Only needed if a cin >> was used before!
    current_player.name = get_valid_name();

    current_player.correct_words = 0;
    current_player.incorrect_words = 0;
    current_player.all_words = 0;
    current_player.start_game_time = time(0);
    srand(time(0));

    bool hard_mode = choose_difficulty();  // Ask user for difficulty level
    player best_round = current_player;  // Store first round

    run_main_game(current_player, char_mistake_count, hard_mode);
    log_all_rounds(current_player, "first");

    // After first round, store its stats for averaging later
    best_round.correct_words = current_player.correct_words;
    best_round.incorrect_words = current_player.incorrect_words;
    best_round.all_words = current_player.all_words;
    best_round.speed = current_player.speed;

    // Pass best_round (copy of first round) and update current_player if second round played
    bool did_second_round = evaluate_errors_and_practice(current_player, char_mistake_count);

    if (did_second_round) {
        player second_round = get_round_difference(best_round, current_player, current_player.name); // Calculate second round
        player average_result = get_average_result(best_round, second_round, current_player.name);   // Compute average
        save_average_result(average_result);  // Save average result
    } else {
        save_average_result(current_player); // Only one round played, save as is
    }
}

//-------------------------------------------- Targeted practice round --------------------------------------------
//Launch a focused practice round on the user's most-mistyped letters

void targeted_practice_round(const vector<char>& targeted_chars, player& current_player, int char_mistake_count[128]) {
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

        //Prevent reading input if time is up
        if ((time(0) - second_begin) >= GAME_DURATION) {
            cout << "Time's up before input!" << endl;
            break;
        }

        cin >> input;

        int remaining_time = (GAME_DURATION - (time(0) - second_begin));
        
        string lower_input = to_lowercase(input);
        string lower_word = to_lowercase(selected_word);

        if (lower_input == lower_word) {
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
}

//-------------------------------------------- Statistics --------------------------------------------
//Display statistics (leaderboard) sorted by speed 

void statistics() {
    ifstream current_player("result.txt",ios::in);
    player s[1000];
    int num;
    int i = 0;
    while(current_player >> s[i].name >> s[i].all_words >> s[i].correct_words >> s[i].incorrect_words >> s[i].speed) {
        i++;
    }
	current_player.close();
	num = i;
    sort_players_by_speed (s, num) ;
    
    ofstream write_to_file("result.txt");
    for( i = 0; i < num; i++) {
        write_to_file << s[i].name << '\t' << s[i].all_words << '\t'
        << s[i].correct_words << '\t' << s[i].incorrect_words << '\t'
        << s[i].speed << endl;
    }
	write_to_file.close();
    
    cout << "\nRank\tName\t\tAll\tCorrect\t\tIncorrect\tSpeed\n";
    cout << "----------------------------------------------------------------------\n";

    for (int i = 0; i < num; i++) {
        cout << (i + 1) << '\t' << s[i].name << "\t\t"
            << s[i].all_words << '\t'
            << s[i].correct_words << "\t\t"
            << s[i].incorrect_words << "\t\t"
            << int(s[i].speed * 100 + 0.5) / 100.0 << endl;
    }
}

//-------------------------------------------- Show About --------------------------------------------

void show_about() {
    cout << "\n========== About Fast Fingers ==========\n";
    cout << "Fast Fingers is a timed typing game where you try to type as many\n";
    cout << "words correctly as possible within a limited time. After the main\n";
    cout << "round, you get a focused practice round with your most-mistyped letters.\n";
    cout << "Your average performance is saved and compared with other players!\n";
    cout << "\nHow to Play:\n";
    cout << "- Choose a difficulty (Normal or Difficult)\n";
    cout << "- Type each word exactly as it appears\n";
    cout << "- After the round, practice your most-mistyped letters for extra improvement\n";
    cout << "- Check your stats and see the leaderboard!\n";
    cout << "-------------------------------------------\n";
}

//--------------------------------------------------------------------------------------------------------------
int main()
{
    int choice;            
    do {
        show_menu();
        cout << "\nPlease enter your choice (1-4): ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number between 1 and 4: ";
        }
        switch(choice) {
            case 1: start_game(); break;
            case 2: statistics(); break;
            case 3: show_about(); break;
            case 4: cout << "Goodbye!" << endl; break;
        }
    } while(choice != 4);

    return 0;        
}