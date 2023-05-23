#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

int rows = 15;
int columns = 35;
char board[50][120];

int level = 2;

int score;

int numberOfGhosts = 4;
class Ghost
{
public:
    int position[2];
    char direction = 'a';
    char newDirection = 'd';
    int targetPosition[2];
    int startPosition[2];
    bool powerMode = 0;
    char colorcode;
    int IQLevel;
    int level;

    char randDir()
    {
        int r = rand() % 4 + 1;
        switch (r)
        {
        case 1:
            return ('a');

        case 2:
            return ('s');

        case 3:
            return ('d');

        case 4:
            return ('w');

        default:
            return ('w');
            break;
        }
    }
    void setRandomTarget()
    {
        srand(time(0));
        targetPosition[0] = rand() % rows;
        targetPosition[1] = rand() % columns;
    }
    char setnewDirection()
    {
        if (direction == 'w' || direction == 's')
        {
            if (position[1] > targetPosition[1])
            {
                return ('a');
            }
            if (position[1] < targetPosition[1])
            {
                return ('d');
            }
            if (position[1] == targetPosition[1])
            {
                srand(clock());
                if (rand() % 2 == 0)
                {
                    return ('a');
                }
                return ('d');
            }
        }
        if (direction == 'd' || direction == 'a')
        {
            if (position[0] > targetPosition[0])
            {
                return ('w');
            }
            if (position[0] < targetPosition[0])
            {
                return ('s');
            }
            if (position[0] == targetPosition[0])
            {
                srand(clock());
                if (rand() % 2 == 0)
                {
                    return ('w');
                }
                return ('s');
            }
        }
    }
    void setTargetFollower(int pacman[2])
    {
        if (direction == 'w')
        {
            targetPosition[0] = pacman[0] + 3;
            targetPosition[1] = pacman[1];
        }
        if (direction == 's')
        {
            targetPosition[0] = pacman[0] - 3;
            targetPosition[1] = pacman[1];
        }
        if (direction == 'd')
        {
            targetPosition[0] = pacman[0];
            targetPosition[1] = pacman[1] - 3;
        }
        if (direction == 'a')
        {
            targetPosition[0] = pacman[0];
            targetPosition[1] = pacman[1] + 3;
        }
    }
    void setTargetLurker(int pacman[2])
    {
        if (direction == 'w')
        {
            targetPosition[0] = pacman[0] - 3;
            targetPosition[1] = pacman[1];
        }
        if (direction == 's')
        {
            targetPosition[0] = pacman[0] + 3;
            targetPosition[1] = pacman[1];
        }
        if (direction == 'd')
        {
            targetPosition[0] = pacman[0];
            targetPosition[1] = pacman[1] + 3;
        }
        if (direction == 'a')
        {
            targetPosition[0] = pacman[0];
            targetPosition[1] = pacman[1] - 3;
        }
    }
    bool ifNear(int pacman[], int range)
    {
        if ((position[0] - pacman[0] < range) || (pacman[0] - position[0] < range))
        {
            if ((position[1] - pacman[1] < range) || (pacman[1] - position[1] < range))
            {
                return (1);
            }
        }
        return (0);
    }
    bool ifWall()
    {
        if (direction == 'w')
        {
            if (board[position[0] - 1][position[1]] == char(219))
            {
                return (1);
            }
        }
        if (direction == 's')
        {
            if (board[position[0] + 1][position[1]] == char(219))
            {
                return (1);
            }
        }
        if (direction == 'd')
        {
            if (board[position[0]][position[1] + 1] == char(219))
            {
                return (1);
            }
        }
        if (direction == 'a')
        {
            if (board[position[0]][position[1] - 1] == char(219))
            {
                return (1);
            }
        }
        return (0);
    }
    bool checkAnotherWay()
    {
        if (direction == 'w' || direction == 's')
        {
            if (board[position[0]][position[1] + 1] != char(219))
            {
                return (1);
            }
            if (board[position[0]][position[1] - 1] != char(219))
            {
                return (1);
            }
        }
        if (direction == 'd' || direction == 'a')
        {
            if (board[position[0] + 1][position[1]] != char(219))
            {
                return (1);
            }
            if (board[position[0] - 1][position[1]] != char(219))
            {
                return (1);
            }
        }
    }
    void ifNearAccordingToLevel(int pacman[])
    {
        if (level == 1)
        {
            if (ifNear(pacman, 2))
            {
                targetPosition[0] = pacman[0];
                targetPosition[1] = pacman[1];
                newDirection = setnewDirection();
            }
        }
        if (level == 2)
        {
            if (ifNear(pacman, 6))
            {
                targetPosition[0] = pacman[0];
                targetPosition[1] = pacman[1];
                newDirection = setnewDirection();
            }
        }
        if (level == 3)
        {
            if (ifNear(pacman, 12))
            {
                targetPosition[0] = pacman[0];
                targetPosition[1] = pacman[1];
                newDirection = setnewDirection();
            }
        }
    }
    void chooseDirection1(int pacman[])
    {
        setTargetFollower(pacman);
        if (checkAnotherWay())
        {
            newDirection = setnewDirection();
            ifNearAccordingToLevel(pacman);
            if (position[0] >= rows - 8 && position[0] <= rows - 6 && position[1] >= columns - 12 && position[1] <= columns - 10)
            {
                targetPosition[0] = rows - 10;
                targetPosition[1] = columns - 11;
                newDirection = setnewDirection();
            }
            srand(clock());
            if (((rand() % 12) / 10) >= 1)
            {
                newDirection = randDir();
            }
        }
    }
    void chooseDirection2(int pacman[])
    {
        // lurker(pinky)
        setTargetLurker(pacman);
        if (ifWall())
        {
            newDirection = setnewDirection();
            ifNearAccordingToLevel(pacman);
            srand(clock());
            if (((rand() % 8) / 6) >= 1)
            {
                newDirection = randDir();
            }
        }
    }
    void chooseDirection3(int pacman[])
    {
        // random(inky)
        if (ifWall())
        {
            newDirection = randDir();
            if (rand() % 5 == 0)
            {
                setRandomTarget();
                newDirection = setnewDirection();
            }
            ifNearAccordingToLevel(pacman);
            srand(clock());
            if (((rand() % 8) / 6) >= 1)
            {
                newDirection = randDir();
            }
        }
    }
    void chooseDirection4(int pacman[])
    {
        // random(clyde)
        if (checkAnotherWay())
        {
            newDirection = randDir();
            if (rand() % 5 == 0)
            {
                setRandomTarget();
                newDirection = setnewDirection();
            }
            ifNearAccordingToLevel(pacman);
            srand(clock());
            if (((rand() % 8) / 4) >= 1)
            {
                newDirection = randDir();
            }
        }
    }
    void chooseDirectionInPowerMode()
    {
        if (newDirection == 'a')
        {
            newDirection = 'd';
            return;
        }
        if (newDirection == 'd')
        {
            newDirection = 'a';
            return;
        }
        if (newDirection == 's')
        {
            newDirection = 'w';
            return;
        }
        if (newDirection == 'w')
        {
            newDirection = 's';
            return;
        }
    }

