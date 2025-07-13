#include "statistiche.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

Statistiche::Statistiche(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    totalEncounteredLabel = new QLabel(this);
    germanEncounteredLabel = new QLabel(this);
    italianEncounteredLabel = new QLabel(this);
    totalGuessedLabel = new QLabel(this);
    germanGuessedLabel = new QLabel(this);
    italianGuessedLabel = new QLabel(this);
    totalPercentLabel = new QLabel(this);
    germanPercentLabel = new QLabel(this);
    italianPercentLabel = new QLabel(this);

    layout->addWidget(totalEncounteredLabel);
    layout->addWidget(germanEncounteredLabel);
    layout->addWidget(italianEncounteredLabel);
    layout->addWidget(totalGuessedLabel);
    layout->addWidget(germanGuessedLabel);
    layout->addWidget(italianGuessedLabel);
    layout->addWidget(totalPercentLabel);
    layout->addWidget(germanPercentLabel);
    layout->addWidget(italianPercentLabel);

    loadStatistics();
}

void Statistiche::loadStatistics()
{
    QFile file("/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/scores.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open scores.csv");
        return;
    }

    QTextStream in(&file);
    QList<QStringList> rows;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;
        QStringList values = line.split(",");
        if (values.size() >= 3)
            rows.append(values);
    }

    int total = rows.size();
    int half = total / 2;

    int encounteredTotal = 0;
    int encounteredGerman = 0;
    int encounteredItalian = 0;

    int guessedTotal = 0;
    int guessedGerman = 0;
    int guessedItalian = 0;

    for (int i = 0; i < total; ++i) {
        bool ok2, ok3;
        int attempts = rows[i][1].toInt(&ok2);
        int correct = rows[i][2].toInt(&ok3);

        if (ok2 && attempts > 0) {
            encounteredTotal++;
            if (i < half)
                encounteredGerman++;
            else
                encounteredItalian++;
        }

        if (ok3 && correct > 0) {
            guessedTotal++;
            if (i < half)
                guessedGerman++;
            else
                guessedItalian++;
        }
    }

    auto percentage = [](int part, int whole) -> int {
        return (whole == 0) ? 0 : (100 * part / whole);
    };

    totalEncounteredLabel->setText("Words encountered: " + QString::number(encounteredTotal));
    germanEncounteredLabel->setText("German words encountered: " + QString::number(encounteredGerman));
    italianEncounteredLabel->setText("Italian words encountered: " + QString::number(guessedItalian));

    totalGuessedLabel->setText("Words guessed: " + QString::number(guessedTotal));
    germanGuessedLabel->setText("German words guessed: " + QString::number(guessedGerman));
    italianGuessedLabel->setText("Italian words guessed: " + QString::number(encounteredItalian));

    totalPercentLabel->setText("Total guessing accuracy: " + QString::number(percentage(guessedTotal, encounteredTotal)) + "%");
    germanPercentLabel->setText("German guessing accuracy: " + QString::number(percentage(guessedGerman, encounteredGerman)) + "%");
    italianPercentLabel->setText("Italian guessing accuracy: " + QString::number(percentage(encounteredItalian, guessedItalian)) + "%");
}
