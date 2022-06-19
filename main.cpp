#include <windows.h> // for color
#include <conio.h> // for getch()
#include <ctime> // for random
#include <iostream>
#include <mmsystem.h> // for sounds
#include<fstream> // for file
#include<string>

/*
***************************************************************************  LINE 105 AZ PLAYSOUND ESTEFADE KARDAM
DOWNLOAD THIS:  https://uupload.ir/view/k8lo_2048_-_seyedmahdijalali.zip/
***************************************************************************  LINE 105 AZ PLAYSOUND ESTEFADE KARDAM
*/

using namespace std;

// Keyboard
#define up 72
#define down 80
#define left 75
#define right 77
#define quit 113 // q
#define reset 114 // r
#define scoreboard 115 // s
#define logout 108 // l
#define backToGame 98 // b
#define modes 109 // m
#define undo1 117 // u

// **************** GLOBAL VARIABLES ******************
// score[] is the current score of the player in each mode
int row=4, col=4, user_counter=0, score[4], mode_Ind=4;
bool game_over= false;

int resetGame();
int startGame();

void add_number();
void select_mode();

struct user_data {
    // max_score is the max score of each mode which is saves in the file userDB.txt
    int player_id, max_score[4]; // max_score[0]: 4.4 - max_score[1]: 5.5 - max_score[2]: 6.6 - max_score[3]: 7.7
    int total_score;
    string username, password;
} user;

struct userScoreStr { // this is used for sorting the players scores
    string username;
    int score;
};

void home();
void signUp();
int auth(string username, string password);

void last_id();

void SetColor(int);

int random(int to);

// to generate 2 or 4
int initial_value();

// a pair of row and col indexes of frees
pair<int, int> freePosition();

void print_board(int);

void update_score(int, int); // takes the player_id and mode_Ind as parameters and updates player's score in userDB.txt

void delete_line(string, string);
int count_fileLines(string); // counts the number of lines in a file

string nthWord(int, string); // int-th word of the file

void scoreBoard(int);
void sort(userScoreStr arr[], int);

void delete_nthline(const char *, int); // removes the n-th line of the file char

void saveBoard(const char *, int, int);

void push_down();
void sum_down(int);
bool dead_down();

void push_up();
void sum_up(int);
bool dead_up();

void push_right();
void sum_right(int);
bool dead_right();

void push_left();
void sum_left(int);
bool dead_left();

// Game board
int board[7][7]= {0};

int main(){
    //PlaySound(TEXT("temple_of_love-sisters_of_mercy.wav"), NULL, SND_ASYNC); // play sound

    home();

    return 0;
}

