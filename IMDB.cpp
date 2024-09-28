/****************************
 * Program 4: IMDB
 *
 * Course: CS 141, Fall 2022
 * System: WindowsOS using Clion
 * Student Author: Zaid Khan Mohammed
 * *** **********************/


/*
 * Instructions for Users
 *
 * This program allows users to search for movies and actors using data from three TSV files:
 * - movie.titles.tsv (movie info)
 * - movie.names.tsv (actor info)
 * - movie.principals.tsv (roles played)
 *
 * How to Use:
 * 1. Compile and run the program in Clion (Windows).
 * 
 * 2. Choose an option from the menu:
 *    a. Search for movies
 *    b. Search for actors/actresses
 *    c. Exit
 *
 * 3. For movie searches, enter a phrase (e.g., "Inception+2010") to find titles, years, and genres.
 *    Select a movie to see its actors or type "done" to return to the menu.
 *
 * 4. For actor searches, enter a name (e.g., "Leonardo+DiCaprio") to see details and roles.
 *    Select an actor to view their movies or type "done" to return.
 *
 * 5. To exit, select option 3 from the menu.
 *
 * Note: Ensure the TSV files are properly formatted and located in the same directory as the executable.
 *
 * Happy Searching!
*/


// including libraries
#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// declaring structs

struct TitleRecord{
    string tconst;
    string titleType;
    string primaryTitle;
    string originalTitle;
    string isAdult;
    string startYear;
    string endYear;
    string runTimeMinutes;
    string genres;
};

struct NameRecord{
    string nconst;
    string primaryName;
    string birthYear;
    string deathYear;
    string primaryProfession;
    string knownForTitles;
};

struct PrincipalRecord{
    string tconst;
    string ordering;
    string nconst;
    string category;
    string job;
    string characters;
};

// struct to store information for movie search
struct ActorAndCharacter{
    string nconst;
    string actorName;
    string characterName;
};

// struct to store information for actor search
struct MovieAndCharacter{
    string tconst;
    string movieName;
    string characterName;
    string movieYear;
};
/*********************************************** Helper functions ***************************************/

// function to split string into elements of vector
void splitString(string line, char delimiter, vector<string>& items ) {
    stringstream s_stream(line);
    while (s_stream.good()) {
        string item;
        getline(s_stream, item, delimiter);
        items.push_back(item);
    }
}

// function to read entire movieTitles file to a vector
void movieTitlesToVector(string titlesFile,vector<string>& movieTitlesFile, vector<TitleRecord>& movieTitlesList){
    string line;
    fstream myfile;
    myfile.open (titlesFile); // opening file
    getline(myfile,line);
    while (myfile) {
        getline(myfile, line); // getting line from file and storing it in the vector
        splitString(line, '\t', movieTitlesFile);
        TitleRecord titleRec;
        // setting struct members according to line read from file
        titleRec.tconst = movieTitlesFile.at(movieTitlesFile.size()-9);
        titleRec.titleType = movieTitlesFile.at(movieTitlesFile.size()-8);
        titleRec.primaryTitle = movieTitlesFile.at(movieTitlesFile.size()-7);
        titleRec.originalTitle = movieTitlesFile.at(movieTitlesFile.size()-6);
        titleRec.isAdult = movieTitlesFile.at(movieTitlesFile.size()-5);
        titleRec.startYear = movieTitlesFile.at(movieTitlesFile.size()-4);
        titleRec.endYear = movieTitlesFile.at(movieTitlesFile.size()-3);
        titleRec.runTimeMinutes = movieTitlesFile.at(movieTitlesFile.size()-2);
        titleRec.genres = movieTitlesFile.at(movieTitlesFile.size()-1);
        movieTitlesList.push_back(titleRec); // pushing struct into vector
    }
    myfile.close(); // closing file
}

// function to read entire movieNames file to vector
void movieNamesToVector(string namesFile, vector<string>& movieNamesFile, vector<NameRecord>& movieNamesList){
    // same process of the first function
    string line;
    fstream myfile2;
    myfile2.open (namesFile); // opening file
    getline(myfile2,line);
    while (myfile2) {
        getline(myfile2, line); // getting line from file and storing it in the vector
        splitString(line, '\t', movieNamesFile);
        NameRecord nameRec;
        nameRec.nconst = movieNamesFile.at(movieNamesFile.size()-6);
        nameRec.primaryName = movieNamesFile.at(movieNamesFile.size()-5);
        nameRec.birthYear = movieNamesFile.at(movieNamesFile.size()-4);
        nameRec.deathYear = movieNamesFile.at(movieNamesFile.size()-3);
        nameRec.primaryProfession = movieNamesFile.at(movieNamesFile.size()-2);
        nameRec.knownForTitles = movieNamesFile.at(movieNamesFile.size()-1);
        movieNamesList.push_back(nameRec);
    }
    myfile2.close(); // closing file
}

