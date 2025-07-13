#include "mappa.h"
#include "array.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QResizeEvent>

extern void initGiornali();

Mappa::Mappa(QWidget *parent)
    : QWidget(parent), map("/Users/riccardoorsi/Desktop/deutschlernen/map.png"), zoomLevel(1.0f)
{

    initGiornali(); // Initialize the array

    // QLabel that shows the map
    mapLabel = new QLabel(this);
    mapLabel->setAlignment(Qt::AlignCenter);
    mapLabel->setPixmap(map.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    mapLabel->setFixedSize(300, 300);

    // Create city buttons based on giornali array
    QMap<QPair<int, int>, int> positionCount; // Track positions to handle overlaps

    for (const auto &city : giornali) {
        QString cityName = city.first;
        float lat = city.second.second.first;
        float lon = city.second.second.second;

        // Convert coordinates to pixel positions (300x300 map)
        int px = static_cast<int>(300 * ((lon - 5.5f) / 11.7f));
        int py = static_cast<int>(300 * (1.0 - (lat - 45.7f) / 9.4f)); // Flip Y coordinate

        // Round position to grid to detect overlaps
        int gridX = px / 30;
        int gridY = py / 15;
        QPair<int, int> gridPos(gridX, gridY);

        // If position is already occupied, offset the button
        if (positionCount.contains(gridPos)) {
            positionCount[gridPos]++;
            px += 15 * (positionCount[gridPos] % 2 ? 1 : -1);
            py += 8 * ((positionCount[gridPos] / 2) + 1);
        } else {
            positionCount[gridPos] = 0;
        }

        QPushButton *cityButton = new QPushButton(cityName, mapLabel);
        cityButton->setStyleSheet(
            "QPushButton {"
            "   background-color: transparent;"
            "   color: rgba(0, 0, 0, 0.3);"  // Semi-transparent black
            "   border: none;"
            "   font: bold 7px;"  // Smaller font
            "   padding: 1px;"
            "}"
            "QPushButton:hover {"
            "   color: black;"               // Solid black on hover
            "   text-decoration: underline;"  // Added underline on hover
            "}"
        );
        cityButton->setFixedSize(60, 15);  // Smaller button size
        cityButton->move(px - 20, py - 7); // Adjusted centering for new size
        cityButton->show();
        cityButton->raise();

        // Store button and its original position for scaling
        cityButtons.append({cityName, cityButton, QPoint(px - 20, py - 7)});

        // Connect button click to show city info
        connect(cityButton, &QPushButton::clicked, this, [this, cityName]() {
            emit citySelected(cityName);
        });
    }

    // Zoom controls
    zoomInButton = new QPushButton("+", this);
    zoomOutButton = new QPushButton("-", this);
    zoomInButton->setFixedSize(24, 24);
    zoomOutButton->setFixedSize(24, 24);
    connect(zoomInButton, &QPushButton::clicked, this, &Mappa::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &Mappa::zoomOut);

    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(zoomOutButton);
    zoomLayout->addWidget(zoomInButton);
    zoomLayout->setSpacing(4);
    zoomLayout->setAlignment(Qt::AlignCenter);

    // Right-side layout: map and zoom controls
    rightLayout = new QVBoxLayout();
    rightLayout->addWidget(mapLabel, 0, Qt::AlignCenter);
    rightLayout->addLayout(zoomLayout);
    rightLayout->addStretch();

    // Main layout: left stretch + right layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void Mappa::zoomIn()
{
    if (zoomLevel < 2.75f) {
        zoomLevel += 0.25f;
        updateMapDisplay();
    }
}

void Mappa::zoomOut()
{
    if (zoomLevel > 0.5f) {
        zoomLevel -= 0.25f;
        updateMapDisplay();
    }
}

void Mappa::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateMapDisplay();
}

void Mappa::updateMapDisplay()
{
    QSize baseSize = QSize(300, 300);
    QSize zoomedSize = baseSize * zoomLevel;
    QPixmap scaledMap = map.scaled(zoomedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mapLabel->setPixmap(scaledMap);
    mapLabel->setFixedSize(scaledMap.size());

    float scaleX = static_cast<float>(scaledMap.width()) / 300.0f;
    float scaleY = static_cast<float>(scaledMap.height()) / 300.0f;

    // Scale all city buttons
    for (const auto &cityButton : cityButtons) {
        QPoint scaledPos(static_cast<int>(cityButton.originalPos.x() * scaleX),
                         static_cast<int>(cityButton.originalPos.y() * scaleY));
        cityButton.button->move(scaledPos);
    }
}