// ****************************** CODE TAQIRE RANG RO AZ JAII COPY KARDAM **************************************************************
void SetColor(int ForgC){
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
// ****************************** CODE TAQIRE RANG RO AZ JAII COPY KARDAM **************************************************************

void home(){
    SetColor(15);
    system("cls");
    int menuOpt;
    char username[25], password[25];

    cout << "---------------- Welcome to 2048 ----------------\n";
    cout << "\tMenu: \n";
    cout << "\t<1> Sign up \n";
    cout << "\t<2> Log in \n";
    cout << "\t<3> Overall scoreboard \n";
    cout << "\t<4> Exit \n\n";

    cout << "\tPlease enter a number: ";
    cin >> menuOpt;

    switch(menuOpt){
    case 1:
        signUp();
        SetColor(10);
        cout << "\n\tsigned up successfully";
        Sleep(3000);
        home();
        break;
    case 2:
        cout << "\n\tPlease enter your username: ";
        cin >> username;
        cout << "\n\tPlease enter your password: ";
        cin >> password;

        // if such a user exist, then it return its value **** -1 is for no matching record
        if(auth(username, password)!=-1){
            user.player_id= auth(username, password);
            user.username= username;
            SetColor(10);
            cout << "\n\tloged in successfully\n";
            Sleep(3000);
            select_mode();
        }
        else{
            SetColor(12);
            cout << "\n\tYour log in info is not correct! Please try again\n";
            Sleep(3000);
            home();
        }
        break;
    case 3:
        scoreBoard(4); // Overall scoreboard
        break;
    case 4:
        exit(0);
        break;
    default:
        SetColor(12); // RED
        printf("\n\tIvalid input");
        Sleep(2000);
        home();
        break;
    }
}

// sets the user_counter to the last id of database
void last_id(){
    string line;
    ifstream input("userDB.txt");
    // auto increment of user_counter for user's id ****
    while(getline(input, line))
        user_counter++;
}

void signUp(){
    // CREATING or APPENDING to the database for user's login info and max score
    ofstream reg("userDB.txt", ios::app);

    last_id();

    user.player_id= user_counter;
    for(int i=0; i<4; i++)
        user.max_score[i]= 0; // default scores of all modes for a new user is set to 0
    user.total_score= 0;

    cout << "\n\tEnter your username: ";
    cin >> user.username;

    cout << "\n\tEnter your password: ";
    cin >> user.password;

    // Adding user's data into the userDB.txt
    reg << user.player_id << ' ' << user.username << ' ' << user.password << ' ' << user.max_score[0] << ' ' << user.max_score[1] << ' ' << user.max_score[2] << ' ' << user.max_score[3] << ' ' << user.total_score << endl;
    reg.close();
}

int auth(string username, string password){
    string id;
    ifstream read_file("userDB.txt");

    while(read_file >> id >> user.username >> user.password >> user.max_score[0] >> user.max_score[1] >> user.max_score[2] >> user.max_score[3] >> user.total_score){
        if(user.username==username && user.password==password){
            //max_score= stoi(file_score); // az forsat estefade kardam V bishtarin emtiaz fard ro az file khondam

            user.player_id= stoi(id);
            return stoi(id);
        }
    }
    read_file.close();
    // no matching record...
    return -1;
}

// 4*4->mode_Ind=0, 5*5->mode_Ind=1, ....
void update_score(int player_id, int mode_Ind){
    string file_id, file_username, file_password, file_score[4], file_total_score;

    bool we_have_max[4];
    for(int i=0; i<4; i++)
        we_have_max[i]= false;

    ifstream read_file("userDB.txt");
    // new data will be added
    while(read_file >> file_id >> file_username >> file_password >> file_score[0] >> file_score[1] >> file_score[2] >> file_score[3] >> file_total_score){
        if(stoi(file_id)==player_id){
            // max scores of each mode is being read form userDB.txt
            for(int i=0; i<4; i++)
                user.max_score[i]= stoi(file_score[i]);

            if(score[mode_Ind]>stoi(file_score[mode_Ind])){
                user.max_score[mode_Ind]= score[mode_Ind];
                we_have_max[mode_Ind]= true;
            }
            //else
                //score[mode_Ind]= stoi(file_score[mode_Ind]);
            break;
        }
    }
    read_file.close();

    user.total_score= user.max_score[0] + user.max_score[1] + user.max_score[2] + user.max_score[3];
    // the old record will be omited
    if(we_have_max[mode_Ind]){

        // delete the entire row
        delete_line("userDB.txt", to_string(player_id));
        ofstream update("userDB.txt", ios::app);

        last_id();

        update << player_id << ' ' << file_username << ' ' << file_password << ' ' << user.max_score[0] << ' ' << user.max_score[1] << ' ' << user.max_score[2] << ' ' << user.max_score[3] << ' ' << user.total_score << endl;
        update.close();
    }
}

void delete_line(string filename, string delete_id){
    string line;

    ifstream fin;
    fin.open(filename);
    ofstream temp;
    temp.open("temp.txt");

    while (getline(fin, line)){
        string id(line.begin(), line.begin() + line.find(" "));
        if(id != delete_id)
            temp << line << endl;
    }

    temp.close();
    fin.close();
    remove("userDB.txt");
    rename("temp.txt", "userDB.txt");
}

int count_fileLines(string fileName){
    int counter=0;
    string line;
    ifstream input(fileName);
    while(getline(input, line))
        counter++;
    return counter;
}

void select_mode(){
    SetColor(15);
    system("cls");
    int menuOpt;

    cout << "\tSelect 2048 grid: \n\n";
    cout << "\t1. Classic - 4 * 4 \n";
    cout << "\t2. Big - 5 * 5 \n";
    cout << "\t3. Bigger - 6 * 6 \n";
    cout << "\t4. Huge - 7 * 7 \n";
    cout << "\t5. Log out \n";
    cout << "\t6. Exit \n\n";

    cout << "\tPlease enter a number: ";
    cin >> menuOpt;

    switch(menuOpt){
    case 1:
        mode_Ind= 0;
        row= col= 4;
        startGame();
        break;
    case 2:
        mode_Ind= 1;
        row= col= 5;
        startGame();
        break;
    case 3:
        mode_Ind= 2;
        row= col= 6;
        startGame();
        break;
    case 4:
        mode_Ind= 3;
        row= col= 7;
        startGame();
        break;
    case 5:
        home();
        break;
    case 6:
        exit(0);
        break;
    default:
        SetColor(12); // RED
        printf("\n\tIvalid input");
        Sleep(2000);
        select_mode();
        break;
    }
}

void saveBoard(const char *file_name, int player_id, int mode_Ind){
    int line_counter= 0;
    string mix= to_string(player_id) + to_string(row), line;

    bool does_user_have_mode[4], file_exists= true;
    for(int i=0; i<4; i++)
        does_user_have_mode[i]= false;

    // Checking for the existance of the file
    fstream fileStream;
    fileStream.open(file_name);
    if (fileStream.fail()) {
        file_exists= false;
    }
    fileStream.close();

    ifstream read_file(file_name);

    while(!read_file.eof() && file_exists){
        getline(read_file, line);
        line_counter++;
        if(nthWord(1, line)==mix){
            does_user_have_mode[mode_Ind]= true;
            break;
        }
    }
    read_file.close();

    if(does_user_have_mode[mode_Ind]){
        delete_nthline(file_name, line_counter);
    }

    ofstream update(file_name, ios::app);
    update << mix << ' ' << score[mode_Ind];
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            update << ' ' << board[i][j];
        }
    }
    update << endl;
    update.close();
}