    void chooseDirection(int pacman[])
    {
        if (IQLevel == 1)
        {
            chooseDirection1(pacman);
        }
        if (IQLevel == 2)
        {
            chooseDirection2(pacman);
        }
        if (IQLevel == 3)
        {
            chooseDirection3(pacman);
        }
        if (IQLevel == 4)
        {
            chooseDirection4(pacman);
        }
        if (powerMode)
        {
            chooseDirectionInPowerMode();
        }
    }
};
void chooseDirectionOfGhosts(Ghost ghost[], int pacman[])
{
    for (int i = 0; i < numberOfGhosts; i++)
    {
        ghost[i].chooseDirection(pacman);
    }
}

void setCursor(int x, int y)
{
    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(handle, coordinates);
}

void upToDown(int x, int y, int r, int c)
{
    while (x < r)
    {
        board[x][y] = ' ';
        x++;
    }
    if (y >= c)
    {
        while (y >= c)
        {
            board[x][y] = ' ';
            y--;
        }
    }
    else if (y <= c)
    {
        while (y <= c)
        {
            board[x][y] = ' ';
            y++;
        }
    }
}
void leftToRight(int x, int y, int r, int c)
{
    while (x < c)
    {
        board[y][x] = ' ';
        x++;
    }
    if (y > r)
    {
        while (y >= r)
        {
            board[y][x] = ' ';
            y--;
        }
        return;
    }
    if (y < r)
    {
        while (y <= r)
        {
            board[y][x] = ' ';
            y++;
        }
        return;
    }
    if (y == r)
    {
        board[y][x] = ' ';
    }
}

void goToDown(int x, int y)
{
    int i = y;
    while (i <= rows - 2)
    {
        board[i][x] = ' ';
        i++;
    }
}

void goToRight(int x, int y)
{
    int i = x;
    while (i <= columns - 2)
    {
        board[y][i] = ' ';
        i++;
    }
}

// tedad
int sotun = 1;
int radif = 1;
void matchBrances(int x[], int y[], bool right[], bool down[])
{
    for (int i = 0; i < sotun * radif; i++)
    {
        if (right[i])
        {
            leftToRight(x[i], y[i], y[i + 1], x[i + 1]);
        }
        else
        {
            goToRight(x[i], y[i]);
        }

        if (down[i])
        {
            upToDown(y[i], x[i], y[i + sotun], x[i + sotun]);
        }
        else
        {
            goToDown(x[i], y[i]);
        }
    }
}

void choseBrances()
{
    srand(time(0));

    int x[sotun * radif];
    int y[sotun * radif];
    bool right[sotun * radif];
    bool down[sotun * radif];

    int s = 1; // sotun
    int r = 1; // radif

    for (int i = 0; i < sotun * radif; i++)
    {
        x[i] = rand() % 4 + s;
        y[i] = rand() % 4 + r;
        // control
        board[y[i]][x[i]] = ' ';
        right[i] = 1;
        down[i] = 1;
        s += 5;
        if (i / sotun == radif - 1)
        {
            down[i] = 0;
        }
        if ((i + 1) % sotun == 0)
        {
            right[i] = 0;
            s = 1;
            r += 4;
        }
    }
    matchBrances(x, y, right, down);
}

void countBrances()
{
    sotun = 0;
    radif = 1;
    int i;
    i = 0;
    while (i + 5 < columns - 2)
    {
        i += 5;
        sotun++;
    }

    i = 0;
    while (i + 5 < rows - 2)
    {
        i += 5;
        radif++;
    }
}

