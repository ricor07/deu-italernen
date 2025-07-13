#include "impostazioni.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>

Impostazioni::Impostazioni(QWidget *parent) : QDialog(parent),
    m_numeroArticoli(5), m_numeroDomande(5)
{
    setWindowTitle("Impostazioni");

    // Create labels
    QLabel *labelArticoli = new QLabel(tr("Numero Articoli:"), this);
    QLabel *labelDomande = new QLabel(tr("Numero Domande:"), this);

    // Scrollbars
    scrollArticoli = new QScrollBar(Qt::Horizontal, this);
    scrollArticoli->setRange(2, 10);
    scrollArticoli->setPageStep(1);

    scrollDomande = new QScrollBar(Qt::Horizontal, this);
    scrollDomande->setRange(2, 10);
    scrollDomande->setPageStep(1);

    // Spinboxes
    spinArticoli = new QSpinBox(this);
    spinArticoli->setRange(2, 10);

    spinDomande = new QSpinBox(this);
    spinDomande->setRange(2, 10);

    // Layout for numero articoli controls
    QHBoxLayout *layoutArticoli = new QHBoxLayout;
    layoutArticoli->addWidget(labelArticoli);
    layoutArticoli->addWidget(scrollArticoli, 1);
    layoutArticoli->addWidget(spinArticoli);

    // Layout for numero domande controls
    QHBoxLayout *layoutDomande = new QHBoxLayout;
    layoutDomande->addWidget(labelDomande);
    layoutDomande->addWidget(scrollDomande, 1);
    layoutDomande->addWidget(spinDomande);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layoutArticoli);
    mainLayout->addLayout(layoutDomande);

    setLayout(mainLayout);

    // Load saved values
    loadSettings();

    // Set initial values
    scrollArticoli->setValue(m_numeroArticoli);
    spinArticoli->setValue(m_numeroArticoli);
    scrollDomande->setValue(m_numeroDomande);
    spinDomande->setValue(m_numeroDomande);

    // Connect scrollbar <-> spinbox sync
    connect(scrollArticoli, &QScrollBar::valueChanged,
            spinArticoli, &QSpinBox::setValue);
    connect(spinArticoli, qOverload<int>(&QSpinBox::valueChanged),
            scrollArticoli, &QScrollBar::setValue);

    connect(scrollDomande, &QScrollBar::valueChanged,
            spinDomande, &QSpinBox::setValue);
    connect(spinDomande, qOverload<int>(&QSpinBox::valueChanged),
            scrollDomande, &QScrollBar::setValue);

    // Track changes and save
    connect(scrollArticoli, &QScrollBar::valueChanged,
            this, &Impostazioni::onNumeroArticoliChanged);
    connect(scrollDomande, &QScrollBar::valueChanged,
            this, &Impostazioni::onNumeroDomandeChanged);

    setFixedSize(400, 150);
}

void Impostazioni::onNumeroArticoliChanged(int value)
{
    if (m_numeroArticoli != value) {
        m_numeroArticoli = value;
        saveSettings();
        emit numeroArticoliChanged(value);
    }
}

void Impostazioni::onNumeroDomandeChanged(int value)
{
    if (m_numeroDomande != value) {
        m_numeroDomande = value;
        saveSettings();
        emit numeroDomandeChanged(value);
    }
}

void Impostazioni::loadSettings()
{
    QSettings settings("DeutschLernen", "Giornale");
    m_numeroArticoli = settings.value("numeroArticoli", 5).toInt();
    m_numeroDomande = settings.value("numeroDomande", 5).toInt();
}

void Impostazioni::saveSettings()
{
    QSettings settings("DeutschLernen", "Giornale");
    settings.setValue("numeroArticoli", m_numeroArticoli);
    settings.setValue("numeroDomande", m_numeroDomande);
}

int Impostazioni::getNumeroArticoli()
{
    QSettings settings("DeutschLernen", "Giornale");
    return settings.value("numeroArticoli", 5).toInt();
}

int Impostazioni::getNumeroDomande()
{
    QSettings settings("DeutschLernen", "Giornale");
    return settings.value("numeroDomande", 5).toInt();
}