// function to read entire moviePrincipals file to vector
void moviePrincipalsToVector(string principalsFile, vector<string>& moviePrincipalsFile, vector<PrincipalRecord>& moviePrincipalsList){
    string line;
    fstream myfile3;
    myfile3.open (principalsFile); // opening file
    getline(myfile3,line);
    while (myfile3) {
        getline(myfile3, line); // getting line from file and storing it in the vector
        splitString(line, '\t', moviePrincipalsFile);
        PrincipalRecord principalRec;
        principalRec.tconst = moviePrincipalsFile.at(moviePrincipalsFile.size()-6);
        principalRec.ordering = moviePrincipalsFile.at(moviePrincipalsFile.size()-5);
        principalRec.nconst = moviePrincipalsFile.at(moviePrincipalsFile.size()-4);
        principalRec.category = moviePrincipalsFile.at(moviePrincipalsFile.size()-3);
        principalRec.job = moviePrincipalsFile.at(moviePrincipalsFile.size()-2);
        principalRec.characters = moviePrincipalsFile.at(moviePrincipalsFile.size()-1);
        moviePrincipalsList.push_back(principalRec);
    }
    myfile3.close(); // closing file
}

// function that executes when user enters menu choice: 1
int MenuChoice1(vector<TitleRecord>& movieTitlesList, vector<NameRecord>& movieNamesList, vector<PrincipalRecord>& moviePrincipalsList,string movieSearchPhrase){
    // declaring variables
    vector<string> movieSearchPhraseWords;
    int movieSearchPhraseWordsCtr=1;
    int selectedMovie;
    string movieTconst;
    vector<ActorAndCharacter> actorsAndCharacters;
    int movieSearchIndex=-1;
    vector<string> foundMovieTitles;
    bool movieFound=0;
    bool matchFoundStatus=0;
    bool finalMovieCheck=0;

    // converting search phrase to lower
    for (int i=0;i<movieSearchPhrase.size();++i){
        movieSearchPhrase.at(i)= tolower(movieSearchPhrase.at(i));
    }
    // counting number of words in search phrase
    for (int i=0;i<movieSearchPhrase.size();++i){
        if (movieSearchPhrase.at(i)=='+'){
            ++movieSearchPhraseWordsCtr;
        }
    }
    // splitting search phrase into words
    splitString(movieSearchPhrase,'+',movieSearchPhraseWords);

    // reading movie title in lowercase for comparison
    for (int i=0;i<movieTitlesList.size();++i){
        string movieTitleToLower=movieTitlesList.at(i).primaryTitle;
        for (int j=0;j<movieTitleToLower.size();++j){
            movieTitleToLower.at(j)=tolower(movieTitleToLower.at(j));
        }
        // checking if every word in search phrase is found
        for (int k=movieSearchPhraseWordsCtr;k>0;--k){
            if (movieTitleToLower.find(movieSearchPhraseWords.at(movieSearchPhraseWords.size()-k))==string::npos){movieFound=0;
                break;
            }
            else{movieFound=1;}
        }
        // skipping to next record in vector if title doesn't match search phrase
        if (movieFound==0){ continue;}
        // changing match found status if even one record is found that matches all words in search phrase
        matchFoundStatus=1;
        // incrementing index that will be shown while printing record
        ++movieSearchIndex;
        // loop to replace commas with spaces in genres
        string genre;
        genre=movieTitlesList.at(i).genres;
        for (int l=0;l<genre.size();++l){
            if (genre.at(l)==','){
                genre.at(l)=' ';
            }
        }
        //printing matching records
        cout<<movieSearchIndex<<":"<<endl<<"Title: "<<movieTitlesList.at(i).primaryTitle<<endl<<"Year: "<<movieTitlesList.at(i).startYear<<endl<<"Genre: "<<genre<<endl<<"----------------"<<endl;
        foundMovieTitles.push_back(movieTitlesList.at(i).tconst); // pushing movie title into vector of movie titles
    }
    // printing appropriate statement if no matches were found
    if (matchFoundStatus==0){cout<<"No match found!"; return 3;}

    // accepting movie index from user
    cout<<"Select a movie to see its actors/actresses (-1 to go back to the previous menu): "<<endl;
    cin>>selectedMovie;
    if (cin.fail()){
        return 0;
    }
    // checking if user tries to access a non existing index
    if (selectedMovie>movieSearchIndex){
        cout<<"No match found!";
        return 3;
    }
    // breaking out of function if user chose to go back to previous menu
    if (selectedMovie==-1){ return -1;}
    // creating struct object to store actor name and character played
    ActorAndCharacter actorAndCharacter;
    movieTconst=foundMovieTitles.at(selectedMovie);
    // setting struct information
    for (int i=0;i<moviePrincipalsList.size();++i) {
        if (moviePrincipalsList.at(i).tconst == movieTconst && moviePrincipalsList.at(i).characters != "\\N") {
            actorAndCharacter.nconst = moviePrincipalsList.at(i).nconst;
            actorAndCharacter.characterName = moviePrincipalsList.at(i).characters;
            actorsAndCharacters.push_back(actorAndCharacter);
        }
    }
    // iterating through file vector to look for actor name
    for (int i=0;i<actorsAndCharacters.size();++i){
        for (int j=0;j<movieNamesList.size();++j){
            if (movieNamesList.at(j).nconst==actorsAndCharacters.at(i).nconst){
                actorsAndCharacters.at(i).actorName=movieNamesList.at(j).primaryName;
            }
        }
    }
    // printing actor name and character played
    for (int i=0;i<actorsAndCharacters.size();++i){
        cout<<actorsAndCharacters.at(i).actorName<<" "<<actorsAndCharacters.at(i).characterName<<endl;
    }
    return 0;
}