void firstInitializeRandom()
{
    board[rows - 9][columns - 13] = '#';
    board[rows - 8][columns - 13] = '#';
    board[rows - 7][columns - 13] = '#';
    board[rows - 6][columns - 13] = '#';
    board[rows - 5][columns - 13] = '#';
    board[rows - 9][columns - 9] = '#';
    board[rows - 8][columns - 9] = '#';
    board[rows - 7][columns - 9] = '#';
    board[rows - 6][columns - 9] = '#';
    board[rows - 5][columns - 9] = '#';
    board[rows - 9][columns - 10] = '#';
    board[rows - 5][columns - 10] = '#';
    board[rows - 5][columns - 11] = '#';
    board[rows - 5][columns - 12] = '#';
    board[rows - 9][columns - 12] = '#';

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            board[i][j] = char(219);
        }
    }

    countBrances();

    choseBrances();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (i == 1 || j == 1 || i == rows - 2 || j == columns - 2)
            {
                board[i][j] = ' ';
            }
            if (i == 0 || j == 0 || i == rows - 1 || j == columns - 1)
            {
                board[i][j] = char(219);
            }
        }
    }

    board[rows - 8][columns - 12] = '.';
    board[rows - 8][columns - 11] = '.';
    board[rows - 8][columns - 10] = '.';
    board[rows - 7][columns - 12] = '.';
    board[rows - 7][columns - 11] = '.';
    board[rows - 7][columns - 10] = '.';
    board[rows - 6][columns - 12] = '.';
    board[rows - 6][columns - 11] = '.';
    board[rows - 6][columns - 10] = '.';
    board[rows - 9][columns - 11] = '.';

    int r, c;

    srand(time(0));
    r = rows - 3;
    c = (rand() % 10) + (columns / 2);
    while (true)
    {
        board[r][c] = ' ';
        r--;
        if (board[r][c] == ' ' || board[r][c] == '#')
        {
            break;
        }
    }

    r = rows - 1;
    c = (rand() % 5) + (columns / 2);
    board[r][c] = ' ';
    r = 0;
    board[r][c] = ' ';

    c = columns - 1;
    r = (rand() % 5) + (rows / 2);
    board[r][c] = ' ';
    c = 0;
    board[r][c] = ' ';

    int i = 5, j = 5;
    while (true)
    {
        board[i][j] = ' ';
        j++;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }
    i = 5;
    j = 5;
    while (true)
    {
        board[i][j] = ' ';
        j--;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }
    i = 5;
    j = 5;
    while (true)
    {
        board[i][j] = ' ';
        i--;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }

    i = rows - 10;
    j = columns - 11;
    while (true)
    {
        board[i][j] = ' ';
        j++;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }
    i = rows - 10;
    j = columns - 11;
    while (true)
    {
        board[i][j] = ' ';
        j--;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }
    i = rows - 10;
    j = columns - 11;
    while (true)
    {
        board[i][j] = ' ';
        i--;
        if (board[i][j] == ' ' || board[i][j] == '#')
        {
            break;
        }
    }
}

void powerFood(int x[], int y[])
{
    int r = rows / 3;
    int c = columns / 3;

    srand(clock());
    x[0] = rand() % (r) + 1;
    y[0] = rand() % (c) + 1;
    for (int i = 0; i < (r * c); i++)
    {
        if (board[x[0]][y[0]] == char(250))
        {
            board[x[0]][y[0]] = char(167);
            break;
        }
        if (x[0] == r && y[0] == c)
        {
            x[0] = 1;
        }
        if (y[0] == c)
        {
            x[0]++;
            y[0] = 1;
        }
        y[0]++;
    }
    x[1] = rand() % (r) + 1;
    y[1] = rand() % (c) + (1 + ((c)*2));
    for (int i = 0; i < (r * c); i++)
    {
        if (board[x[1]][y[1]] == char(250))
        {
            board[x[1]][y[1]] = char(167);
            break;
        }
        if (x[1] == r && y[1] == c * 3)
        {
            x[1] = 1;
        }
        if (y[1] == c * 3)
        {
            x[1]++;
            y[1] = c * 2;
        }
        y[1]++;
    }
    x[2] = rand() % r + r;
    y[2] = rand() % c + c;
    for (int i = 0; i < r * c; i++)
    {
        if (board[x[2]][y[2]] == char(250))
        {
            board[x[2]][y[2]] = char(167);
            break;
        }
        if (x[2] == r * 2 && y[2] == c * 2)
        {
            x[2] = r;
        }
        if (y[2] == c * 2)
        {
            x[2]++;
            y[2] = c;
        }
        y[2]++;
    }
    x[3] = rand() % r + r * 2;
    y[3] = rand() % c + 1;
    for (int i = 0; i < r * c; i++)
    {
        if (board[x[3]][y[3]] == char(250))
        {
            board[x[3]][y[3]] = char(167);
            break;
        }
        if (x[3] == r * 3 && y[3] == c)
        {
            x[3] = r * 2;
        }
        if (y[3] == c)
        {
            x[3]++;
            y[3] = 1;
        }
        y[3]++;
    }
    x[4] = rand() % r + r * 2;
    y[4] = rand() % c + c * 2;
    for (int i = 0; i < r * c; i++)
    {
        if (board[x[4]][y[4]] == char(250))
        {
            board[x[4]][y[4]] = char(167);
            break;
        }
        if (x[4] == r * 3 && y[4] == c * 3)
        {
            x[4] = r * 2;
        }
        if (y[4] == c * 3)
        {
            x[4]++;
            y[4] = c * 2;
        }
        y[4]++;
    }
}
void firstInitialize(int i[], int j[])
{

    // board[rows - 9][columns - 13] = '#';
    // board[rows - 8][columns - 13] = '#';
    // board[rows - 7][columns - 13] = '#';
    // board[rows - 6][columns - 13] = '#';
    // board[rows - 5][columns - 13] = '#';
    // board[rows - 9][columns - 9] = '#';
    // board[rows - 8][columns - 9] = '#';
    // board[rows - 7][columns - 9] = '#';
    // board[rows - 6][columns - 9] = '#';
    // board[rows - 5][columns - 9] = '#';
    // board[rows - 9][columns - 10] = '#';
    // board[rows - 5][columns - 10] = '#';
    // board[rows - 5][columns - 11] = '#';
    // board[rows - 5][columns - 12] = '#';
    // board[rows - 9][columns - 12] = '#';

    board[rows - 8][columns - 12] = '.';
    board[rows - 8][columns - 11] = '.';
    board[rows - 8][columns - 10] = '.';
    board[rows - 7][columns - 12] = '.';
    board[rows - 7][columns - 11] = '.';
    board[rows - 7][columns - 10] = '.';
    board[rows - 6][columns - 12] = '.';
    board[rows - 6][columns - 11] = '.';
    board[rows - 6][columns - 10] = '.';
    board[rows - 9][columns - 11] = '.';

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = char(250);
            }
            if (board[i][j] == '#')
            {
                board[i][j] = char(219);
            }
            if (board[i][j] == '.')
            {
                board[i][j] = ' ';
            }
        }
    }
    powerFood(i, j);
}

