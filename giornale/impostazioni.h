#ifndef IMPOSTAZIONI_H
#define IMPOSTAZIONI_H

#include <QDialog>
#include <QSpinBox>
#include <QScrollBar>

class Impostazioni : public QDialog {
    Q_OBJECT

public:
    explicit Impostazioni(QWidget *parent = nullptr);
    static int getNumeroArticoli();
    static int getNumeroDomande();

signals:
    void numeroArticoliChanged(int);
    void numeroDomandeChanged(int);

private slots:
    void onNumeroArticoliChanged(int value);
    void onNumeroDomandeChanged(int value);

private:
    QScrollBar *scrollArticoli;
    QSpinBox *spinArticoli;
    QScrollBar *scrollDomande;
    QSpinBox *spinDomande;

    void loadSettings();
    void saveSettings();

    int m_numeroArticoli;
    int m_numeroDomande;
};

#endif // IMPOSTAZIONI_H