void delete_nthline(const char *file_name, int n){
    ifstream is(file_name);
	ofstream ofs;
	ofs.open("temp.txt", ofstream::out);

	char c;
	int line_no = 1;
	while (is.get(c)){
		if(c == '\n')
            line_no++;

		if(line_no != n)
			ofs << c;
	}

	ofs.close();
	is.close();
	remove(file_name);
	rename("temp.txt", file_name);
}

string nthWord(int index, string line){
    int count = 0;
    string word;
    for(int i = 0 ; i < line.length(); i++){
        if(line[i] == ' '){
            if(line[i+1] != ' '){
                count++;
                if(count == index){
                    return word;
                }
                word ="";
            }
        }
        else{
           word += line[i];
        }
    }
}

int resetGame(){
    int i, j;
    game_over= false;
    for(i=0; i<4; i++)
        score[i]= 0;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            board[i][j]=0;
        }
    }

    // 2 initial values at the begining of the game
    add_number();
    add_number();
    saveBoard("undoDB.txt", user.player_id, mode_Ind);
    saveBoard("boardDB.txt", user.player_id, mode_Ind);
}

int startGame(){
    string mix= to_string(user.player_id) + to_string(row), line;

    int dir = 0, temp, nth=3;
    bool start_fresh= true, file_exists= true;

    if(row==4)
        mode_Ind=0;
    else if(row==5)
        mode_Ind=1;
    else if(row==6)
        mode_Ind=2;
    else if(row==7)
        mode_Ind=3;

    // Checking for the existance of the file
    fstream fileStream;
    fileStream.open("boardDB.txt");
    if (fileStream.fail()) {
        file_exists= false;
    }
    fileStream.close();


    ifstream read_file("boardDB.txt");

    // Reading data from boardDB.txt to recover the board numbers for the rest of the game
    while(!read_file.eof() && file_exists){
        getline(read_file, line);
        if(nthWord(1, line)==mix){
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    board[i][j]= stoi(nthWord(nth, line));
                    nth++;
                }
            }
            score[mode_Ind]= stoi(nthWord(2, line));
            start_fresh= false;
            break;
        }
    }
    read_file.close();

    if(start_fresh){ // user doesn't have an unfinished game. so we should add 2 initial values to start
        add_number();
        add_number();
    }

    update_score(user.player_id, mode_Ind);
    print_board(mode_Ind);

    while(!game_over){
        if(dead_up() && dead_right() && dead_down() && dead_left())
            game_over= true;

        print_board(mode_Ind);

        temp= getch(); // to store 0
        dir = getch();

        switch(dir){
        case up:
            if(!dead_up()){
                saveBoard("undoDB.txt", user.player_id, mode_Ind); // It stores the numbers berfore the new move for undo
                push_up();
                sum_up(mode_Ind);
                add_number();
                saveBoard("boardDB.txt", user.player_id, mode_Ind); // It stores the numbers after the new move
            }
            break;
        case down:
            if(!dead_down()){
                saveBoard("undoDB.txt", user.player_id, mode_Ind);
                push_down();
                sum_down(mode_Ind);
                add_number();
                saveBoard("boardDB.txt", user.player_id, mode_Ind);
            }
            /*
            else
                cout << "down is fucking dead" << endl;
            */
            break;
        case left:
            if(!dead_left()){
                saveBoard("undoDB.txt", user.player_id, mode_Ind);
                push_left();
                sum_left(mode_Ind);
                add_number();
                saveBoard("boardDB.txt", user.player_id, mode_Ind);
            }
            break;
        case right:
            if(!dead_right()){
                saveBoard("undoDB.txt", user.player_id, mode_Ind);
                push_right();
                sum_right(mode_Ind);
                add_number();
                saveBoard("boardDB.txt", user.player_id, mode_Ind);
            }
            break;
        case quit:
            exit(0);
            break;
        case reset:
            resetGame();
            break;
        case logout:
            home();
            break;
        case backToGame:
            startGame();
            break;
        case modes:
            select_mode();
            break;
        case scoreboard:
            scoreBoard(mode_Ind);
            break;
        case undo1:
            { // I wraped the case into {} because of "jump to case label -fpermissive" error
                file_exists= true;
                nth= 3;

                fstream fileStream;
                fileStream.open("undoDB.txt");
                if (fileStream.fail()) {
                    file_exists= false;
                }
                fileStream.close();

                ifstream read_file("undoDB.txt");
                while(!read_file.eof() && file_exists){
                    getline(read_file, line);
                    if(nthWord(1, line)==mix){
                        for(int i=0; i<row; i++){
                            for(int j=0; j<col; j++){
                                board[i][j]= stoi(nthWord(nth, line));
                                nth++;
                            }
                        }
                        score[mode_Ind]= stoi(nthWord(2, line));
                        break;
                    }
                }
                read_file.close();
                break;
            }
        default:
            SetColor(12); // RED
            cout << "\nInvalid input!\n" << endl;
            SetColor(15); // white
            break;
        }
    }

    if(game_over){
        resetGame();
        int menuOpt;
        cout << "\n\tgame is over!\n";
        cout << "\n\t 1. Modes \t 2. scores \n";
        cout << "\n\t 3. logout\t 4. quit \n";
        cout << "\n\t Enter your choice: ";
        cin >> menuOpt;

        switch(menuOpt){
        case 1:
            select_mode();
            break;
        case 2:
            scoreBoard(mode_Ind);
            break;
        case 3:
            home();
            break;
        case 4:
            exit(0);
            break;
        default:
            cout << "\n\tInvalid input!\n";
            break;
        }
    }
}