bool checkWin()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (board[i][j] == char(250) || board[i][j] == char(167))
            {
                return (0);
            }
        }
    }
    return (1);
}
int checkLose(Ghost ghost[], int pacman[])
{
    for (int i = 0; i < numberOfGhosts; i++)
    {
        if (pacman[0] == ghost[i].position[0] && pacman[1] == ghost[i].position[1])
        {
            if (ghost[i].powerMode)
            {
                ghost[i].powerMode = 0;
                ghost[i].position[0] = ghost[i].startPosition[0];
                ghost[i].position[1] = ghost[i].startPosition[1];
                return (2);
            }
            board[pacman[0]][pacman[1]] = ' ';
            return (1);
        }
    }
    return (0);
}
string changeToString(int num)
{
    string ret = "";
    string p = "";
    while (num)
    {
        p = (num % 10) + 48;
        p += ret;
        ret = p;
        num /= 10;
    }
    return (ret);
}
int changeToInt(string record)
{
    int r = 0;
    int tavan = 1;
    int n = record.length();
    for (int i = n - 1; i >= 0; i--)
    {
        r += (record[i] - 48) * tavan;
        tavan *= 10;
    }
    return (r);
}
void checkRecord(int score, int timer, string playerName[], string record[], string recordTime[])
{
    bool isRecord = 0;
    int r, t;
    string re = "", ti = "";
    re += changeToString(score);
    ti += changeToString(timer);
    string name;
    r = changeToInt(record[4]);
    if (score > r)
    {
        cout << endl;
        cout << "Enter your name: ";
        cin >> name;

        playerName[4] = name;
        record[4] = re;
        recordTime[4] = ti;
    }
    t = changeToInt(recordTime[4]);
    if (score == r && timer < t)
    {
        cout << endl;
        cout << "Enter your name: ";
        cin >> name;

        playerName[4] = name;
        record[4] = re;
        recordTime[4] = ti;
    }
    for (int i = 3; i >= 0; i--)
    {
        r = changeToInt(record[i]);
        t = changeToInt(recordTime[i]);
        if (score > r)
        {
            playerName[i + 1] = playerName[i];
            record[i + 1] = record[i];
            recordTime[i + 1] = recordTime[i];
            playerName[i] = name;
            record[i] = re;
            recordTime[i] = ti;
        }
        else if (score == r && timer < t)
        {
            playerName[i + 1] = playerName[i];
            record[i + 1] = record[i];
            recordTime[i + 1] = recordTime[i];
            playerName[i] = name;
            record[i] = re;
            recordTime[i] = ti;
        }
    }
}
void saveRecord(string playerName[], string record[], string recordTime[])
{
    ofstream fout("record.txt");
    for (int i = 0; i < 5; i++)
    {
        fout << playerName[i];
        fout << endl;
        fout << record[i];
        fout << endl;
        fout << recordTime[i];
        fout << endl;
    }
}
void finRecord(string playerName[], string record[], string recordTime[])
{
    ifstream file("record.txt");
    string temp;

    for (int i = 0; i < 5; i++)
    {
        getline(file, temp);
        playerName[i] += temp;

        getline(file, temp);
        record[i] += temp;

        getline(file, temp);
        recordTime[i] += temp;
    }
}

