#ifndef SCHEDA_H
#define SCHEDA_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include <QPair>
#include <tuple>

class Scheda : public QWidget
{
    Q_OBJECT

public:
    explicit Scheda(bool deToIt = true, int numeroparole = 10, int numerotentativi = 5, QWidget *parent = nullptr);
    void setNumeroTentativi(int n); // <-- add setter for numeroTentativi

private slots:
    void checkAnswer();
    void nextFlashcard();

private:
    // UI components
    QLabel *wordLabel;
    QLineEdit *inputEdit;
    QPushButton *btnClick;
    QPushButton *btnNext;
    QPushButton *btnLangToggle;
    QLabel *attemptsLabel = nullptr; // Add this for n / total display

    // --- Gender selection ---
    QWidget *genderWidget = nullptr;
    QPushButton *btnM = nullptr;
    QPushButton *btnN = nullptr;
    QPushButton *btnF = nullptr;
    QChar currentGender; // 'M', 'N', 'F' or '\0'
    QVector<QChar> genders; // store genders for each flashcard
    // --- End gender selection ---

    // Data storage
    QVector<QPair<QString, QString>> flashcards;  // All flashcards from file
    QVector<QPair<int, int>> scores;              // Scores for all flashcards
    QVector<int> selectedIndices;
    QSet<int> indicesSet;
    // Scheda-specific data
    int numeroParole;                             // Number of words in scheda
    int numeroTentativi;                          // Number of correct attempts needed
    QVector<QPair<QString, QString>> schedaCards; // Words in current scheda

    // For bidirectional quiz
    struct QuizItem {
        int schedaIndex; // index in schedaCards
        bool deToIt;     // true: DE->IT, false: IT->DE
    };
    QVector<QuizItem> quizQueue; // The queue of (word, direction) to ask
    QVector<QPair<int, int>> attemptCounter; // [i].first: DE->IT, [i].second: IT->DE

    QuizItem currentQuizItem; // Current item being asked

    // Current state
    int currentIndex;
    bool deToIt;  // Language direction: true for DE->IT, false for IT->DE

    // Methods for flashcard management
    void loadFlashcards();
    void loadScores();
    void saveScores(int index, const QPair<int, int>& tupleToAdd);
    void saveSelectedIndices();
    void showRandomFlashcard();
    
    // Methods for scheda management
    void ensureArraysSynchronized();
    void loadScheda();
    void generateNewScheda();
    void saveScheda();
    void loadAttemptCounter();
    void loadSelectedIndices();
    void saveAttemptCounter();
    void showRandomScheda();
    void prepareQuizQueue();
};

#endif // SCHEDA_H