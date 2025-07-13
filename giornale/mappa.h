#ifndef MAPPA_H
#define MAPPA_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QPair>
#include <QString>
#include <QPoint>

class Mappa : public QWidget
{
    Q_OBJECT

public:
    explicit Mappa(QWidget *parent = nullptr);

signals:
    void citySelected(const QString &cityName);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *mapLabel;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QVBoxLayout *rightLayout;
    QHBoxLayout *mainLayout;
    QPixmap map;
    float zoomLevel; // 1.0 = 100%

    struct CityButton {
        QString name;
        QPushButton *button;
        QPoint originalPos;
    };
    QVector<CityButton> cityButtons;

    void updateMapDisplay();
    void zoomIn();
    void zoomOut();
};

#endif // MAPPA_H
