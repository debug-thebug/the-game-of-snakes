#include<iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

char mat[1500];
int pos;
int lastpos;
int len;
int lastmove;
int snake[100];
int lev;

void clrscr()       //clrscr fn
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

bool collision(int p)       //check if the snake hit something
{
    if(mat[p]=='-' || mat[p]=='*' || mat[p] == '|')
        return true;
    else
        return false;
}

void transpose()        //change dirn of various body parts
{
    for(int i=len-2; i>=0; i--)
        snake[i+1] = snake[i];

    if(snake[len-1] == 72)
        lastpos -= 80;
    else if(snake[len-1] == 80)
        lastpos += 80;
    else if(snake[len-1] == 75)
        lastpos--;
    else
        lastpos++;
    return;
}

void print()
{
    //system("cls");
    clrscr();	//go to the first pixel again (does not actually clear screen, just overwrites)
    for(int i=0; i<1280; i++)
        cout<<mat[i];
}

void gen()  //generate food for snake
{
    int r = rand()%1040 + 160;  //1040/80 rows and leave the first two rows
    while(mat[r] != ' ')        //so that food is not gen on border
        r = rand()%1040 + 160;
    mat[r] = '#';
}

bool check(int p)       //check if snake eats food
{
    if(mat[p] == '#')
    {
        len++;
        snake[len-1] = snake[len-2];

        if(snake[len-1] == 72)  //72 means up i.e. earlier tail moving up
            lastpos += 80;      //if earlier tail was going up, now the lastpos i.e. new tail of snake is at next line same col next row.
        else if(snake[len-1] == 80) //down
            lastpos -= 80;
        else if(snake[len-1] == 75)
            lastpos++;
        else
            lastpos--;

        gen();
        return true;
    }
    return false;
}

void block()
{
    int past = 0;
    while(true)
    {
        Sleep(lev);
        if(_kbhit())
        {
            /*
            #define 72 up
            #define 80 down
            #define 75 left
            #define 77 right
            */
            char key;
            key = _getch(); //Now this click is key

            switch(key)
            {
               case 72:             //up
                    if(past != 80)  //if the snake is going down it should not go up
                    {
                        snake[0] = past;    //save old direction of head. See above in loop we update all dirn but not of head i.e. top most so we do it here
                    	transpose();
                        mat[pos] = '*';     //earlier head 'O' should now have *
                        pos -= 80;          //update pos (posn of head)
                        lastmove = -80;     //used when we do not change dirn of snake (see else)
                        past = 72;          //update past so we can check if next move if possible or not
                    }
                    break;
               case 80:
                   if(past != 72)   //down
                    {
                        snake[0] = past;
                    	transpose();
                        mat[pos] = '*';
                        pos += 80;
                        lastmove = 80;
                        past = 80;
                    }
                    break;
               case 75:             //left
                    if(past != 77)
                    {
                        snake[0] = past;
                    	transpose();
                        mat[pos] = '*';
                        pos -= 1;
                        lastmove = -1;
                        past = 75;
                    }
                    break;
               case 77:             //right
                    if(past != 75)
                    {
                        snake[0] = past;
                    	transpose();
                        mat[pos] = '*';
                        pos += 1;
                        lastmove = 1;
                        past = 77;
                    }
                    break;
            }

            if(check(pos) == false) //if we are not eating anything, tail should move fwd so
                mat[lastpos] = ' ';
            if(collision(pos) == true)  //dead bruh
            {
                mat[pos] = 'X';
                print();
                break;
            }
            mat[pos] = 'O';     //print head
            print();
        }
        else        //if no keys are pressed
        {
            snake[0] = past;
            mat[pos] = '*';
            pos += lastmove;

            if(check(pos) == false)     //smart move! no need to move the whole snake, just add * in head and remove last position i.e. tail
                mat[lastpos] = ' ';

            if(collision(pos)==true)
            {
                mat[pos] = 'X';
                print();
                break;
            }
            mat[pos] = 'O';
            transpose();
            print();
        }
    }
}

void beginning()
{
    for(int i=0; i<1280; i++)       //set up the display screen
    {
        if(i<80)
            mat[i] = ' ';
        else if(i>=80 && i<160)
            mat[i] = '-';
        else if(i>=160 && i<1200)
            mat[i] = ' ';
        else
            mat[i] = '-';
    }

    int temp = 80;
    while(temp<1120)
    {
        temp += 80;
        mat[temp] = '|';
        if(temp != 160)
            mat[temp-1] = '|';
    }
    temp += 80;
    mat[temp-1] = '|';

    pos = rand()%1040 + 160;        //0 to 1039 + 160 => 160 to 1199
    len = 3;
    lastpos = pos - len + 1;        //0 indexing

    for(int i=0; i<len; i++)
        snake[i] = 77;

    mat[pos] = 'O';
    for(int i=1; i<len; i++)
        mat[pos-i] = '*';

    lastmove = 1;
    return;
}

void level()
{
    int no;
    cout<<"Choose a level: \n1. Easy\n2. Medium\n3. Hard\n4. Crazy\n\nSelect Choice: ";
    cin>>no;

    lev = 0;
    switch(no)
    {
    case 1:
        lev = 30;
        break;
    case 2:
        lev = 20;
        break;
    case 3:
        lev = 10;
        break;
    case 4:
        lev = 5;
        break;
    default:
        cout<<"Please enter a valid choice!"<<endl;
    }
    return;
}

bool cont()
{
    cout<<"Do you wish to play again? (y/n): ";
    char c;
    cin>>c;
    if(c=='y' || c=='Y')
        return true;
    else
        return false;
}

int main()
{
    srand (time(NULL));
    while(true)
    {
        system("cls");
        clrscr();
        lev = 0;
        while(lev == 0)
            level();
        beginning();
        gen();
        print();
        block();
        if(cont() == false)
            break;
    }

    return 0;
}