// similar function to execute if user enters menu choice: 2
int MenuChoice2(vector<TitleRecord>& movieTitlesList, vector<NameRecord>& movieNamesList, vector<PrincipalRecord>& moviePrincipalsList,string actorSearchPhrase){
    // declaring variables
    vector<string> actorSearchPhraseWords;
    int actorSearchPhraseWordsCtr=1;
    int selectedActor;
    string actorNconst;
    vector<MovieAndCharacter> moviesAndCharacters;
    int actorSearchIndex=-1;
    vector<string> foundActorNames;
    bool actorFound = 1;
    bool matchFoundStatus=0;

    // converting search phrase to lowercase
    for (int i=0;i<actorSearchPhrase.size();++i){
        actorSearchPhrase.at(i)= tolower(actorSearchPhrase.at(i));
    }
    // counting number of words in search phrase
    for (int i=0;i<actorSearchPhrase.size();++i){
        if (actorSearchPhrase.at(i)=='+'){
            ++actorSearchPhraseWordsCtr;
        }
    }
    // splitting search phrase into a vector of words
    splitString(actorSearchPhrase,'+',actorSearchPhraseWords);
    for (int i=0;i<movieNamesList.size();++i) {
        string actorNameToLower = movieNamesList.at(i).primaryName;
        // converting actor names to lowercase for comparison
        for (int j = 0; j < actorNameToLower.size(); ++j) {
            actorNameToLower.at(j) = tolower(actorNameToLower.at(j));
        }
        // checking if every word in search phrase is found
        for (int k = actorSearchPhraseWordsCtr; k > 0; --k) {
            if (actorNameToLower.find(actorSearchPhraseWords.at(actorSearchPhraseWords.size() - k)) == string::npos) {
                actorFound = 0;
                break;
            }
            else{actorFound=1;}
        }
        // skipping actor if name doesn't match search
        if (actorFound == 0) { continue; }
        // changing match found status if even 1 record matches search phrase
        matchFoundStatus=1;
        // incrementing index before printing
        ++actorSearchIndex;
        // replacing commas with spaces for profession
        string profession;
        profession=movieNamesList.at(i).primaryProfession;
        for (int l=0;l<profession.size();++l){
            if (profession.at(l)==','){
                profession.insert(l+1," ");
            }
        }
        // printing the matching records
        cout << actorSearchIndex << ":" << endl << movieNamesList.at(i).primaryName << endl
             << profession << endl << "(" << movieNamesList.at(i).birthYear << "-"
             << movieNamesList.at(i).deathYear << ")" << endl << "----------------" << endl;
        foundActorNames.push_back(movieNamesList.at(i).nconst); // pushing actor name into vector
    }
    // checking if any match was found and printing appropriate message
    if (matchFoundStatus==0){cout<<"No match found!"; return 3;}
    cout<<"Select an actor/actress to see movies (-1 to go back to the previous menu): "<<endl;
    cin>>selectedActor;
    if (cin.fail()){
        return 0;
    }
    // checking if user tries to access a non existing index
    if (selectedActor>actorSearchIndex){
        cout<<"No match found!";
        return 3;
    }
    // checking if user wants to go back to previous menu
    if (selectedActor==-1){ return -1;}
    // creating struct object to store information
    MovieAndCharacter movieAndCharacter;
    actorNconst=foundActorNames.at(selectedActor);
    // storing information in struct object
    for (int i=0;i<moviePrincipalsList.size();++i){
        if (moviePrincipalsList.at(i).nconst==actorNconst){
            movieAndCharacter.tconst=moviePrincipalsList.at(i).tconst;
            movieAndCharacter.characterName=moviePrincipalsList.at(i).characters;
            moviesAndCharacters.push_back(movieAndCharacter);
        }
    }
    // iterating through file vector to look for movie title
    for (int i=0;i<moviesAndCharacters.size();++i) {
        for (int j = 0; j < movieTitlesList.size(); ++j) {
            if (movieTitlesList.at(j).tconst==moviesAndCharacters.at(i).tconst){
                moviesAndCharacters.at(i).movieName=movieTitlesList.at(j).primaryTitle;
                moviesAndCharacters.at(i).movieYear=movieTitlesList.at(j).startYear;
            }
        }
    }
    // printing movie title, year, and character played
    for (int i=0;i<moviesAndCharacters.size();++i){
        cout<<moviesAndCharacters.at(i).movieName<<" "<<moviesAndCharacters.at(i).movieYear<<" "<<moviesAndCharacters.at(i).characterName<<endl;
    }
    return 0;
}

