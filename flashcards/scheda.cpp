#include "scheda.h"
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
#include <QSet>
#include <QDir>
#include <QDateTime>

Scheda::Scheda(bool deToIt, int numeroparole, int numerotentativi, QWidget *parent) 
    : QWidget(parent), deToIt(deToIt), numeroParole(numeroparole), numeroTentativi(numerotentativi), currentIndex(-1)
{
    setWindowTitle("scheda");
    resize(400, 250);

    wordLabel = new QLabel("Word");
    wordLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    wordLabel->setAlignment(Qt::AlignCenter);

    inputEdit = new QLineEdit;
    inputEdit->setPlaceholderText("Inserisci la traduzione...");

    btnClick = new QPushButton("Click");
    btnNext = new QPushButton("Next");

    connect(inputEdit, &QLineEdit::returnPressed, this, &Scheda::checkAnswer);
    connect(btnClick, &QPushButton::clicked, this, &Scheda::checkAnswer);
    connect(btnNext, &QPushButton::clicked, this, &Scheda::nextFlashcard);
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
    mainLayout->addWidget(genderWidget);
    mainLayout->addLayout(buttonLayout);

    // Add attempts label
    attemptsLabel = new QLabel(this);
    attemptsLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    QHBoxLayout *attemptsLayout = new QHBoxLayout;
    attemptsLayout->addStretch();
    attemptsLayout->addWidget(attemptsLabel);

    mainLayout->addLayout(attemptsLayout);

    setLayout(mainLayout);

    loadFlashcards();
    loadScores();
    loadScheda();
    prepareQuizQueue();
    showRandomScheda();
}

// Prepare the quiz queue: for each word, add both directions
void Scheda::prepareQuizQueue() {
    quizQueue.clear();
    // Do NOT reset attemptCounter here, just ensure size matches
    if (attemptCounter.size() != schedaCards.size()) {
        attemptCounter.resize(schedaCards.size());
        for (int i = 0; i < attemptCounter.size(); ++i) {
            if (attemptCounter[i].first < 0 || attemptCounter[i].second < 0)
                attemptCounter[i] = qMakePair(0, 0);
        }
    }
    for (int i = 0; i < schedaCards.size(); ++i) {
        if (attemptCounter[i].first < numeroTentativi)
            quizQueue.append({i, true});  // DE->IT
        if (attemptCounter[i].second < numeroTentativi)
            quizQueue.append({i, false}); // IT->DE
    }
}

void Scheda::loadFlashcards() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/flashcards.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Errore", "Impossibile aprire flashcards.csv");
        return;
    }

    QByteArray rawData = file.readAll();
    QString content = QString::fromLocal8Bit(rawData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    flashcards.clear();
    genders.clear();

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

void Scheda::loadScores() {
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

void Scheda::loadScheda() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scheda.csv");
    
    // Check if file exists and is not empty
    bool fileEmpty = true;
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray rawData = file.readAll();
        QString content = QString::fromLocal8Bit(rawData);
        QStringList lines = content.split('\n', Qt::SkipEmptyParts);
        
        if (!lines.isEmpty()) {
            fileEmpty = false;
            // Load existing scheda words
            schedaCards.clear();
            for (const QString &line : lines) {
                QString trimmed = line.trimmed();
                if (!trimmed.isEmpty()) {
                    QStringList parts = trimmed.split(',');
                    if (parts.size() >= 2) {
                        schedaCards.append(qMakePair(parts[0].trimmed(), parts[1].trimmed()));
                    }
                }
            }
        }
        file.close();
    }
    
    // If file is empty, generate new scheda
    if (fileEmpty) {
        generateNewScheda();
    } else {
        // Load attempt counter and selected indices for existing scheda
        loadAttemptCounter();
        loadSelectedIndices();
        
        // Ensure all arrays have the same size
        ensureArraysSynchronized();
    }
}

void Scheda::ensureArraysSynchronized() {
    int schedaSize = schedaCards.size();

    // Resize attemptCounter to match schedaCards
    if (attemptCounter.size() != schedaSize) {
        int oldSize = attemptCounter.size();
        attemptCounter.resize(schedaSize);
        for (int i = oldSize; i < schedaSize; ++i) {
            attemptCounter[i] = qMakePair(0, 0);
        }
    }
    
    // selectedIndices should already be properly loaded or generated
    // Only resize if it's empty and we have scheda cards (this shouldn't normally happen)
    if (selectedIndices.isEmpty() && !schedaCards.isEmpty()) {
        qDebug() << "Warning: selectedIndices is empty but schedaCards is not. This indicates a data inconsistency.";
        // In this case, we can't recover the original indices, so we'll have to regenerate
        generateNewScheda();
        return;
    }
    
    // Ensure selectedIndices has the same size as schedaCards
    if (selectedIndices.size() != schedaSize) {
        qDebug() << "Warning: selectedIndices size mismatch. Expected:" << schedaSize << "Got:" << selectedIndices.size();
        // Resize to match, but this might cause issues with score tracking
        selectedIndices.resize(schedaSize);
    }
}

