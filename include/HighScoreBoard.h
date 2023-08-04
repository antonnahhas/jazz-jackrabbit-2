//----------------------------------include section-----------------------------------
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

//----------------------------------const section-----------------------------------

const int MAX_NUM_SCORES = 10; // maximum number of high-scores to save
const std::string SCORES_FILE = "high_score_table.txt";

//----------------------------------struct definition---------------------------------

//A struct representing a player's score.
struct Score
{
    std::string playerName;
    int playerScore;
    // compare by descending order according to the larger score
    bool operator<(const Score& other) { return playerScore > other.playerScore; }
};

//----------------------------------class definition----------------------------------
class HighScoreBoard
{
public:
    HighScoreBoard(); 
    ~HighScoreBoard() = default; 
    void addScore(std::string name, int score);
    void saveScoresToFile();
    void loadScoresFromFile(); 
    std::vector<Score> getScores() const;

private:
    void sortScores();

    std::vector<Score> m_highScores;  // a vector of all the scores saved until this moment
    std::fstream& m_file;             // the file that holds the record of all high scores
};