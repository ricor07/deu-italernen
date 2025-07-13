#include "search.h"
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>  // ✅ FIXED: Required for QCoreApplication::applicationDirPath()
#include <algorithm>         // for std::sort
#include <string>            // for std::as_const

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

    const QString filepath = QDir(QCoreApplication::applicationDirPath()).filePath("csvfiles/flashcards.csv");

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

        // parts[0] = German, parts[1] = Italian
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

    QList<QPair<QString, QString>> sortedPairs = flashcardPairs;

    if (isDE) {
        std::sort(sortedPairs.begin(), sortedPairs.end(),
                  [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                      return a.first.toLower() < b.first.toLower();
                  });
    } else {
        std::sort(sortedPairs.begin(), sortedPairs.end(),
                  [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                      return a.second.toLower() < b.second.toLower();
                  });
    }

    for (const auto &pair : std::as_const(sortedPairs)) {
        QString key = isDE ? pair.first : pair.second;

        if (key.toLower().startsWith(filterLower)) {
            QString displayText = isDE
                ? pair.first + " - " + pair.second
                : pair.second + " - " + pair.first;

            listResults->addItem(displayText);
        }
    }
}