void printSuperSpeedTimer(clock_t superPowerTimer)
{
    cout << " speed:";
    cout << (8000 - (clock() - superPowerTimer)) / 1000;
}
void printPowerModeTimer(clock_t powerModeTimer)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x6);
    cout << "powermode remaining time: ";
    cout << (10000 - (clock() - powerModeTimer)) / 1000;
}
void printHeart(int heart)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x4);
    cout << heart << "x" << char(3);
    cout << "      ";
}
void printScore(int heart)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x6);
    cout << "score: " << score;
    int digit = 1;
    for (int i = 10; i < 1000000; i *= 10)
    {
        if (score / i == 0)
        {
            break;
        }
        digit++;
    }
    int space = (17 - digit) - 9;
    for (int i = 0; i < space; i++)
    {
        cout << " ";
    }
    printHeart(heart);
}
void printTimer(int timer[])
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x6);
    int timersec = timer[0];
    timer[1] = 0;
    while (timersec >= 60)
    {
        timer[1]++;
        timersec -= 60;
    }

    cout << "time: ";
    if (timer[1] <= 9)
    {
        cout << "0";
    }
    cout << timer[1];
    cout << ":";
    if (timersec <= 9)
    {
        cout << "0";
    }
    cout << timersec;
    cout << endl;
}
bool ghostPrinter(Ghost ghost[], int i, int j, int k)
{
    if (i == ghost[k].position[0] && j == ghost[k].position[1])
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 0x4);
        if (ghost[k].powerMode)
        {
            SetConsoleTextAttribute(h, 0xf1);
        }
        cout << char(1);
        return (1);
    }
    return (0);
}
void print(int timer[], Ghost ghost[], int heart, bool powermode, clock_t powerModeTimer, bool turbo, int turboPosition[], bool superSpeed, clock_t superPowerTimer)
{
    printScore(heart);
    printTimer(timer);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            SetConsoleTextAttribute(h, 0x2);
            if (powermode)
            {
                SetConsoleTextAttribute(h, 0xf2);
            }

            if (board[i][j] == 'O')
            {
                SetConsoleTextAttribute(h, 0x6);
                if (powermode)
                {
                    SetConsoleTextAttribute(h, 0x76);
                }
                if (superSpeed)
                {
                    SetConsoleTextAttribute(h, 0x46);
                }
                cout << char(2);
                continue;
            }

            if (board[i][j] == char(250))
            {
                SetConsoleTextAttribute(h, 0x7);
                if (powermode)
                {
                    SetConsoleTextAttribute(h, 0xf4);
                }
            }

            if (board[i][j] == char(167))
            {
                SetConsoleTextAttribute(h, 0x7);
                if (powermode)
                {
                    SetConsoleTextAttribute(h, 0xf4);
                }
            }

            bool cont = 0;
            for (int k = 0; k < numberOfGhosts; k++)
            {
                if (ghostPrinter(ghost, i, j, k))
                {
                    cont = 1;
                    break;
                }
            }
            if (cont)
            {
                continue;
            }
            if (turbo)
            {
                if (i == turboPosition[0] && j == turboPosition[1])
                {
                    SetConsoleTextAttribute(h, 0x1);
                    if (powermode)
                    {
                        SetConsoleTextAttribute(h, 0xf6);
                    }
                    cout << char(231);
                    continue;
                }
            }

            cout << board[i][j];
        }
        cout << endl;
    }
    if (powermode)
    {
        printPowerModeTimer(powerModeTimer);
    }
    else
    {
        for (int i = 0; i < 27; i++)
        {
            cout << " ";
        }
    }
    if (superSpeed)
    {
        printSuperSpeedTimer(superPowerTimer);
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            cout << " ";
        }
    }
}
char arrowKey()
{

    char p = getch();
    switch (p)
    {
    case 72:
        return ('w');

    case 80:
        return ('s');

    case 75:
        return ('a');

    case 77:
        return ('d');

    default:
        break;
    }
}
bool ifDirectionCorrect(int pacman[], char direction)
{
    switch (direction)
    {
    case 'd': // M or d
        if (pacman[1] == columns - 1)
        {
            board[pacman[0]][pacman[1]] = ' ';
            if (board[pacman[0]][0] == char(250))
            {
                score += 10;
            }
            pacman[1] = 0;
            board[pacman[0]][pacman[1]] = 'O';
            return (1);
        }
        if (board[pacman[0]][pacman[1] + 1] == char(219))
        {
            return (0);
        }
        board[pacman[0]][pacman[1]] = ' ';
        if (board[pacman[0]][pacman[1] + 1] == char(250))
        {
            score += 10;
        }
        board[pacman[0]][pacman[1] + 1] = 'O';
        pacman[1]++;
        return (1);
        break;
    case 'a': // K or a
        if (pacman[1] == 0)
        {
            board[pacman[0]][pacman[1]] = ' ';
            if (board[pacman[0]][columns - 1] == char(250))
            {
                score += 10;
            }
            pacman[1] = columns - 1;
            board[pacman[0]][pacman[1]] = 'O';
            return (1);
        }
        if (board[pacman[0]][pacman[1] - 1] == char(219))
        {
            return (0);
        }
        board[pacman[0]][pacman[1]] = ' ';
        if (board[pacman[0]][pacman[1] - 1] == char(250))
        {
            score += 10;
        }
        board[pacman[0]][pacman[1] - 1] = 'O';
        pacman[1]--;
        return (1);
        break;
    case 'w': // H or w
        if (pacman[0] == 0)
        {
            board[pacman[0]][pacman[1]] = ' ';
            if (board[rows - 1][pacman[1]] == char(250))
            {
                score += 10;
            }
            pacman[0] = rows - 1;
            board[pacman[0]][pacman[1]] = 'O';
            return (1);
        }
        if (board[pacman[0] - 1][pacman[1]] == char(219))
        {
            return (0);
        }
        board[pacman[0]][pacman[1]] = ' ';
        if (board[pacman[0] - 1][pacman[1]] == char(250))
        {
            score += 10;
        }
        board[pacman[0] - 1][pacman[1]] = 'O';
        pacman[0]--;
        return (1);
        break;
    case 's': // P or s
        if (pacman[0] == rows - 1)
        {
            board[pacman[0]][pacman[1]] = ' ';
            if (board[0][pacman[1]] == char(250))
            {
                score += 10;
            }
            pacman[0] = 0;
            board[pacman[0]][pacman[1]] = 'O';
            return (1);
        }
        if (board[pacman[0] + 1][pacman[1]] == char(219))
        {
            return (0);
        }
        board[pacman[0]][pacman[1]] = ' ';
        if (board[pacman[0] + 1][pacman[1]] == char(250))
        {
            score += 10;
        }
        board[pacman[0] + 1][pacman[1]] = 'O';
        pacman[0]++;
        return (1);
        break;
    default:
        return (0);
        break;
    }
}
bool ifDirectionCorrectGhost(int Ghost[2], char direction)
{
    switch (direction)
    {
    case 'd': // M or d
        if (Ghost[1] == columns - 1)
        {
            Ghost[1] = 0;
            return (1);
        }
        if (board[Ghost[0]][Ghost[1] + 1] == char(219))
        {
            return (0);
        }
        Ghost[1]++;
        return (1);
        break;
    case 'a': // K or a
        if (Ghost[1] == 0)
        {
            Ghost[1] = columns - 1;
            return (1);
        }
        if (board[Ghost[0]][Ghost[1] - 1] == char(219))
        {
            return (0);
        }
        Ghost[1]--;
        return (1);
        break;
    case 'w': // H or w
        if (Ghost[0] == 0)
        {
            Ghost[0] = rows - 1;
            return (1);
        }
        if (board[Ghost[0] - 1][Ghost[1]] == char(219))
        {
            return (0);
        }
        Ghost[0]--;
        return (1);
        break;
    case 's': // P or s
        if (Ghost[0] == rows - 1)
        {
            Ghost[0] = 0;
            return (1);
        }
        if (board[Ghost[0] + 1][Ghost[1]] == char(219))
        {
            return (0);
        }
        Ghost[0]++;
        return (1);
        break;
    default:
        return (0);
        break;
    }
}
void help()
{
    system("cls");
    getch();
}
void setting()
{
    while (1)
    {
        system("cls");
        cout << "1-board width  " << rows << "\n";
        cout << "2-board lenth  " << columns << "\n";
        cout << "3-number of ghosts  " << numberOfGhosts << "\n";
        cout << "4-level  " << level << "\n";
        cout << "5-back  " << endl;
        {
            char a = getch();
            switch (a)
            {
            case '1':
                cout << "board width: ";
                cin >> rows;
                break;
            case '2':
                cout << "board lenth ";
                cin >> columns;
                break;
            case '3':
                cout << "number of ghosts ";
                cin >> numberOfGhosts;
                break;
            case '4':
                cout << "level  ";
                cin >> level;
                break;
            case '5':
                return;
            }
        }
    }
}
void resetRecords(string playerName[], string record[], string recordTime[])
{
    ofstream fout("record.txt");
    for (int i = 0; i < 5; i++)
    {
        fout << "0";
        fout << endl;
        fout << "0";
        fout << endl;
        fout << "0";
        fout << endl;
    }
    for (int i = 0; i < 5; i++)
    {
        playerName[i] = "";
        record[i] = "";
        recordTime[i] = "";
    }
}
void sureToResetRecord(string playerName[], string record[], string recordTime[])
{
    while (1)
    {
        system("cls");
        cout << "are you sure to reset records?\n1-yes 2-no";
        char a = getch();
        switch (a)
        {
        case '1':
            resetRecords(playerName, record, recordTime);
            // saveRecord(playerName, record, recordTime);
            finRecord(playerName, record, recordTime);
            return;
            break;

        case '2':
            return;
            break;
        default:
            break;
        }
    }
}
void printRecords(string playerName[], string record[], string recordTime[])
{
    while (1)
    {
        system("cls");
        for (int i = 0; i < 5; i++)
        {
            cout << i + 1;
            cout << endl;
            cout << "player name: ";
            cout << playerName[i];
            cout << endl;
            cout << "score: ";
            cout << record[i];
            cout << endl;
            cout << "time(s): ";
            cout << recordTime[i];
            cout << endl;
            cout << endl;
        }
        cout << "1-back\n";
        cout << "2-reset records\n";
        char a = getch();
        switch (a)
        {
        case '1':
            return;
            break;
        case '2':
            sureToResetRecord(playerName, record, recordTime);
            return;
            break;

        default:
            break;
        }
    }
}
int pauseMeno(string playerName[], string record[], string recordTime[])
{
    while (true)
    {
        cout << "\n1-resume\n2-help\n3-records\n4-exit";
        char input = getch();
        if (input == '1')
        {
            break;
        }
        if (input == '2')
        {
            help();
        }
        if (input == '3')
        {
            printRecords(playerName, record, recordTime);
        }
        if (input == '4')
        {
            return (1);
        }
    }
    return (0);
}
int meno(string playerName[], string record[], string recordTime[])
{
    while (1)
    {
        system("cls");
        cout << "1-play game\n";
        cout << "2-setting\n";
        cout << "3-help\n";
        cout << "4-record\n";
        cout << "5-exit\n";
        {
            char a = getch();
            switch (a)
            {
            case '1':
                return (0);
            case '2':
                setting();
                break;
            case '3':
                help();
                break;
            case '4':
                printRecords(playerName, record, recordTime);
                break;
            case '5':
                sleep(1);
                return (4);
            }
        }
    }
}
bool checkPowerMode(Ghost ghost[], int pacman[2], int i[2], int j[2])
{
    if (pacman[0] == i[0] && pacman[1] == j[0])
    {
        i[0] = 0;
        j[0] = 0;
        return (1);
    }
    if (pacman[0] == i[1] && pacman[1] == j[1])
    {
        i[1] = 0;
        j[1] = 0;
        return (1);
    }
    if (pacman[0] == i[2] && pacman[1] == j[2])
    {
        i[2] = 0;
        j[2] = 0;
        return (1);
    }
    if (pacman[0] == i[3] && pacman[1] == j[3])
    {
        i[3] = 0;
        j[3] = 0;
        return (1);
    }
    if (pacman[0] == i[4] && pacman[1] == j[4])
    {
        i[4] = 0;
        j[4] = 0;
        return (1);
    }
    return (0);
}
void placeTurbo(int turboPosition[])
{
    srand(clock());
    int r = rand() % (rows * columns);
    int a = 0;

    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < columns - 1; j++)
        {
            if (board[i][j] == char(250))
            {
                if (a == r)
                {
                    turboPosition[0] = i;
                    turboPosition[1] = j;
                    return;
                }
                a++;
            }
        }
    }
}
bool checkSuperSpeed(int pacman[], int turboPosition[])
{
    if (pacman[0] == turboPosition[0] && pacman[1] == turboPosition[1])
    {
        return (1);
    }
    return (0);
}

