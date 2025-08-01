#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class QScrollBar;
class QSpinBox;
class QLabel;

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

    int numeroparole() const;
    int numerotentativi() const;

signals:
    void numeroparoleChanged(int);
    void numerotentativiChanged(int);

private slots:
    void onNumeroParoleChanged(int value);
    void onNumeroTentativiChanged(int value);

private:
    QScrollBar *scrollParole;
    QSpinBox *spinParole;
    QScrollBar *scrollTentativi;
    QSpinBox *spinTentativi;

    void loadSettings();
    void saveSettings();

    int m_numeroparole;
    int m_numerotentativi;
};

#endif // SETTINGS_H
