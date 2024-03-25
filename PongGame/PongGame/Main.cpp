#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std::this_thread;
using namespace std::chrono;

const int GRIDWIDTH = 20;
const int GRIDHEIGHT = 15;

int TransformBallToPaddle(int yPos) {
    return yPos * 2 + 1;
}

int GetRandDirection() {
    if (rand() % 4 >= 2) {
        return 1;
    }
    else {
        return -1;
    }
}

class Ball {
private:
    int x = 0;
    int y = 0;
    int xSpeed = 0;
    int ySpeed = 0;
public:
    Ball(int xBallPos, int yBallPos, int xBallSpeed, int yBallSpeed) : x(xBallPos), y(yBallPos), xSpeed(xBallSpeed), ySpeed(yBallSpeed) {}

    void Reset(int xBallPos, int yBallPos, int xBallSpeed, int yBallSpeed) {
        x = xBallPos;
        y = yBallPos;
        xSpeed = xBallSpeed;
        ySpeed = yBallSpeed;
    }

    int GetX() {
        return x;
    }

    int GetY() {
        return y;
    }

    int GetXSpeed() {
        return xSpeed;
    }

    int GetYSpeed() {
        return ySpeed;
    }

    void Bounce(bool xAxisBounce, bool yAxisBounce) {
        if (xAxisBounce) {
            xSpeed *= -1;
        }
        if (yAxisBounce) {
            ySpeed *= -1;
        }
    }

    void Update() {
        x += xSpeed;
        y += ySpeed;
    }
};

class Paddle {
private:
    bool isComputer = false;
    bool hasTarget = false;
    Ball* target;
    int x = 0;
    int y = 0;
    int topBound = 0;
    int lowerBound = 0;
public:
    Paddle(bool computer, int yPos, int xPos, int yLowerBound, int yTopBound) : isComputer(computer), y(yPos), x(xPos), lowerBound(yLowerBound), topBound(yTopBound) {}

    void Reset(int yPos) {
        y = yPos;
    }

    void SetTarget(Ball* ball) {
        target = ball;
        hasTarget = true;
    }

    int GetY() {
        return y;
    }

    void Move(bool moveUpwards) {
        if (moveUpwards) {
            y--;
            if (y <= lowerBound) {
                y = lowerBound + 1;
            }
        }
        else {
            y++;
            if (y >= topBound) {
                y = topBound - 1;
            }
        }
    }

    void Update() {
        if (isComputer) {
            if (hasTarget) {
                int dist = x - target->GetY();
                if (target->GetYSpeed() < 0) {
                    Move(GetRandDirection() > 0);
                }
                
                Move(TransformBallToPaddle(target->GetY()) < y);
            }
        }
    }
};

bool quit = false;

void HandleInput(Paddle* playerPaddle) {
    while (!quit) {
        if (_kbhit() != 0) {
            char input = _getch();
            if (input == 'w') {
                playerPaddle->Move(true);
            }
            else if (input == 's') {
                playerPaddle->Move(false);
            }
            else if (input == 'r') {
                quit = true;
            }
        }
    }
}

int PlayPong() {
    quit = false;
    Paddle playerPaddle = Paddle(false, 0, -1, 0, TransformBallToPaddle(GRIDHEIGHT) - 1);
    Paddle computerPaddle = Paddle(true, 10, GRIDWIDTH, 0, TransformBallToPaddle(GRIDHEIGHT) - 1);

    std::thread inputThread(&HandleInput, &playerPaddle);

    Ball ball = Ball(GRIDWIDTH / 2, GRIDHEIGHT / 2, 1, GetRandDirection());

    computerPaddle.SetTarget(&ball);

    int x = -1;
    int y = -1;

    int score = 0;

    system("CLS");

    std::cout << "FULLSCREEN IS RECOMMENDED" << std::endl;
    sleep_until(system_clock::now() + milliseconds(1000));
    for (int i = 3; i > 0; i--) {
        std::cout << i << std::endl;
        sleep_until(system_clock::now() + milliseconds(1000));
    }
    std::cout << "GO";
    sleep_until(system_clock::now() + milliseconds(1000));

    while (!quit) {
        system("CLS");
        y = -1;
        for (int i = 0; i < GRIDHEIGHT * 2 + 1; i++) {
            if (i <= playerPaddle.GetY() + 1 && i >= playerPaddle.GetY() - 1) {
                std::cout << "O ";
            }
            else {
                std::cout << "  ";
            }
            if (i % 2 == 0) {
                for (int j = 0; j < GRIDWIDTH * 3 + 1; j++) {
                    std::cout << "-";
                }
            }
            else {
                y++;
                x = -1;
                for (int j = 0; j < GRIDWIDTH * 3 + 1; j++) {
                    if (j % 3 == 0) {
                        std::cout << "|";
                        x++;
                    }
                    else {
                        if (ball.GetX() == x && ball.GetY() == y) {
                            std::cout << "B";
                        }
                        else {
                            std::cout << " ";
                        }
                    }
                }
            }
            if (i <= computerPaddle.GetY() + 1 && i >= computerPaddle.GetY() - 1) {
                std::cout << " O";
            }
            else {
                std::cout << "  ";
            }
            std::cout << "\n";
        }

        if (ball.GetX() == GRIDWIDTH - 1 && ball.GetXSpeed() > 0) {
            if (TransformBallToPaddle(ball.GetY()) >= computerPaddle.GetY() - 1 && TransformBallToPaddle(ball.GetY()) <= computerPaddle.GetY() + 1) {
                ball.Bounce(true, false);
            }
            else {
                score++;
                ball.Reset(GRIDWIDTH / 2, GRIDHEIGHT / 2, 1, GetRandDirection());
                playerPaddle.Reset(TransformBallToPaddle(GRIDHEIGHT) / 2);
                computerPaddle.Reset(TransformBallToPaddle(GRIDHEIGHT) / 2);
                sleep_until(system_clock::now() + milliseconds(1000));
                continue;
            }
        }
        else if (ball.GetX() == 0 && ball.GetXSpeed() < 0) {
            if (TransformBallToPaddle(ball.GetY()) >= playerPaddle.GetY() - 1 && TransformBallToPaddle(ball.GetY()) <= playerPaddle.GetY() + 1) {
                ball.Bounce(true, false);
            }
            else {
                quit = true;
                inputThread.join();
                return score;
            }
        }

        if (ball.GetY() == GRIDHEIGHT - 1 && ball.GetYSpeed() > 0) {
            ball.Bounce(false, true);
        }
        else if (ball.GetY() == 0 && ball.GetYSpeed() < 0) {
            ball.Bounce(false, true);
        }

        sleep_until(system_clock::now() + milliseconds(100));
        ball.Update();
        computerPaddle.Update();
    }

    quit = true;
    return score;
}


