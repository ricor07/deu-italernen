#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QWidget>

class QPushButton;

class Account : public QWidget
{
    Q_OBJECT

public:
    explicit Account(QWidget *parent = nullptr);

private slots:
    void onResetClicked();

private:
    QPushButton *btnReset;

    void resetScores();
};

#endif // ACCOUNT_H