// Adds new numbers to the board
void add_number(){
    pair<int, int> index= freePosition();
    board[index.first][index.second]= initial_value();
    // cout << "i: " << index.first << "\t j: " << index.second << endl;
}

// random numbers from 0 to ceil
int random(int ceil){
    srand(time(0));
    return rand() % ceil;
}

int initial_value(){
    int initial[]= {2, 4};
    return initial[random(1)];
}

pair<int, int> freePosition(){
    int i, j;
    bool is_full=true;
    while(is_full){
        i= random(row);
        j= random(col);
        if(board[i][j]==0){
            is_full= false;
        }
    }
    return make_pair(i, j);
}

void print_board(int mode_Ind){

    int i, j, line_counter= 0;

    system("cls");

    SetColor(11);
    cout << "\n\n\t\tSCORE  " << score[mode_Ind] << "\t YOUR BEST  " << user.max_score[mode_Ind] << "\n\n\n";
    SetColor(15);

    for(i=0; i<row; i++){
        cout << "\t\t";
        for(j=0; j<col; j++){
            if(board[i][j]==0)
                SetColor(15); // BLACK
            else if(board[i][j]==2 || board[i][j]==4)
                SetColor(14); // YELLOW
            else if(board[i][j]>=8 && board[i][j]<=64)
                SetColor(11); // LIGHT BLUE
            else if(board[i][j]>=128 && board[i][j]<=1024)
                SetColor(13); // PURPLE
            else if(board[i][j]>1024)
                SetColor(12); // RED
            else
                SetColor(15); // WHITE -> the default

            cout << " " << board[i][j] << " \t";
        }
        SetColor(15);
        if(mode_Ind==0)
            cout << "\n\t\t---------------------------\n\n";
        else if(mode_Ind==1)
            cout << "\n\t\t-----------------------------------\n\n";
        else if(mode_Ind==2)
            cout << "\n\t\t--------------------------------------------\n\n";
        else if(mode_Ind==3)
            cout << "\n\t\t---------------------------------------------------\n\n";
    }

    SetColor(11);
    cout << "\n\n\t\t u - undo    \t r - restart";
    cout << "\n\n\t\t s - scores  \t m - modes";
    cout << "\n\n\t\t l - log out \t q - quit \n";
    SetColor(15);
}

