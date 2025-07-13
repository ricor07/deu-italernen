#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QStringList>

class QPushButton;
class QLineEdit;
class QListWidget;

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);

private slots:
    void toggleLanguage();
    void filterList(const QString &text);

private:
    QPushButton *btnLangToggle;
    QLineEdit *inputSearch;
    QListWidget *listResults;

    bool isDE = true;  // true = DE->IT, false = IT->DE

    QList<QPair<QString, QString>> flashcardPairs;

    void loadFlashcards();
    void updateList(const QString &filter);
};

#endif // SEARCH_H
