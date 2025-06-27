#include <iostream>
#include <time.h>
#include <cstring>
#include <fstream>
#include <cstdio>
// #include <conio.h>
#include <stdlib.h>
#include <cctype> // for isascii()

using namespace std;

struct student
{
    string name ;
    int all_words;
    double correct_words;
    double incorrect_words;
    double speed;
    time_t start_time;
};
//--------------------------------------------------------------------------------------------------------------
void show_menu()
{
    cout << "\nFor <Start> Enter 1 \nFor <Statistics> Enter 2 \nFor <About> Enter 3 \nFor <Exit> Enter 4" << endl;
}
//-------------------------------------------------------------------------------------------------------------
void student_sort( student a[], int num )
{
    //int num = 0;
	student temp ;
    for(int i = 0 ; i < num ; i++)
    {
        for(int j = 0; j < num - i -1; j++)
        {
            if(a[j].speed < a[j + 1].speed)
            {
                temp = a[j] ;
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------
void start()
{
    string input; //word that you enter
    student my_student;
    int error_freq[128] = {0}; // Tracks how often each ASCII character is mistyped
	ofstream result("result.txt", ios::app);
    
    cout << "Enter your name" << endl;
    cin >> my_student.name;

    my_student.start_time = time(0);
    
    time_t begin = time(0);
    while ((time(0) - begin) < 20)
    {
        string word;
        int random = 0;
        int numOfLines = 0;
        ifstream wordsFile("dataset.txt", ios::in);
        srand(time(0));
        random = rand() % 5000;
        while(getline(wordsFile, word))
        {
            ++numOfLines;
            if(numOfLines == random)
            {
                cout << word << endl; //shows the random words in dataset file
                
                cin >> input;

                int remaining_time = (20 - (time(0) - begin));
                if(input == word)
                {
                    cout << "remaining time : " << remaining_time << endl;
                    my_student.correct_words ++;
                }
                else
                {
                    cout << "\a" << "remaining time : " << remaining_time << endl;
                    my_student.incorrect_words ++;
                    // Count mistyped characters
                    // Compare each character in the incorrect word
                    int minLen = min(input.length(), word.length());
                    for (int i = 0; i < minLen; i++) {
                        if (input[i] != word[i] && isascii(word[i])) {
                            error_freq[(int)word[i]]++;
                        }
                    }
                    // If word is longer than input, count remaining letters as missed
                    for (int i = minLen; i < word.length(); i++) {
                        if (isascii(word[i])) {
                            error_freq[(int)word[i]]++;
                        }
                    }
                }
                my_student.all_words ++;
                if((60 -(time(0) - begin)) < 0)
                {
                    cout << "The last word was failed" << endl;
                    
                    my_student.all_words --;
                    
                    if(input == word)
                        my_student.correct_words --;
                    
                    else
                        my_student.incorrect_words --;
                }
            }
        }
    }

    // 🔒 Guard against empty or uninitialized memory usage
    bool anyMistakeLogged = false;
    for (int i = 0; i < 128; i++) {
        if (error_freq[i] > 0) {
            anyMistakeLogged = true;
            break;
        }
    }
    if (!anyMistakeLogged) {
    cout << "Great job! You didn't mistype any specific letters.\n";
    } else {
        cout << "Your most mistyped letters:\n";

        int top3[3] = {-1, -1, -1};
        int top3Count[3] = {0, 0, 0};

        for (int i = 32; i < 127; i++) {
            for (int j = 0; j < 3; j++) {
                if (error_freq[i] > top3Count[j]) {
                    for (int k = 2; k > j; k--) {
                        top3Count[k] = top3Count[k - 1];
                        top3[k] = top3[k - 1];
                    }
                    top3Count[j] = error_freq[i];
                    top3[j] = i;
                    break;
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            if (top3[i] != -1 && top3Count[i] > 0) {
                cout << "- '" << (char)top3[i] << "' was mistyped " << top3Count[i] << " times\n";
            }
        }
    }

    if (! result) {
        cout << "file doesnt exist."<<endl;
        exit(1);
    }
    my_student.speed = my_student.correct_words / 60;
    result  << my_student.name << '\t'<< my_student.all_words << '\t';
    result  << my_student.correct_words << '\t' << my_student.incorrect_words;
    result  << '\t' << my_student.speed << '\t'  << endl;
    result.close();
}
//------------------------------------------------------------------------------------------------------------------
void statistics()
{
    ifstream my_student("result.txt",ios::in);
    student s[1000];
    int num;
    int i = 0;
	while(my_student >> s[i].name >> s[i].all_words >> s[i].speed >> s[i].correct_words >> s[i].incorrect_words)
    {
        i++;
    }
	my_student.close();
	num = i;
    student_sort (s, num) ;
    
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
                start();
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