void sort(userScoreStr arr[], int arraySize){
    int i, j, imax;
    userScoreStr max;

    for(i=0; i<arraySize; i++){
        max= arr[i];
        imax= i;
        for(j=i+1; j<arraySize; j++){
            if(arr[j].score > max.score){
                max= arr[j];
                imax= j;
            }
        }
        arr[imax]= arr[i];
        arr[i]= max;
    }
}

void scoreBoard(int index){
    userScoreStr USD[count_fileLines("userDB.txt")];

    int menuOpt, counter=0;
    string bs, file_username, file_score;
    system("cls");
    SetColor(12); // RED

    ifstream read_file("userDB.txt");

    if(index==0)
        cout << "\n\t\t--- 4 * 4 SCOREBOARD ---\n\n";
    else if(index==1)
        cout << "\n\t\t--- 5 * 5 SCOREBOARD ---\n\n";
    else if(index==2)
        cout << "\n\t\t--- 6 * 6 SCOREBOARD ---\n\n";
    else if(index==3)
        cout << "\n\t\t--- 7 * 7 SCOREBOARD ---\n\n";
    else
        cout << "\n\t\t--- OVERALL SCOREBOARD ---\n\n";

    SetColor(13); // PURPLE
    cout << "\n\t\t # \t Username \t Score \n";
    cout << "\n\t\t -------------------------------- \n\n";
    SetColor(14); // YELLOW

    if(index==0){ // 4 * 4
        while(read_file >> bs >> file_username >> bs >> file_score >> bs >> bs >> bs >> bs){
            USD[counter].username= file_username;
            USD[counter].score= stoi(file_score);
            counter++;
        }
    }
    else if(index==1){ // 5 * 5
        while(read_file >> bs >> file_username >> bs >> bs >> file_score >> bs >> bs >> bs){
            USD[counter].username= file_username;
            USD[counter].score= stoi(file_score);
            counter++;
        }
    }
    else if(index==2){ // 6 * 6
        while(read_file >> bs >> file_username >> bs >> bs >> bs >> file_score >> bs >> bs){
            USD[counter].username= file_username;
            USD[counter].score= stoi(file_score);
            counter++;
        }
    }
    else if(index==3){ // 7 * 7
        while(read_file >> bs >> file_username >> bs >> bs >> bs >> bs >> file_score >> bs){
            USD[counter].username= file_username;
            USD[counter].score= stoi(file_score);
            counter++;
        }
    }
    else if(index==4){ // All modes
        while(read_file >> bs >> file_username >> bs >> bs >> bs >> bs >> bs >> file_score){
            USD[counter].username= file_username;
            USD[counter].score= stoi(file_score);
            counter++;
        }
    }


    bool file_exists= true;
    // Checking for the existance of the file
    fstream fileStream;
    fileStream.open("userDB");
    if (fileStream.fail()) {
        file_exists= false;
    }
    fileStream.close();


    sort(USD, count_fileLines("userDB.txt"));
    for(int i=0; i<counter; i++)
        cout << "\n\t\t " << i+1 <<" \t " << USD[i].username << " \t\t " << USD[i].score <<endl;
    if(!file_exists){
        cout << "\n\t\t Be the first person who sets a record...\n";
    }

    read_file.close();

    SetColor(11); // LIGHT BLUE
    cout << "\n\n\n\t\t 1. back \t 2. quit\n\n";
    cout << "\t\t Enter your choice: ";
    cin >> menuOpt;
    switch(menuOpt){

    case 1:
        if(mode_Ind==4)
            home();
        else
            startGame();
        break;
    case 2:
        exit(0);
        break;
    default:
        SetColor(12); // LIGHT BLUE
        cout << "\n\n\t\t Invalid input! \n\n";
        Sleep(2000);
        home();
    }
    SetColor(15); // WHITE
}

