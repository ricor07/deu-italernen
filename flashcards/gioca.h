#ifndef GIOCA_H
#define GIOCA_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include <QPair>

class Gioca : public QWidget {
    Q_OBJECT

public:
    explicit Gioca(bool deToIt = true, QWidget *parent = nullptr);

    void checkAnswer();
    void nextFlashcard();
    void loadFlashcards();
    void loadScores();
    void saveScores(int index, const QPair<int, int>& tupleToAdd);
    void showRandomFlashcard();

private:
    QLabel *wordLabel;
    QLineEdit *inputEdit;
    QPushButton *btnClick;
    QPushButton *btnNext;
    QPushButton *btnLangToggle;  // ✅ Added for DE<->IT toggle

    // --- Add for gender selection ---
    QWidget *genderWidget = nullptr;
    QPushButton *btnM = nullptr;
    QPushButton *btnN = nullptr;
    QPushButton *btnF = nullptr;
    QChar currentGender; // 'M', 'N', 'F' or '\0'
    // --- End gender selection ---

    QVector<QPair<QString, QString>> flashcards;
    QVector<QChar> genders; // Add this to store genders for each flashcard
    QVector<QPair<int, int>> scores;
    int currentIndex = -1;
    bool deToIt;  // true for DE to IT, false for IT to DE
};

#endif // GIOCA_H
