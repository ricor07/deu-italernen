#ifndef FLASHCARDS_H
#define FLASHCARDS_H

#include <QWidget>

class Flashcards : public QWidget {
    Q_OBJECT  // <-- THIS IS CRITICAL!

public:
    explicit Flashcards(QWidget *parent = nullptr);
};

#endif // FLASHCARDS_H
