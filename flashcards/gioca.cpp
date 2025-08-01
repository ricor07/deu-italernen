#include "gioca.h"
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QPair>
#include <QTextStream>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <QRandomGenerator>
#include <QMessageBox>

Gioca::Gioca(bool deToIt, QWidget *parent) : QWidget(parent), deToIt(deToIt) {
    setWindowTitle("Gioca");
    resize(400, 250);

    wordLabel = new QLabel("Word");
    wordLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    wordLabel->setAlignment(Qt::AlignCenter);

    inputEdit = new QLineEdit;
    inputEdit->setPlaceholderText("Inserisci la traduzione...");

    btnClick = new QPushButton("Click");
    btnNext = new QPushButton("Next");

    connect(inputEdit, &QLineEdit::returnPressed, this, &Gioca::checkAnswer);
    connect(btnClick, &QPushButton::clicked, this, &Gioca::checkAnswer);
    connect(btnNext, &QPushButton::clicked, this, &Gioca::nextFlashcard);
    connect(btnNext, &QPushButton::clicked, this, [this]() {
        inputEdit->setFocus();
        inputEdit->clear();
    });

    // --- Gender buttons setup ---
    genderWidget = new QWidget(this);
    QHBoxLayout *genderLayout = new QHBoxLayout(genderWidget);
    genderLayout->setContentsMargins(0, 0, 0, 0);
    btnM = new QPushButton("M");
    btnN = new QPushButton("N");
    btnF = new QPushButton("F");
    QList<QPushButton*> genderBtns = {btnM, btnN, btnF};
    for (auto btn : genderBtns) {
        btn->setCheckable(true);
        genderLayout->addWidget(btn);
        btn->setVisible(false);
    }
    genderWidget->setVisible(false);

    // Only one gender button can be checked at a time
    connect(btnM, &QPushButton::clicked, this, [=]() {
        btnM->setChecked(true); btnN->setChecked(false); btnF->setChecked(false);
    });
    connect(btnN, &QPushButton::clicked, this, [=]() {
        btnM->setChecked(false); btnN->setChecked(true); btnF->setChecked(false);
    });
    connect(btnF, &QPushButton::clicked, this, [=]() {
        btnM->setChecked(false); btnN->setChecked(false); btnF->setChecked(true);
    });
    // --- End gender buttons setup ---

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnClick);
    buttonLayout->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addWidget(wordLabel);
    mainLayout->addWidget(inputEdit);
    mainLayout->addWidget(genderWidget); // <-- Add genderWidget to layout
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    loadFlashcards();
    loadScores();
    showRandomFlashcard();
}

void Gioca::loadFlashcards() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/flashcards.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Errore", "Impossibile aprire flashcards.csv");
        return;
    }

    QByteArray rawData = file.readAll();
    QString content = QString::fromLocal8Bit(rawData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    flashcards.clear();
    genders.clear(); // clear genders

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            QStringList parts = trimmed.split(',');
            if (parts.size() >= 2) {
                flashcards.append(qMakePair(parts[0].trimmed(), parts[1].trimmed()));
                // Parse gender if present
                if (parts.size() >= 3) {
                    QString g = parts[2].trimmed().toUpper();
                    if (g == "M" || g == "N" || g == "F")
                        genders.append(g.at(0));
                    else
                        genders.append(QChar());
                } else {
                    genders.append(QChar());
                }
            }
        }
    }

    file.close();
}

void Gioca::loadScores() {
    scores.clear();

    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scores.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        int totalWords = flashcards.size() * 2;
        for (int i = 0; i < totalWords; ++i)
            scores.append(qMakePair(0, 0));
        return;
    }

    QByteArray rawData = file.readAll();
    QString content = QString::fromLocal8Bit(rawData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    int index = 0;
    for (const QString &line : lines) {
        if (index >= flashcards.size() * 2) break;

        QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            QStringList parts = trimmed.split(',');
            if (parts.size() >= 3) {
                bool ok1 = false, ok2 = false;
                int correct = parts[1].trimmed().toInt(&ok1);
                int total = parts[2].trimmed().toInt(&ok2);

                if (ok1 && ok2) {
                    scores.append(qMakePair(correct, total));
                } else {
                    scores.append(qMakePair(0, 0));
                }
            } else {
                scores.append(qMakePair(0, 0));
            }
            index++;
        }
    }

    while (scores.size() < flashcards.size() * 2) {
        scores.append(qMakePair(0, 0));
    }

    file.close();
}

