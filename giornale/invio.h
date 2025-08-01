#ifndef INVIO_H
#define INVIO_H

#include <QDialog>
#include <QList>
#include <QPair>
#include <QString>

class Invio : public QDialog {
    Q_OBJECT

public:
    explicit Invio(const QString& city, 
                  const QList<QPair<QString, QString>>& selectedPapers,
                  int numArticles,
                  int numQuestions,
                  QWidget *parent = nullptr);

private slots:
    void onAccept();
    void onCancel();

private:
    QString m_city;
    QList<QPair<QString, QString>> m_selectedPapers;
    int m_numArticles;
    int m_numQuestions;
};

#endif // INVIO_H
