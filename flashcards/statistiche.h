#ifndef STATISTICHE_H
#define STATISTICHE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Statistiche : public QWidget
{
    Q_OBJECT

public:
    explicit Statistiche(QWidget *parent = nullptr);

private:
    QLabel *totalEncounteredLabel;
    QLabel *germanEncounteredLabel;
    QLabel *italianEncounteredLabel;
    QLabel *totalGuessedLabel;
    QLabel *germanGuessedLabel;
    QLabel *italianGuessedLabel;
    QLabel *totalPercentLabel;
    QLabel *germanPercentLabel;
    QLabel *italianPercentLabel;

    void loadStatistics();
};

#endif // STATISTICHE_H
