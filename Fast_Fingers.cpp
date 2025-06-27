#include <iostream>
#include <time.h>
#include <cstring>
#include <fstream>
#include <cstdio>
// #include <conio.h>
#include <stdlib.h>

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
    cout << "For <Start> Enter 1 , For <Statistics> Enter 2 , For <About> Enter 3 , For <Exit> Enter 4." << endl;
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
        }
    }
    while(choice != 4);
    return 0;        
}
//------------------------------------------------------------------------------------------------------------------