void ghostMaker(Ghost ghost[])
{
    int position[2] = {rows - 8, columns - 12};
    for (int i = 0; i < numberOfGhosts; i++)
    {
        // for position and startPosition
        ghost[i].position[0] = position[0];
        ghost[i].position[1] = position[1];
        ghost[i].startPosition[0] = position[0];
        ghost[i].startPosition[1] = position[1];
        position[1]++;
        if (position[1] == columns - 9)
        {
            position[0]++;
            position[1] = columns - 12;
        }

        // for powerMode
        ghost[i].powerMode = 0;

        // for colorCode
        if (i % 4 == 0)
        {
            ghost[i].colorcode = '3';
        }
        if (i % 4 == 1)
        {
            ghost[i].colorcode = '4';
        }
        if (i % 4 == 2)
        {
            ghost[i].colorcode = '5';
        }
        if (i % 4 == 3)
        {
            ghost[i].colorcode = 'c';
        }

        // for IQLevel
        if (i % 2 == 0)
        {
            ghost[i].IQLevel = 3;
        }
        if (i % 2 == 1)
        {
            ghost[i].IQLevel = 4;
        }
        if (i == 0)
        {
            ghost[i].IQLevel = 1;
        }
        if (i == 1)
        {
            ghost[i].IQLevel = 2;
        }

        // for level
        ghost[i].level = level;
    }
}

