#include "search.h"
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <algorithm>    // for std::sort
#include <string>       // for std::as_const

Search::Search(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Search");
    resize(400, 300);

    btnLangToggle = new QPushButton("DE", this);
    inputSearch = new QLineEdit(this);
    listResults = new QListWidget(this);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(btnLangToggle);
    topLayout->addWidget(inputSearch);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(listResults);

    setLayout(mainLayout);

    connect(btnLangToggle, &QPushButton::clicked, this, &Search::toggleLanguage);
    connect(inputSearch, &QLineEdit::textChanged, this, &Search::filterList);

    loadFlashcards();
    updateList("");
}

void Search::toggleLanguage()
{
    isDE = !isDE;
    btnLangToggle->setText(isDE ? "DE" : "IT");
    updateList(inputSearch->text());
}

void Search::loadFlashcards()
{
    flashcardPairs.clear();

    const QString filepath = "/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/flashcards.csv";
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open flashcards.csv");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(',');
        if (parts.size() < 2) continue;

        // parts[0] = German, parts[1] = Italian, parts[2] = char ignored
        flashcardPairs.append(qMakePair(parts[0].trimmed(), parts[1].trimmed()));
    }

    file.close();
}

void Search::filterList(const QString &filter)
{
    updateList(filter);
}

void Search::updateList(const QString &filter)
{
    listResults->clear();

    QString filterLower = filter.toLower();

    // Create a temp copy to sort before filtering
    QList<QPair<QString, QString>> sortedPairs = flashcardPairs;

    if (isDE) {
        // Sort by German (first)
        std::sort(sortedPairs.begin(), sortedPairs.end(),
                  [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                      return a.first.toLower() < b.first.toLower();
                  });
    } else {
        // Sort by Italian (second)
        std::sort(sortedPairs.begin(), sortedPairs.end(),
                  [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                      return a.second.toLower() < b.second.toLower();
                  });
    }

    // Use std::as_const instead of deprecated qAsConst
    for (const auto &pair : std::as_const(sortedPairs)) {
        QString key = isDE ? pair.first : pair.second;

        if (key.toLower().startsWith(filterLower)) {
            QString displayText;
            if (isDE)
                displayText = pair.first + " - " + pair.second;
            else
                displayText = pair.second + " - " + pair.first;

            listResults->addItem(displayText);
        }
    }
}
