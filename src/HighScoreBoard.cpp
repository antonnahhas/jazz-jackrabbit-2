//----------------------------------include section-------------------------------------
#include "HighScoreBoard.h"
#include "Resources.h"
#include "macros.h"

//----------------------------------const section-------------------------------------

const std::string NONE = "No name", EMPTY = "", SPACE = " ";
//----------------------------------class implementation------------------------------

// Constructs the scoreboard from the default file
HighScoreBoard::HighScoreBoard()  : m_file(Resources::instance().getHighScoresFile())
{
    loadScoresFromFile();
}

//----------------------------------functions section---------------------------------

// Load scores from the given file
void HighScoreBoard::loadScoresFromFile()
{
    int score, lineCounter = INIT; // counter for making sure we reading at most 10 scores
    std::string line, name;// variables for storing line info  
          
    // read lines until EOF or maximum number of lines read
    while (lineCounter < MAX_NUM_SCORES && std::getline(m_file, line))
    {
        std::istringstream iss(line);
        if (!(iss >> name >> score)) {
            break;
        }
        m_highScores.push_back(Score{name, score});
        lineCounter++;
    }
    sortScores(); // sortScores when all relevant scores have been read
}

// Saves the current scores to the given file
void HighScoreBoard::saveScoresToFile()
{
    try
    {
        std::ofstream outFile(SCORES_FILE, std::ofstream::trunc);
        //m_file.open("high_score_table.txt");
        for (const Score &score: m_highScores)
            outFile << score.playerName << SPACE << score.playerScore << std::endl;
    }
    catch (...)
    {}
}

// Adds a new (name, score) entry to the score board if the given
// score is one of the top 10 scores.                          
void HighScoreBoard::addScore(std::string name, int score)
{
    if (name == EMPTY)
        name = NONE;
    // number of scores reached max capacity, check if we need to replace the smallest score
    if (m_highScores.size() == MAX_NUM_SCORES)
    {
        int smallestScore = m_highScores[m_highScores.size() - 1].playerScore;
        // given score  is larger than the smallest one, replace and sortScores
        if (score >= smallestScore)
        {
            m_highScores[m_highScores.size() - 1] = Score{name, score};
            sortScores();
        }
        // else dont add a smaller score
        return;
    }
    // else we have less than MAX_NUM_SCORES, add and sortScores
    m_highScores.push_back(Score{name, score});
    sortScores();
    saveScoresToFile();
}

// Sort the score table by descending order using the built-in comparator
void HighScoreBoard::sortScores()
{
    std::sort(m_highScores.begin(), m_highScores.end());
}

std::vector<Score> HighScoreBoard::getScores() const { return m_highScores; }