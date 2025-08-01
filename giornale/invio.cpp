#include "invio.h"
#include "articoli.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

Invio::Invio(const QString& city, 
             const QList<QPair<QString, QString>>& selectedPapers,
             int numArticles,
             int numQuestions,
             QWidget *parent)
    : QDialog(parent),
      m_city(city),
      m_selectedPapers(selectedPapers),
      m_numArticles(numArticles),
      m_numQuestions(numQuestions)
{
    setWindowTitle("Conferma Selezione");
    setFixedSize(400, 300);

    // Create layout
    auto layout = new QVBoxLayout(this);

    // Show selected city
    auto cityLabel = new QLabel(QString("Città: %1").arg(m_city));
    layout->addWidget(cityLabel);

    // Show selected newspapers
    auto papersLabel = new QLabel("Giornali selezionati:");
    layout->addWidget(papersLabel);

    for (const auto& paper : m_selectedPapers) {
        auto paperLabel = new QLabel(QString("- %1").arg(paper.first));
        layout->addWidget(paperLabel);
    }

    // Show settings
    auto settingsLabel = new QLabel(QString("Numero articoli: %1\nNumero domande: %2")
                                  .arg(m_numArticles)
                                  .arg(m_numQuestions));
    layout->addWidget(settingsLabel);

    // Buttons
    auto buttonLayout = new QHBoxLayout;
    auto confirmButton = new QPushButton("Conferma", this);
    auto cancelButton = new QPushButton("Annulla", this);

    connect(confirmButton, &QPushButton::clicked, this, &Invio::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &Invio::onCancel);

    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    layout->addStretch();
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void Invio::onAccept()
{
    if (m_selectedPapers.isEmpty()) {
        QMessageBox::warning(this, "Attenzione", 
                           "Seleziona almeno un giornale prima di continuare.");
        return;
    }

    Articoli articoliDialog(m_selectedPapers, m_numArticles, this);
    accept();
    articoliDialog.exec();
}

void Invio::onCancel()
{
    reject();
}
