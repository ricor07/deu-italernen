#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "flashcards/flashcards.h"
#include "giornale/giornale.h"
#include "verbi/verbiinterface.h" // add this include


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("deutschlernen");
    window.resize(300, 400);

    QPushButton *btnFlashcards = new QPushButton("Flashcards");
    QPushButton *btnGiornali   = new QPushButton("Giornali");
    QPushButton *btnVerbi      = new QPushButton("Verbi");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(btnFlashcards);
    layout->addWidget(btnGiornali);
    layout->addWidget(btnVerbi);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    window.setLayout(layout);
    window.show();

    // Open flashcards interface on click
    QObject::connect(btnFlashcards, &QPushButton::clicked, [&]() {
        Flashcards *fc = new Flashcards();
        fc->show();
    });

    QObject::connect(btnGiornali, &QPushButton::clicked, [&]() {
        Giornale *gr = new Giornale();
        gr->show();
    });

    // Open verbi interface on click
    QObject::connect(btnVerbi, &QPushButton::clicked, [&]() {
        VerbiInterface *vi = new VerbiInterface();
        vi->show();
    });

    return app.exec();
}