/**********************************************************************************************************/

// function that runs inside main()
int run(string titlesFile, string namesFile, string principalsFile) {
    // declaring variables
    vector<string> movieTitlesFile;
    vector<TitleRecord> movieTitlesList;
    vector<string> movieNamesFile;
    vector<NameRecord> movieNamesList;
    vector<string> moviePrincipalsFile;
    vector<PrincipalRecord> moviePrincipalsList;
    int userMenuChoice;
    string movieSearchPhrase;
    string actorSearchPhrase;
    int run1Status;
    int run2Status;

    // reading files into vectors
    movieTitlesToVector(titlesFile,movieTitlesFile,movieTitlesList);
    movieNamesToVector(namesFile,movieNamesFile,movieNamesList);
    moviePrincipalsToVector(principalsFile,moviePrincipalsFile,moviePrincipalsList);

    // printing menu infinitely
    while (true){
        cout<<"Select a menu option:"<<endl<<"\t1. Search for movies"<<endl<<"\t2. Search for actors/actresses"<<endl<<"\t3. Exit"<<endl;
        cout<<"Your choice --> ";
        // accepting menu choice from the user
        cin>>userMenuChoice;
        if (cin.fail()){
            break;
        }
        // user choice is 1
        if (userMenuChoice==1){
            bool doneStatus=0;
            // accepting search phrase and calling function to implement when choice is 1
            cout<<"Enter search phrase: ";
            cin>>movieSearchPhrase;
            if (cin.fail()){
                break;
            }
            run1Status=MenuChoice1(movieTitlesList,movieNamesList,moviePrincipalsList,movieSearchPhrase);
            if (run1Status==3){ break;} // completely ending program if no match was found (because 3 was returned)
            // recursively calling function till user enters "done"
            while (run1Status!=-1 && doneStatus!=1) {
                cout << "Enter search phrase (type done to go back to the previous menu): ";
                cin>>movieSearchPhrase;
                if (cin.fail()){
                    break;
                }
                if (movieSearchPhrase=="done"){
                    doneStatus=1;
                }
                else{MenuChoice1(movieTitlesList,movieNamesList,moviePrincipalsList,movieSearchPhrase);}
            }
        }
        // user choice is 2
        else if(userMenuChoice==2){
            bool doneStatus=0;
            // accepting search phrase and calling function to implement when choice is 2
            cout<<"Enter search phrase: ";
            cin>>actorSearchPhrase;
            if (cin.fail()){
                break;
            }
            run2Status=MenuChoice2(movieTitlesList,movieNamesList,moviePrincipalsList,actorSearchPhrase);
            if (run2Status==3){ break;} // completely ending program if no match was found (because 3 was returned)
            // recursively calling function till user enters "done"
            while (run2Status!=-1 && doneStatus!=1){
                cout<<"Enter search phrase (type done to go back to the previous menu): ";
                cin>>actorSearchPhrase;
                if (cin.fail()){
                    break;
                }
                if (actorSearchPhrase=="done"){
                    doneStatus=1;
                }
                else{MenuChoice2(movieTitlesList,movieNamesList,moviePrincipalsList,actorSearchPhrase);}
            }
        }
        // breaking out if choice is 3
        else if(userMenuChoice==3){
            break;
        }
        // printing appropriate message if invalid choice was entered
        else {cout<<"Invalid choice!"<<endl;}
    }

    return 0;
}

int main() {
    // file locations
    string titlesFile =	"movie.titles.tsv";
    string namesFile = "movie.names.tsv";
    string principalsFile = "movie.principals.tsv";
    
    run(titlesFile, namesFile, principalsFile);
    return 0;
}
