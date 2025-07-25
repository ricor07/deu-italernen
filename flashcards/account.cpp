#include "account.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>  // FIXED: Needed for applicationDirPath()

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
    QString scoresPath = QDir(QCoreApplication::applicationDirPath()).filePath("csvfiles/scores.csv");

    QFile file(scoresPath);  // FIXED: was 'filepath', now 'scoresPath'
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
    QString historyDir = QDir(QCoreApplication::applicationDirPath()).filePath("history");
    QDir dir(historyDir);
    if (dir.exists()) {
        QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files);
        for (const QString &filename : files) {
            dir.remove(filename);
        }
    } else {
        dir.mkpath(".");
    }

    // Delete all storico (history) files for schede
    QString schedaHistoryDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/scheda_history";

    QDir schedaDir(schedaHistoryDir);
    if (schedaDir.exists()) {
        QStringList schedaFiles = schedaDir.entryList(QStringList() << "scheda_*.csv", QDir::Files);
        for (const QString &filename : schedaFiles) {
            schedaDir.remove(filename);
        }
    } else {
        schedaDir.mkpath(".");  // FIXED: previously dir.mkpath(), now correct one
    }

    QMessageBox::information(this, "Reset", "Scores have been reset to zero e storico svuotato.");
}