void push_down(){
    int i, j, k;

    for(k=0; k<row; k++){
        for(i=row-1; i>0; i--){
            for(j=0; j<col; j++){
                if(board[i][j]==0 && board[i-1][j]!=0){
                    board[i][j]= board[i-1][j];
                    board[i-1][j]= 0;
                }
            }
        }
    }
}

void sum_down(int mode_Ind){
    int i, j, sum=0;

    for(i=row-1; i>0; i--){
        for(j=0; j<col; j++){
            if(board[i][j]==board[i-1][j] && board[i][j]!=0 && board[i-1][j]!=0){
                sum= board[i][j] + board[i-1][j];
                board[i][j]= sum; // payyineie mishe sum
                // pushing other numbers down
                for(int k=i-1; k>=1; k--){
                    board[k][j]= board[k-1][j];
                }
                board[0][j]= 0; // satre aval mishe 0
                score[mode_Ind]+= sum;
                SetColor(12);
                cout << "\n\t\t Wait..." << endl;
                SetColor(15);
                update_score(user.player_id, mode_Ind);
            }
        }
    }
}

bool dead_down(){
    int i, j, k;

    for(i=0; i<row-1; i++){
        for(j=0; j<col; j++){
            // cout << "i: " << i << "\t j: " << j << endl;
            // age hade aqal ye sefr paiintar az khodesh dashte bashe
            for(k=i+1; k<row; k++){
                if(board[i][j]!=0 && board[k][j]==0){
                    // cout << "k: " << k << endl;
                    return false;
                }
            }
            // age mohreha kenare ham bashan
            if(board[i][j]==board[i+1][j] && board[i][j]!=0 && board[i+1][j]!=0){
                // cout << "returns fucking false 2" << endl;
                return false;
            }
        }
    }
    // cout << "returns fucking true" << endl;
    return true;
}

void push_up(){
    int i, j, k;

    for(k=0; k<row; k++){
        for(i=row-1; i>0; i--){
            for(j=0; j<col; j++){
                if(board[i][j]!=0 && board[i-1][j]==0){
                    board[i-1][j]= board[i][j];
                    board[i][j]= 0;
                }
            }
        }
    }
}