struct PlayerInfo {
    std::string name;
    int score;
};

void PrintHighScores(std::vector<PlayerInfo> highScores) {
    for (int i = 0; i < highScores.size(); i++) {
        std::cout << highScores.at(i).name << ": " << highScores.at(i).score << std::endl;
    }
    std::cout << std::endl;
}

void HighScores(std::vector<PlayerInfo>*  highScores);

void LoadHighScores(std::vector<PlayerInfo>* highScores)
{
    std::ifstream myFile("highScores.txt");
    std::string text, name, score;

    while (getline(myFile, text))
    {
        std::stringstream ss(text);
        getline(ss, name, ',');
        getline(ss, score, ',');

        PlayerInfo player;
        player.name = name;
        player.score = std::stoi(score);
        highScores->push_back(player);
    }
    myFile.close();
}

void SaveHighScores(std::vector<PlayerInfo>* highScores) {
    std::ofstream ofs;
    ofs.open("highScores.txt", std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < highScores->size(); i++) {
        ofs << highScores->at(i).name << "," << highScores->at(i).score << std::endl;
    }

    ofs.close();
}

void GetUserChoice(std::vector<PlayerInfo>* highScores)
{
    //Gets user input and displays text
    int userInput = 0;

    while (userInput != 3) {
        system("CLS");
        std::cout << "Welcome to Pong! What would you like to do?" << std::endl;
        std::cout << "Start Game (1)" << std::endl;
        std::cout << "View Highscores (2)" << std::endl;
        std::cout << "Exit (3)" << std::endl;
        std::cin >> userInput;

        if (userInput == 1)
        {
            int score = PlayPong();
            system("CLS");
            std::cout << "You scored " << score << std::endl;

            if (highScores->size() < 5) {
                std::cout << "Enter your name" << std::endl;
                std::string name;
                std::cin >> name;
                std::cout << "Your score has been recorded" << std::endl;
                PlayerInfo playerInfo;
                playerInfo.name = name;
                playerInfo.score = score;
                highScores->push_back(playerInfo);
                sleep_until(system_clock::now() + milliseconds(1000));
            }
            else {
                int smallestValue = 99999;
                int smallestIndex = -1;
                for (int i = 0; i < highScores->size(); i++) {
                    if (highScores->at(i).score < smallestValue) {
                        smallestValue = highScores->at(i).score;
                        smallestIndex = i;
                    }
                }
                if (smallestValue < score) {
                    std::cout << "Enter your name" << std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Your score has been recorded" << std::endl;
                    PlayerInfo playerInfo;
                    playerInfo.name = name;
                    playerInfo.score = score;
                    highScores->erase(highScores->begin() + smallestIndex);
                    highScores->push_back(playerInfo);
                    sleep_until(system_clock::now() + milliseconds(1000));
                }
                else {
                    std::cout << "You did not get a high score" << std::endl;
                    sleep_until(system_clock::now() + milliseconds(1000));
                }
            }
        }

        //Call HighScores Function
        else if (userInput == 2)
        {
            HighScores(highScores);
        }

        //Exits Program
        else
        {
            return;
        }
    }
}

void HighScores(std::vector<PlayerInfo>* highScores)
{
    system("CLS");
    PrintHighScores(*highScores);
    int userInput = 0;
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "Access (1)" << std::endl;
    std::cout << "Delete (2)" << std::endl;
    std::cout << "Reset (3)" << std::endl;
    std::cout << "Back (4)" << std::endl;
    std::cin >> userInput;

    if (userInput == 1)
    {
        std::cout << "\nWhat line would you like to see" << std::endl;
        std::cin >> userInput;
        if (userInput > 0 && userInput < highScores->size()) {
            std::cout << highScores->at(userInput).name << ": " << highScores->at(userInput).score << std::endl << std::endl;
        }
    }
    else if (userInput == 2)
    {
        std::cout << "\nWhat line would you like to remove" << std::endl;
        std::cin >> userInput;
        if (userInput > 0 && userInput < highScores->size()) {
            highScores->erase(highScores->begin() + userInput);
            std::cout << std::endl;
        }
    }
    else if (userInput == 3)
    {
        highScores->clear();
        std::cout << "\nThe High scores have been cleared" << std::endl;
        sleep_until(system_clock::now() + milliseconds(1000));
    }
    else
    {
        std::cout << "\nExiting game" << std::endl;
        sleep_until(system_clock::now() + milliseconds(1000));
        return;
    }
}


int main()
{
    std::vector <PlayerInfo> highScores;
    LoadHighScores(&highScores);
    GetUserChoice(&highScores);
    SaveHighScores(&highScores);

    return 0;
}