void Gioca::saveScores(int index, const QPair<int, int>& tupleToAdd) {
    QVector<std::tuple<QString, int, int>> csvData;

    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scores.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open CSV file for reading";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() != 3) continue;

        bool ok1 = false, ok2 = false;
        int val1 = parts[1].toInt(&ok1);
        int val2 = parts[2].toInt(&ok2);
        if (!ok1 || !ok2) continue;

        csvData.append(std::make_tuple(parts[0], val1, val2));
    }
    file.close();

    while (csvData.size() < flashcards.size() * 2) {
        csvData.append(std::make_tuple("word", 0, 0));
    }

    if (index < 0 || index >= csvData.size()) {
        qDebug() << "Index out of range";
        return;
    }

    auto& row = csvData[index];
    std::get<1>(row) += tupleToAdd.first;
    std::get<2>(row) += tupleToAdd.second;

    QFile outFile("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scores.csv");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Cannot open CSV file for writing";
        return;
    }

    QTextStream out(&outFile);
    for (const auto& tup : csvData) {
        out << std::get<0>(tup) << "," << std::get<1>(tup) << "," << std::get<2>(tup) << "\n";
    }

    outFile.close();
}

void Gioca::showRandomFlashcard() {
    if (flashcards.isEmpty()) {
        wordLabel->setText("Nessuna scheda");
        btnClick->setEnabled(false);
        btnNext->setEnabled(false);
        inputEdit->setEnabled(false);
        genderWidget->setVisible(false); // hide gender buttons
        return;
    }

    currentIndex = QRandomGenerator::global()->bounded(flashcards.size());

    if (deToIt) {
        wordLabel->setText(flashcards[currentIndex].first);  // show German
    } else {
        wordLabel->setText(flashcards[currentIndex].second); // show Italian
    }

    // --- Gender logic ---
    for (QPushButton *btn : {btnM, btnN, btnF}) {
        btn->setChecked(false);
        btn->setEnabled(true);
        btn->setStyleSheet("");
        btn->setVisible(false);
    }
    genderWidget->setVisible(false);
    currentGender = QChar();

    if (currentIndex >= 0 && currentIndex < genders.size()) {
        QChar g = genders[currentIndex];
        if (g == 'M' || g == 'N' || g == 'F') {
            currentGender = g;
            genderWidget->setVisible(true);
            btnM->setVisible(true);
            btnN->setVisible(true);
            btnF->setVisible(true);
        }
    }
    // --- End gender logic ---
}

void Gioca::checkAnswer() {
    if (currentIndex < 0 || currentIndex >= flashcards.size())
        return;

    QString userAnswer = inputEdit->text().trimmed();
    QString correctAnswer;
    int scoreIndex;

    if (deToIt) {
        correctAnswer = flashcards[currentIndex].second; // Italian
        scoreIndex = currentIndex;
    } else {
        correctAnswer = flashcards[currentIndex].first;  // German
        scoreIndex = flashcards.size() + currentIndex;
    }

    // --- Gender feedback logic ---
    bool genderActive = (currentGender == 'M' || currentGender == 'N' || currentGender == 'F');
    QPushButton* selectedBtn = nullptr;
    if (genderActive) {
        if (btnM->isChecked()) selectedBtn = btnM;
        else if (btnN->isChecked()) selectedBtn = btnN;
        else if (btnF->isChecked()) selectedBtn = btnF;
        // Show feedback only if a button was selected
        if (selectedBtn) {
            for (QPushButton *btn : {btnM, btnN, btnF}) btn->setEnabled(false);
            QChar selected = selectedBtn->text().at(0);
            if (selected == currentGender) {
                selectedBtn->setStyleSheet("background-color: green; color: white;");
            } else {
                selectedBtn->setStyleSheet("background-color: red; color: white;");
                for (QPushButton *btn : {btnM, btnN, btnF}) {
                    if (btn->text().at(0) == currentGender)
                        btn->setStyleSheet("background-color: green; color: white;");
                }
            }
        }
    }
    // --- End gender feedback logic ---

    if (userAnswer.compare(correctAnswer, Qt::CaseInsensitive) == 0) {
        scores[scoreIndex] = qMakePair(1, 1);
        QMessageBox::information(this, "Risposta corretta", "✅ Bravo!");
    } else {
        scores[scoreIndex] = qMakePair(1, 0);
        QMessageBox::warning(this, "Risposta sbagliata", "❌ La risposta corretta era:\n" + correctAnswer);
    }

    saveScores(scoreIndex, scores[scoreIndex]);
    scores[scoreIndex] = qMakePair(0, 0); // reset temp entry
    nextFlashcard();
}

void Gioca::nextFlashcard() {
    showRandomFlashcard();
    inputEdit->clear();
    inputEdit->setFocus();
    // Reset gender buttons for next card
    for (QPushButton *btn : {btnM, btnN, btnF}) {
        btn->setChecked(false);
        btn->setEnabled(true);
        btn->setStyleSheet("");
    }
}
