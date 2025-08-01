#ifndef GIORNALE_H
#define GIORNALE_H

#include <QWidget>
#include <QLineEdit>
#include "mappa.h"
#include "listagiornali.h"

class Giornale : public QWidget {
    Q_OBJECT

public:
    Giornale(QWidget *parent = nullptr);

private slots:
    void handleCitySelection(const QString& city);

private:
    void showSettings();
    void onInvioClicked();
    QPushButton *settingsButton;
    QPushButton *invioButton;
    Mappa *mappa;
    QLineEdit *cityDisplay;
    ListaGiornali *newspaperList;
};

#endif // GIORNALE_H