void sum_up(int mode_Ind){
    int i, j, sum=0;

    for(i=1; i<row; i++){
        for(j=0; j<col; j++){
            if(board[i][j]==board[i-1][j] && board[i][j]!=0 && board[i-1][j]!=0){
                sum= board[i][j] + board[i-1][j];
                board[i-1][j]= sum; // sum toye balaii
                for(int k=i; k<row-1; k++){
                    board[k][j]= board[k+1][j];
                }
                board[row-1][j]=0; // satre akhar mishe 0
                score[mode_Ind]+= sum;
                SetColor(12);
                cout << "\n\t\t Wait..." << endl;
                SetColor(15);
                update_score(user.player_id, mode_Ind);
            }
        }
    }
}

bool dead_up(){
    int i, j, k;

    for(i=row-1; i>=1; i--){
        for(j=0; j<col; j++){
            for(k=i-1; k>=0; k--){
                if(board[i][j]!=0 && board[k][j]==0){
                    return false;
                }
            }
            if(board[i][j]==board[i-1][j] && board[i][j]!=0 && board[i-1][j]!=0){
                return false;
            }
        }
    }
    return true;
}

void push_right(){
    int i, j, k;

    for(k=0; k<row; k++){
        for(i=0; i<row; i++){
            for(j=col-1; j>0; j--){
                if(board[i][j]==0 && board[i][j-1]!=0){
                    board[i][j]= board[i][j-1];
                    board[i][j-1]= 0;
                }
            }
        }
    }
}

void sum_right(int mode_Ind){
    int i, j, sum=0;

    for(i=0; i<row; i++){
        for(j=col-1; j>0; j--){
            if(board[i][j-1]==board[i][j] && board[i][j-1]!=0 && board[i][j]!=0){
                sum= board[i][j] + board[i][j-1];
                board[i][j]= sum;
                for(int k=j-1; k>=1; k--){
                    board[i][k]= board[i][k-1];
                }
                board[i][0]= 0;
                score[mode_Ind]+= sum;
                SetColor(12);
                cout << "\n\t\t Wait..." << endl;
                SetColor(15);
                update_score(user.player_id, mode_Ind);
            }
        }
    }
}

bool dead_right(){
    int i, j, k;

    for(i=0; i<row; i++){
        for(j=0; j<col-1; j++){
            for(k=j+1; k<col; k++){
                if(board[i][j]!=0 && board[i][k]==0){
                    return false;
                }
            }
            if(board[i][j]==board[i][j+1] && board[i][j]!=0 && board[i][j+1]!=0){
                return false;
            }
        }
    }
    return true;
}


void push_left(){
    int i, j, k;

    for(k=0; k<row; k++){
        for(i=0; i<row; i++){
            for(j=1; j<col; j++){
                if(board[i][j]!=0 && board[i][j-1]==0){
                    board[i][j-1]= board[i][j];
                    board[i][j]= 0;
                }
            }
        }
    }
}

void sum_left(int mode_Ind){
    int i, j, sum=0;

    for(i=0; i<row; i++){
        for(j=1; j<col; j++){
            if(board[i][j-1]==board[i][j] && board[i][j-1]!=0 && board[i][j]!=0){
                sum= board[i][j] + board[i][j-1];
                board[i][j-1]= sum;
                for(int k=j; k<col-1; k++){
                    board[i][k]= board[i][k+1];
                }
                board[i][col-1]= 0;
                score[mode_Ind]+= sum;
                SetColor(12);
                cout << "\n\t\t Wait..." << endl;
                SetColor(15);
                update_score(user.player_id, mode_Ind);
            }
        }
    }
}

bool dead_left(){
    int i, j, k;

    for(i=0; i<row; i++){
        for(j=col-1; j>=1; j--){
            for(k=j-1; k>=0; k--){
                if(board[i][j]!=0 && board[i][k]==0){
                    return false;
                }
            }
            if(board[i][j]==board[i][j-1] && board[i][j]!=0 && board[i][j-1]!=0){
                return false;
            }
        }
    }
    return true;
}
