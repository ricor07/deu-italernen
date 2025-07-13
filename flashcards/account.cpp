#include "account.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>

Account::Account(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Account");
    resize(300, 150);

    btnReset = new QPushButton("Reset", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(btnReset);
    setLayout(layout);

    connect(btnReset, &QPushButton::clicked, this, &Account::onResetClicked);
}

void Account::onResetClicked()
{
    resetScores();
}

void Account::resetScores()
{
    const QString filepath = "/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scores.csv";

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open scores.csv for reading.");
        return;
    }

    QStringList lines;
    QTextStream in(&file);

    // Read all lines, reset int fields
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');

        // Expect format: word, int, int
        if (parts.size() == 3) {
            // Keep word, set ints to 0
            parts[1] = "0";
            parts[2] = "0";
            line = parts.join(',');
        }
        lines.append(line);
    }
    file.close();

    // Re-open for writing and overwrite file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error", "Cannot open scores.csv for writing.");
        return;
    }

    QTextStream out(&file);
    for (const QString &line : lines) {
        out << line << "\n";
    }
    file.close();

    // Delete all storico (history) files for verbi
    QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/history";
    QDir dir(historyDir);
    if (dir.exists()) {
        QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files);
        for (const QString &filename : files) {
            dir.remove(filename);
        }
    }

    // Delete all storico (history) files for schede
    QString schedaHistoryDir = QDir::homePath() + "/Desktop/deutschlernen/scheda_history";
    QDir schedaDir(schedaHistoryDir);
    if (schedaDir.exists()) {
        QStringList schedaFiles = schedaDir.entryList(QStringList() << "scheda_*.csv", QDir::Files);
        for (const QString &filename : schedaFiles) {
            schedaDir.remove(filename);
        }
    }

    QMessageBox::information(this, "Reset", "Scores have been reset to zero e storico svuotato.");
}
