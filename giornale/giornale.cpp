#include "giornale.h"
#include "impostazioni.h"
#include "articoli.h"
#include "invio.h"  // Add this line
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "array.h"

Giornale::Giornale(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Giornale");

    // Create and set up the city display
    cityDisplay = new QLineEdit(this);
    cityDisplay->setReadOnly(true);
    cityDisplay->setFixedWidth(200);
    cityDisplay->setPlaceholderText("Città selezionata");
    
    // Create newspaper list
    newspaperList = new ListaGiornali(this);
    newspaperList->setFixedWidth(250);
    
    mappa = new Mappa(this);
    
    // Connect signals
    connect(mappa, &Mappa::citySelected, this, &Giornale::handleCitySelection);

    // Add settings button
    settingsButton = new QPushButton(this);
    settingsButton->setText("⚙"); // Fallback to text if icon not found
    settingsButton->setIconSize(QSize(24, 24));
    settingsButton->setFixedSize(32, 32);
    settingsButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.1);"
        "    border-radius: 16px;"
        "}"
    );
    connect(settingsButton, &QPushButton::clicked, this, &Giornale::showSettings);

    // Add invio button
    invioButton = new QPushButton("Invio", this);
    invioButton->setFixedSize(100, 30);
    invioButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );
    connect(invioButton, &QPushButton::clicked, this, &Giornale::onInvioClicked);

    // Layout setup
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    topLayout->addWidget(cityDisplay);
    topLayout->addStretch();
    topLayout->addWidget(settingsButton);
    
    leftLayout->addLayout(topLayout);
    leftLayout->addWidget(newspaperList);
    leftLayout->addStretch();
    leftLayout->addWidget(invioButton, 0, Qt::AlignBottom);
    
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(mappa);
    
    setLayout(mainLayout);
    resize(1000, 600);
}

void Giornale::handleCitySelection(const QString& city)
{
    cityDisplay->setText(city);
    
    // Find newspapers for selected city
    for (const auto& cityInfo : giornali) {
        if (cityInfo.first == city) {
            newspaperList->updateList(cityInfo.second.first);
            break;
        }
    }
}

void Giornale::showSettings()
{
    Impostazioni settings(this);
    settings.exec();
}

void Giornale::onInvioClicked()
{
    QList<QPair<QString, QString>> selectedPapers = newspaperList->getSelectedPapers();

    Invio invioDialog(cityDisplay->text(), 
                     selectedPapers,
                     Impostazioni::getNumeroArticoli(),
                     Impostazioni::getNumeroDomande(),
                     this);
    
    if (invioDialog.exec() == QDialog::Accepted) {
        // TODO: Handle acceptance
    }
}

