//
//  main.cpp
//  ProgrammingAssignment4
//
//  Created by Christian Mirt on 11/27/22.
//  Input: type in the console
//  Output: output to the console
//  Program allows user to enter a word for hangman, then has someone try to guess word.  Utilizes linked list (one created from scratch using WordType object
//  and another using the Stand Template Library), exeption handling though try/catch blocks, and many other basic topics learned in the last two years.

#include "WordType.hpp"
#include <list>
#include <iterator>

enum hangman { NONE, PLATFORM, HEAD, BODY, LEFT_ARM, RIGHT_ARM, LEFT_LEG, RIGHT_LEG, HUNG };

int main() {
    //variables
    string word;
    WordType *first, *last, *temp;
    list <char> guessList;
    
    cout << "Welcome to Hangman.\n";
    //initial read
    cout << "Please enter a word for the game or quit\n";
    cin >> word;
    
    //initiate while loop that allows game to continue until "quit" is entered
    while (word != "quit") {
        hangman status = NONE;

        //process word into dynamically created WordType objects
        for (int i = 0; i < word.length(); i++) {
            //dynamically create new class object and put the charcter at current index into it
            WordType *newNode = new WordType(tolower(word.at(i)));
            
            if (i == 0) //if not first in list, set previous link
                first = newNode;
            else
                last->setLink(newNode);

            last = newNode;
        }
        
        system("clear"); //only works for MACOS and only works while running executable
        cout << "Let's play...\n\n";
        
        bool finished = false;
        char guess;
        while (finished == false) { //game running until word is guessed or user loses
            bool done = false;
            bool allLettersFound = true;
            do {
                try {//exception handling
                    cout << "Guess a Letter\n";
                    cin >> guess;
                    
                    if (!cin)
                        throw "Input stream failstate";
                    
                    if (!isalpha(guess)) {
                        throw 1;
                    }
                    
                    guess = tolower(guess);
                    list <char>::iterator it;
                    for (it = guessList.begin(); it != guessList.end(); ++it) {//check to make sure guess has not be enter before
                        if (guess == *it)
                            throw guess;
                    }
                    temp = first;
                    for (int i = 0; i < word.length(); i++) {//check if guess is a letter in word already shown
                        if (guess == temp->getletter()) {
                            if (temp->getIsBlank() == false)
                                throw guess;
                        }
                        temp = temp->getLink();//move to next element in list
                    }
                    done = true;//to exit loop if no exceptions are thrown
                }
                catch (string s) {//error check to make sure input stream is working
                    cout << "Input stream went into a fail state. Restoring...\n";
                    cin.clear();
                    cout << "Input stream restored, be sure to enter a letter\n";
                }
                catch (char ch) {//error check if letter has already been guessed
                    cout << "The letter " << ch << " has already been guess. Try another...\n";
                }
                catch (int x) {//error check if guess is not a letter
                    cout << "Guess is not a letter. Please use charcaters in the alphabet\n";
                }
            }
            while (!done);
            
            bool found = false;
            temp = first;
            for (int i = 0; i < word.length();i++) { //traverse through word list, break if found

                if (guess == temp->getletter()) {
                    found = true;
                    if (found == true) { //if found, set object temp is pointing at to show letter
                        temp->setIsBlank(false);
                    }
                }
                temp = temp->getLink();
            }
            if (found == false) {
                //process guess through second linked list
                guessList.push_back(guess);
                status = hangman(status + 1);
            }
            
            //print word with guessed letters and blank spaces
            temp = first;
            for (int i = 0; i < word.length(); i++) {
                if (temp->getIsBlank() == false)
                    cout << temp->getletter() << " ";
                else
                    cout << "_ ";
                temp = temp->getLink();
            }
            cout << endl;
                
            //print status of game
            switch (status) {
                case NONE:
                    cout << endl;
                    break;
                case PLATFORM:
                    cout << "PLATFORM\n";
                    break;
                case HEAD:
                    cout << "PLATFORM, HEAD\n";
                    break;
                case BODY:
                    cout << "PLATFORM, HEAD, BODY\n";
                    break;
                case LEFT_ARM:
                    cout << "PLATFORM, HEAD, BODY, LEFT_ARM\n";
                    break;
                case RIGHT_ARM:
                    cout << "PLATFORM, HEAD, BODY, LEFT_ARM, RIGHT_ARM\n";
                    break;
                case LEFT_LEG:
                    cout << "PLATFORM, HEAD, BODY, LEFT_ARM, RIGHT_ARM, LEFT_LEG\n";
                    break;
                case RIGHT_LEG:
                    cout << "PLATFORM, HEAD, BODY, LEFT_ARM, RIGHT_ARM, LEFT_LEG, RIGHT_LEG\n";
                    break;
                case HUNG:
                    cout << "HUNG!\n";
                    break;
            }
            
            //print already guess letters
            cout << "Letters used so far (not in word): ";
            list <char>::iterator it;
            int iteration = 0;
            for (it = guessList.begin(); it != guessList.end(); ++it) {//error check to make sure guess has not be enter before
                if (iteration != 0)
                    cout << ", ";
                cout << *it;
                iteration++;
            }
            cout << endl << endl;
            
            //Check to see if game is over
            if (status == HUNG) {
                done = true;
                cout << "You lost. The word was ";
                temp = first;
                for (int i = 0; i < word.length(); i++) {
                    cout << temp->getletter() << " ";
                    temp = temp->getLink();
                }
                cout << endl << endl;
                finished = true;
            }
            allLettersFound = true;
            temp = first;
            for (int i = 0; i < word.length(); i++) {//look throught word list to see if all blanks are gone
                if (temp->getIsBlank() == true)
                    allLettersFound = false;
                temp = temp->getLink();
            }
            if (allLettersFound == true) {//if all blanks are gone, end game
                cout << "You won! Nice...\n\n";
                finished = true;
            }
            
        }
        
        //KILL LINKED LISTS AND POINTERS
        guessList.clear();
        temp = NULL;
        do {
            last = first;
            first = first->getLink();
            delete last;
        }
        while (first != NULL);
        last = NULL;
        
        //get next word or command to quit
        cout << "Please enter a word for the game or quit\n";
        cin >> word;

    }
    
    return 0; //end of main
}

class WordType {
    public:
    WordType(char letter) {
    /*
     Function is the default constructor
     It takes in a char value and sets isBlank to false and
     link pointer to NULL
     */
    this->letter = letter;
    isBlank = true;
    link = NULL;
    }

    char getletter() {
    /*
     Function gets the value of letter
     It returns the char value of letter of the object and
     takes no parameters
     */
    return letter;
    }

    bool getIsBlank() {
    /*
     Function gets the value of isBlank
     Returns the boolean value of isBlank of the object and
     takes no parameters
     */
    return isBlank;
    }

    WordType* getLink() {
    /*
     Function get the value of link
     Returns the object pointer value of link and
     takes not parameters
     */
    return link;
    }

    void setIsBlank(bool isBlank) {
    /*
     Functions sets the value of isBlank
     Return no value and takes a boolean value as an input and
     sets isBlank equal to it
     */
    this->isBlank = isBlank;
    }

    void setLink(WordType* link) {
    /*
     Function sets the pointer value of link
     Returns no value and takes a WordType pointer as an input and
     sets link equal to it
     */
    this->link = link;
    }

    private:
        char letter;
        bool isBlank;
        WordType *link;
};