void movementOfGhost(int turn, Ghost ghost[], bool superSpeed)
{
    for (int i = 0; i < numberOfGhosts; i++)
    {
        if (ghost[i].powerMode || superSpeed)
        {
            if (turn % 2 == 0)
            {
                if (ifDirectionCorrectGhost(ghost[i].position, ghost[i].newDirection))
                {
                    ghost[i].direction = ghost[i].newDirection;
                }
                else
                {
                    ifDirectionCorrectGhost(ghost[i].position, ghost[i].direction);
                }
            }
        }
        else
        {
            if (ifDirectionCorrectGhost(ghost[i].position, ghost[i].newDirection))
            {
                ghost[i].direction = ghost[i].newDirection;
            }
            else
            {
                ifDirectionCorrectGhost(ghost[i].position, ghost[i].direction);
            }
        }
    }
}

int main()
{
    while (true)
    {
        bool exit = 0;
        // variables for record
        string record[5];
        string recordTime[5];
        string playerName[5];
        finRecord(playerName, record, recordTime);

        int i[5], j[5];     // = x & y in powerFood function
        bool powerMode = 0; //
        clock_t powerModeTimer;
        int turn = 0;

        score = 0;

        char direction = '0';
        char newDirection = '0';

        int check;

        // start
        // cin >> rows >> columns >> level >> numberOfGhosts;

        if (meno(playerName, record, recordTime) == 4)
        {
            saveRecord(playerName, record, recordTime);
            return (0);
        }

        system("cls");
        firstInitializeRandom();
        firstInitialize(i, j);

        // these are for game timer
        int timer[2] = {0, 0}; // timer[0]=secound  timer[1]=minute
        long long int difTime = 0;
        clock_t timeStart = clock();
        clock_t timePause;
        clock_t timeEndPause;
        clock_t timeDif = 0;

        // add heatd after 10000 point
        int heartAdd = 1;

        int heart = 3;
        while (heart > 0)
        {

            bool superSpeed = 0;
            clock_t superSpeedTime;

            bool turbo = 0;
            int turboPosition[2];
            clock_t turboTime;
            turboPosition[0] = 0;
            turboPosition[1] = 0;

            powerMode = 0;
            int GhostEaten = 1;
            turn = 0;
            newDirection = ' ';
            direction = ' ';
            board[5][5] = 'O';

            // ghosts
            Ghost ghost[numberOfGhosts];
            ghostMaker(ghost);

            setCursor(0, 0);
            print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);
            int pacman[2] = {5, 5};
            clock_t time1, time2;
            time1 = clock();
            srand(clock());
            while (true)
            {

                if (score > heartAdd * 10000)
                {
                    heart++;
                    heartAdd++;
                }

                if (checkPowerMode(ghost, pacman, i, j))
                {
                    score += 50;
                    for (int k = 0; k < numberOfGhosts; k++)
                    {
                        ghost[k].powerMode = 1;
                    }
                    powerMode = 1;
                    // powerMode[0] = 1;
                    // powerMode[1] = 1;
                    // powerMode[2] = 1;
                    // powerMode[3] = 1;
                    powerModeTimer = clock();
                }
                if (clock() - powerModeTimer >= 10000)
                {
                    GhostEaten = 1;
                    for (int k = 0; k < numberOfGhosts; k++)
                    {
                        ghost[k].powerMode = 0;
                    }
                    powerMode = 0;
                    // powerMode[0] = 0;
                    // powerMode[1] = 0;
                    // powerMode[2] = 0;
                    // powerMode[3] = 0;
                }

                check = checkLose(ghost, pacman);
                if (check > 0)
                {
                    setCursor(0, 0);
                    print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);
                    timeDif += 3000;
                    sleep(3);
                    // clock_t lose = clock();
                    // while ((clock() - lose) < 3000)
                    // {
                    // }
                    if (check == 1)
                    {
                        break;
                    }
                    else
                    {
                        score += (200 * GhostEaten);
                        GhostEaten *= 2;
                        powerModeTimer += 3000;
                        superSpeedTime += 3000;
                        turboTime += 3000;
                        turn = 0;
                    }
                }
                setCursor(0, 0);
                print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);

                // turbo & super speed
                if (((((clock() - timeStart) - timeDif) / 2000) % 10) == 0)
                {
                    if (((clock() - timeStart) - timeDif) / 2000 > 0 && turbo != 1)
                    {
                        turboTime = clock();
                        placeTurbo(turboPosition);
                        turbo = 1;
                    }
                }
                if (clock() - turboTime > 10000 && turbo == 1)
                {
                    turbo = 0;
                    turboPosition[0] = 0;
                    turboPosition[1] = 0;
                }
                if (checkSuperSpeed(pacman, turboPosition))
                {
                    score += 30;
                    turbo = 0;
                    turboPosition[0] = 0;
                    turboPosition[1] = 0;
                    superSpeed = 1;
                    superSpeedTime = clock();
                }
                if (clock() - superSpeedTime > 8000 && superSpeed == 1)
                {
                    superSpeed = 0;
                }
                if (turboPosition[0] == 0 && turboPosition[1] == 0)
                {
                    turbo = 0;
                }

                timer[0] = ((clock() - timeStart) - timeDif) / 1000;

                if (kbhit())
                {
                    newDirection = getch();

                    if (newDirection == 0 || newDirection == 224)
                    {
                        newDirection = arrowKey();
                    }

                    if (newDirection == 'p')
                    {
                        timePause = clock();
                        if (pauseMeno(playerName, record, recordTime) == 1)
                        {
                            exit = 1;
                            break;
                        }
                        system("cls");
                        print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);
                        timeDif += clock() - timePause;

                        powerModeTimer += clock() - timePause;
                        superSpeedTime += clock() - timePause;
                        turboTime += clock() - timePause;
                    }
                }

                chooseDirectionOfGhosts(ghost, pacman);

                if (powerMode || superSpeed)
                {
                    if (turn % 2 == 0)
                    {
                        movementOfGhost(turn, ghost, superSpeed);
                    }
                }
                else
                {
                    movementOfGhost(turn, ghost, superSpeed);
                }

                check = checkLose(ghost, pacman);
                if (check > 0)
                {
                    setCursor(0, 0);
                    print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);
                    timeDif += 3000;
                    clock_t lose = clock();
                    while ((clock() - lose) < 3000)
                    {
                    }
                    if (check == 1)
                    {
                        break;
                    }
                    else
                    {
                        score += (200 * GhostEaten);
                        GhostEaten *= 2;
                        powerModeTimer += 3000;
                        superSpeedTime += 3000;
                        turboTime += 3000;
                        turn = 0;
                    }
                }

                // pacman
                if (ifDirectionCorrect(pacman, newDirection))
                {
                    direction = newDirection;
                }
                else
                {
                    ifDirectionCorrect(pacman, direction);
                }

                setCursor(0, 0);
                print(timer, ghost, heart, powerMode, powerModeTimer, turbo, turboPosition, superSpeed, superSpeedTime);
                time1 = clock();
                turn++;
                if (superSpeed)
                {
                    usleep(0.1 * 1000000);
                }
                else
                {
                    usleep(0.3 * 1000000);
                }
                if (checkWin())
                {
                    board[pacman[0]][pacman[1]] = ' ';
                    firstInitialize(i, j);
                    heart++;
                    timeDif += 3000;
                    sleep(3);
                    break;
                }
            }
            heart--;
            if (exit)
            {
                break;
            }
        }
        checkRecord(score, timer[0], playerName, record, recordTime);
        saveRecord(playerName, record, recordTime);

        system("cls");
        cout << "game over" << endl;
        cout << "your record: " << score << endl;
        printTimer(timer);

        sleep(4);
    }
}