void Scheda::generateNewScheda() {
    if (flashcards.isEmpty() || numeroParole <= 0) {
        return;
    }

    schedaCards.clear();
    selectedIndices.clear();
    indicesSet.clear();

    // Shuffle indices for better randomness
    QVector<int> allIndices;
    for (int i = 0; i < flashcards.size(); ++i) allIndices.append(i);
    std::shuffle(allIndices.begin(), allIndices.end(), *QRandomGenerator::global());
    int maxWords = qMin(numeroParole, flashcards.size());
    for (int i = 0; i < maxWords; ++i) {
        int idx = allIndices[i];
        schedaCards.append(flashcards[idx]);
        selectedIndices.append(idx);
    }

    // Save the generated scheda to storico immediately
    if (!schedaCards.isEmpty()) {
        QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/scheda_history";
        QDir().mkpath(historyDir);
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString filePath = historyDir + QString("/scheda_%1.csv").arg(timestamp);
        QFile f(filePath);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            for (const auto& card : schedaCards) {
                out << card.first << "," << card.second << "\n";
            }
            f.close();
        }
    }

    // Initialize attempt counter array
    attemptCounter.clear();
    attemptCounter.resize(schedaCards.size());
    for (int i = 0; i < attemptCounter.size(); ++i) {
        attemptCounter[i] = qMakePair(0, 0);
    }

    // Save new scheda to file
    saveScheda();
    saveAttemptCounter();
    saveSelectedIndices();
}

void Scheda::saveScheda() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scheda.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Cannot open scheda.csv for writing";
        return;
    }
    
    QTextStream out(&file);
    for (const auto& card : schedaCards) {
        out << card.first << "," << card.second << "\n";
    }
    
    file.close();
}

void Scheda::loadAttemptCounter() {
    attemptCounter.clear();

    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/attempt_counter.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        attemptCounter.resize(schedaCards.size());
        for (int i = 0; i < attemptCounter.size(); i++) {
            attemptCounter[i] = qMakePair(0, 0);
        }
        return;
    }

    QByteArray rawData = file.readAll();
    QString content = QString::fromLocal8Bit(rawData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            QStringList parts = trimmed.split(',');
            if (parts.size() == 2) {
                bool ok1 = false, ok2 = false;
                int count1 = parts[0].toInt(&ok1);
                int count2 = parts[1].toInt(&ok2);
                if (ok1 && ok2) {
                    attemptCounter.append(qMakePair(count1, count2));
                } else {
                    attemptCounter.append(qMakePair(0, 0));
                }
            } else if (parts.size() == 1) {
                bool ok = false;
                int count = parts[0].toInt(&ok);
                if (ok) {
                    attemptCounter.append(qMakePair(count, 0));
                } else {
                    attemptCounter.append(qMakePair(0, 0));
                }
            }
        }
    }

    while (attemptCounter.size() < schedaCards.size()) {
        attemptCounter.append(qMakePair(0, 0));
    }

    file.close();
}

void Scheda::loadSelectedIndices() {
    selectedIndices.clear(); // Clear before loading
    
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/selected_indices.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        // File doesn't exist - this should only happen when generating a new scheda
        // Don't initialize with zeros here, let generateNewScheda() handle it
        return;
    }
    
    QByteArray rawData = file.readAll();
    QString content = QString::fromLocal8Bit(rawData);
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            bool ok = false;
            int index = trimmed.toInt(&ok);
            if (ok) {
                selectedIndices.append(index);
            }
        }
    }

    file.close();
}

void Scheda::saveAttemptCounter() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/attempt_counter.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Cannot open attempt_counter.csv for writing";
        return;
    }
    
    QTextStream out(&file);
    for (const QPair<int, int> &count : attemptCounter) {
        out << count.first << "," << count.second << "\n";
    }
    
    file.close();
}

void Scheda::saveSelectedIndices() {
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/selected_indices.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Cannot open selected_indices.csv for writing";
        return;
    }
    
    QTextStream out(&file);
    for (int index : selectedIndices) {
        out << index << "\n";
    }

    file.close();
}

