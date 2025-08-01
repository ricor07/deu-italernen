#include "listagiornali.h"

ListaGiornali::ListaGiornali(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void ListaGiornali::clearList()
{
    for (auto button : buttons) {
        layout->removeWidget(button);
        delete button;
    }
    buttons.clear();
}

void ListaGiornali::updateList(const QList<QPair<QString, QString>>& newspapers)
{
    clearList();
    currentPapers = newspapers;  // Store current papers
    
    for (const auto& paper : newspapers) {
        QPushButton* button = new QPushButton(paper.first, this);
        button->setCheckable(true);
        button->setStyleSheet(
            "QPushButton {"
            "    text-align: left;"
            "    padding: 5px;"
            "    border: none;"
            "}"
            "QPushButton:checked {"
            "    color: blue;"
            "    text-decoration: underline;"
            "}"
        );
        
        connect(button, &QPushButton::clicked, this, [url = paper.second]() {
            // Future expansion: handle URL clicks
        });
        
        layout->addWidget(button);
        buttons.append(button);
    }
}

QList<QPair<QString, QString>> ListaGiornali::getSelectedPapers() const
{
    QList<QPair<QString, QString>> selected;
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->isChecked()) {
            selected.append(currentPapers[i]);
        }
    }
    return selected;
}
