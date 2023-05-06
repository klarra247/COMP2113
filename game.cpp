#include "game.h"
#include "setup.h"
#include "players.h"

using namespace std;


//
const int OTHER = 0;
const int WRONG_POS = 1;
const int CORRECT_POS = 2;



vector<string> guessedWord;

//
void addGuess(string& input({
    guessedWord.push_back(input);
}
			    
bool guessed_before(string& user_input){
    if (find(guessedWord.begin(), guessedWord.end(), user_input) != guessedWord.end()){
	return true;	
    }
    else {
	return false;
    }
}

void checkCase(string& word) {
    for (int i = 0; i < word.length(); i++) {
        if(isupper(word[i])) {
            word[i] = tolower(word[i]);
        } 
    }
}

//
string getRandomWord(){
    ifstream files("wordfile.txt");
    vector<string> wordlist;
    string words;
    while (files >> words) {
        checkCase(words);
        wordlist.push_back(words);
    }
    files.close();
    srand(time(NULL));
    int randomNumber = rand() % wordlist.size();
    return wordlist[randomNumber];
}
//


//
bool check_if_5_letters(const string& user_input){
	for ( int i = 0; i < user_input.length(); i++)
	{
		if (isdigit(user_input[i]))
			return false;
	}
	return (user_input.length() == 5);
}

bool spellcheck(const string& user_input){
    //open text file with all valid words
    string filename = "dictionary.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error" << endl;
        return false;
    }

    //add every word from txt file to vector
    vector<string> words;
    string word;
    while (getline(file, word)) {
        words.push_back(word);
    }
    file.close();

    //binary search to see if word is valid 
    int lower = 0;
    int upper = words.size() - 1;
    while (lower <= upper) {
        int mid = (lower + upper) / 2;
        if (words[mid] == user_input) {
            return true;
        } else if(words[mid] < user_input) {
            lower = mid + 1;
        } else if(words[mid] > user_input) {
            upper = mid - 1;
        }
    }
    return false;
}
//


//
void toLowerCase( string &input)
{
    transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
    { return tolower(c); });
}
//

//
void markMatch(vector<vector<int> > &matches, int tryIndex, string target, string guess)
{
    for (int i = 0; i < guess.length(); i++)
    {
        matches[tryIndex][i] = OTHER;
    }
    for (int j = 0; j < guess.length(); j++)
    {
        for (int i = 0; i < target.length(); i++)
        {
            if (guess[j] == target[i])
            {
                if (i == j)
                {
                    matches[tryIndex][j] = CORRECT_POS;
                   break;
                }
                else
                {
                    matches[tryIndex][j] = WRONG_POS;
                }
            }
        }
    }


}

//
void printWordle( vector< string> tries,  vector< vector<int>> matches, int currentTry)
{
    
    for (int i = 0; i <= currentTry && i < tries.size(); i++)
    {
         string separator = "-";
         string padding = "|";
         string text = "|";
        for (int j = 0; j < tries[i].length(); j++)
        {
            separator += "------";
            padding += "     |";
            char value =  toupper(tries[i][j]);
            text += "  ";
            if (matches[i][j] == WRONG_POS)
            {
                text += "\033[33m";
            }
            else if (matches[i][j] == CORRECT_POS)
            {
                text += "\033[32m";
            }
            text += value;
            if (matches[i][j] == WRONG_POS || matches[i][j] == CORRECT_POS)
            {
                text += "\033[0m";
            }
            text += "  |";
        }
        if (i == 0)
        { 
             cout << separator <<  endl;
        }
         cout << padding <<  endl;
         cout << text <<  endl;
         cout << padding <<  endl; 
         cout << separator <<  endl;
    }
}
//
//
bool isAllMatch(string target, string guess)
{
    for (int i = 0; i < guess.length(); i++)
    {
        if (guess[i] != target[i])
            return false;
    }
    return true;
}
//

bool playGame()
{
    //
    int numberOfTries = 6;
    vector<string> tries(numberOfTries);
    vector<vector<int> > matches(numberOfTries, vector<int>(5));
    string targetWord = getRandomWord();
    toLowerCase(targetWord);   
    string input;
    int currentTry = 0; 

    while (currentTry < numberOfTries){
            cout << "Please enter your guess (word length must be 5) or type Q/q to quit: ";
            getline(cin, input);
        if(input != "q" && spellcheck(input) == true && check_if_5_letters(input) == true){

	    if (guessed_before(input) == true){
                cout << "You have guessed this word before! Try a new word." << endl;;
                continue;
            } else{
                    addGuess(input);
            }

            toLowerCase(input);
            tries[currentTry] = input;
            markMatch(matches, currentTry, targetWord, input);
            printWordle(tries, matches, currentTry);
            
            
            
            if (isAllMatch(targetWord, input))
            {
                cout << "Found the word" << endl;
                return true;
            }
            if (currentTry == 4)
            {
                cout << "You failed to find the word" << endl;
                return false;
            }
        } 
        else if (input == "q"){
            cout << "Quit game" <<  endl;
            break;
        }   
        else if (check_if_5_letters(input) == false){
            cout << "The input should be 5 words! Try Again."<<endl;
            continue;
        }
        else if (spellcheck(input) == false){
            cout << "The word does not exist! Try Again."<<endl;
            continue;
        }
      

 
        currentTry++;
    }
    guessedWord.clear();


}