void Scheda::saveScores(int index, const QPair<int, int>& tupleToAdd) {
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

// Show a random quiz item from the queue
void Scheda::showRandomScheda() {
    if (quizQueue.isEmpty()) {
        QMessageBox::information(this, "Scheda completata", "Parole finite");
        generateNewScheda();
        prepareQuizQueue();
        if (quizQueue.isEmpty()) {
            wordLabel->setText("Nessuna scheda");
            btnClick->setEnabled(false);
            btnNext->setEnabled(false);
            inputEdit->setEnabled(false);
            genderWidget->setVisible(false);
            attemptsLabel->setText("");
            return;
        }
    }

    int qidx = QRandomGenerator::global()->bounded(quizQueue.size());
    currentQuizItem = quizQueue[qidx];
    currentIndex = currentQuizItem.schedaIndex;

    // Show the correct prompt
    if (currentQuizItem.deToIt) {
        wordLabel->setText(schedaCards[currentIndex].first);  // show German
    } else {
        wordLabel->setText(schedaCards[currentIndex].second); // show Italian
    }

    // --- Gender logic ---
    for (QPushButton *btn : {btnM, btnN, btnF}) {
        btn->setChecked(false);
        btn->setEnabled(true);
        btn->setStyleSheet("");
        btn->setVisible(false);
    }
    currentGender = QChar();

    int origIdx = (currentIndex >= 0 && currentIndex < selectedIndices.size()) ? selectedIndices[currentIndex] : -1;
    if (currentQuizItem.deToIt && origIdx >= 0 && origIdx < genders.size()) {
        QChar g = genders[origIdx];
        if (g == 'M' || g == 'N' || g == 'F') {
            currentGender = g;
            genderWidget->setVisible(true);
            btnM->setVisible(true);
            btnN->setVisible(true);
            btnF->setVisible(true);
        } else {
            genderWidget->setVisible(false);
        }
    } else {
        genderWidget->setVisible(false); // Hide for IT->DE
    }
    // --- End gender logic ---

    // Show attempts info
    int n = currentQuizItem.deToIt ? attemptCounter[currentIndex].first : attemptCounter[currentIndex].second;
    attemptsLabel->setText(QString("%1 / %2").arg(n).arg(numeroTentativi));
}

void Scheda::showRandomFlashcard() {
    showRandomScheda(); // Now uses scheda instead of flashcards
}

void Scheda::checkAnswer() {
    if (currentIndex < 0 || currentIndex >= schedaCards.size())
        return;

    QString userAnswer = inputEdit->text().trimmed();
    QString correctAnswer;

    if (currentQuizItem.deToIt) {
        correctAnswer = schedaCards[currentIndex].second; // Italian
    } else {
        correctAnswer = schedaCards[currentIndex].first;  // German
    }

    // --- Gender feedback logic ---
    bool genderActive = (currentQuizItem.deToIt && (currentGender == 'M' || currentGender == 'N' || currentGender == 'F'));
    QPushButton* selectedBtn = nullptr;
    if (genderActive) {
        if (btnM->isChecked()) selectedBtn = btnM;
        else if (btnN->isChecked()) selectedBtn = btnN;
        else if (btnF->isChecked()) selectedBtn = btnF;
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

    bool isCorrect = userAnswer.compare(correctAnswer, Qt::CaseInsensitive) == 0;

    if (isCorrect) {
        QMessageBox::information(this, "Risposta corretta", "✅ Bravo!");
        // Increment attempt counter for the direction
        if (currentQuizItem.deToIt) {
            attemptCounter[currentIndex].first++;
        } else {
            attemptCounter[currentIndex].second++;
        }
        saveAttemptCounter(); // <-- persist after every correct answer
        // Remove from quizQueue if reached numeroTentativi for this direction
        for (int i = 0; i < quizQueue.size(); ++i) {
            if (quizQueue[i].schedaIndex == currentIndex && quizQueue[i].deToIt == currentQuizItem.deToIt) {
                if ((currentQuizItem.deToIt && attemptCounter[currentIndex].first >= numeroTentativi) ||
                    (!currentQuizItem.deToIt && attemptCounter[currentIndex].second >= numeroTentativi)) {
                    quizQueue.removeAt(i);
                }
                break;
            }
        }
    } else {
        QMessageBox::warning(this, "Risposta sbagliata", "❌ La risposta corretta era:\n" + correctAnswer);
        int scoreIdx = selectedIndices[currentIndex] + int(!currentQuizItem.deToIt) * flashcards.size();
        saveScores(scoreIdx, qMakePair(1, 0));
    }

    nextFlashcard();
}

void Scheda::nextFlashcard() {
    showRandomScheda();
    inputEdit->clear();
    inputEdit->setFocus();
    for (QPushButton *btn : {btnM, btnN, btnF}) {
        btn->setChecked(false);
        btn->setEnabled(true);
        btn->setStyleSheet("");
    }
}

// Add this setter method to allow changing numeroTentativi at runtime
void Scheda::setNumeroTentativi(int n) {
    numeroTentativi = n;
    prepareQuizQueue();
    // Update attempts label if a card is shown
    if (currentIndex >= 0 && currentIndex < schedaCards.size()) {
        int nAttempts = currentQuizItem.deToIt ? attemptCounter[currentIndex].first : attemptCounter[currentIndex].second;
        attemptsLabel->setText(QString("%1 / %2").arg(nAttempts).arg(numeroTentativi));
    }
}