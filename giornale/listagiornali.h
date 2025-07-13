#ifndef LISTAGIORNALI_H
#define LISTAGIORNALI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include <QPair>
#include <QString>

class ListaGiornali : public QWidget {
    Q_OBJECT

public:
    explicit ListaGiornali(QWidget *parent = nullptr);
    void updateList(const QList<QPair<QString, QString>>& newspapers);
    QList<QPair<QString, QString>> getSelectedPapers() const;  // Add this method

private:
    QVBoxLayout *layout;
    QList<QPushButton*> buttons;
    QList<QPair<QString, QString>> currentPapers;  // Add this member
    void clearList();
};

#endif // LISTAGIORNALI_H
