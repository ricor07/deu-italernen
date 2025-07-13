#include "settings.h"
#include <QScrollBar>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>

Settings::Settings(QWidget *parent) : QWidget(parent),
    m_numeroparole(10), m_numerotentativi(5)
{
    // Create labels
    QLabel *labelParole = new QLabel(tr("Numero Parole:"), this);
    QLabel *labelTentativi = new QLabel(tr("Numero Tentativi:"), this);

    // Scrollbars
    scrollParole = new QScrollBar(Qt::Horizontal, this);
    scrollParole->setRange(10, 200);
    scrollParole->setPageStep(1);

    scrollTentativi = new QScrollBar(Qt::Horizontal, this);
    scrollTentativi->setRange(1, 30);
    scrollTentativi->setPageStep(1);

    // Spinboxes for number input
    spinParole = new QSpinBox(this);
    spinParole->setRange(10, 200);

    spinTentativi = new QSpinBox(this);
    spinTentativi->setRange(1, 30);

    // Layout for numeroparole controls
    QHBoxLayout *layoutParole = new QHBoxLayout;
    layoutParole->addWidget(labelParole);
    layoutParole->addWidget(scrollParole, 1);
    layoutParole->addWidget(spinParole);

    // Layout for numerotentativi controls
    QHBoxLayout *layoutTentativi = new QHBoxLayout;
    layoutTentativi->addWidget(labelTentativi);
    layoutTentativi->addWidget(scrollTentativi, 1);
    layoutTentativi->addWidget(spinTentativi);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layoutParole);
    mainLayout->addLayout(layoutTentativi);

    setLayout(mainLayout);

    // Load saved values
    loadSettings();

    // Set initial values on widgets
    scrollParole->setValue(m_numeroparole);
    spinParole->setValue(m_numeroparole);
    scrollTentativi->setValue(m_numerotentativi);
    spinTentativi->setValue(m_numerotentativi);

    // Connect scrollbar <-> spinbox sync
    connect(scrollParole, &QScrollBar::valueChanged,
            spinParole, &QSpinBox::setValue);
    connect(spinParole, qOverload<int>(&QSpinBox::valueChanged),
            scrollParole, &QScrollBar::setValue);

    connect(scrollTentativi, &QScrollBar::valueChanged,
            spinTentativi, &QSpinBox::setValue);
    connect(spinTentativi, qOverload<int>(&QSpinBox::valueChanged),
            scrollTentativi, &QScrollBar::setValue);

    // Track changes and save to variables + settings
    connect(scrollParole, &QScrollBar::valueChanged,
            this, &Settings::onNumeroParoleChanged);
    connect(scrollTentativi, &QScrollBar::valueChanged,
            this, &Settings::onNumeroTentativiChanged);
}

int Settings::numeroparole() const
{
    return m_numeroparole;
}

int Settings::numerotentativi() const
{
    return m_numerotentativi;
}

void Settings::onNumeroParoleChanged(int value)
{
    if (m_numeroparole != value) {
        m_numeroparole = value;
        saveSettings();
        emit numeroparoleChanged(value);
    }
}

void Settings::onNumeroTentativiChanged(int value)
{
    if (m_numerotentativi != value) {
        m_numerotentativi = value;
        saveSettings();
        emit numerotentativiChanged(value);
    }
}

void Settings::loadSettings()
{
    QSettings settings("YourOrganization", "YourApp");
    m_numeroparole = settings.value("numeroparole", 10).toInt();
    m_numerotentativi = settings.value("numerotentativi", 5).toInt();
}

void Settings::saveSettings()
{
    QSettings settings("YourOrganization", "YourApp");
    settings.setValue("numeroparole", m_numeroparole);
    settings.setValue("numerotentativi", m_numerotentativi);
}
