#pragma once
#include <QDialog>
#include <QString>
#include <QVector>
#include <QLineEdit>

class QLabel;
class QPushButton;

class VerbiInterface : public QDialog {
    Q_OBJECT
public:
    explicit VerbiInterface(QWidget *parent = nullptr);
    explicit VerbiInterface(const QString &verbToLoad, QWidget *parent = nullptr);

    // Indikativ
    QVector<QLineEdit*> indikativPrasens;           // 6
    QVector<QLineEdit*> indikativPrateritum;        // 6
    QVector<QLineEdit*> indikativPerfekt;           // 6
    QVector<QLineEdit*> indikativPlusquamperfekt;   // 6
    QVector<QLineEdit*> indikativFuturI;            // 6
    QVector<QLineEdit*> indikativFuturII;           // 6

    // Konjunktiv
    QVector<QLineEdit*> konjunktivI;                // 6
    QVector<QLineEdit*> konjunktivII;               // 6
    QVector<QLineEdit*> konjunktivPerfekt;          // 6
    QVector<QLineEdit*> konjunktivPlusquamperfekt;  // 6
    QVector<QLineEdit*> konjunktivFuturI;           // 6
    QVector<QLineEdit*> konjunktivFuturII;          // 6

    // Infinitiv
    QVector<QLineEdit*> infinitivI;                 // 2
    QVector<QLineEdit*> infinitivII;                // 2

    // Partizip
    QVector<QLineEdit*> partizipI;                  // 1
    QVector<QLineEdit*> partizipII;                 // 1

    // Imperativ Präsens
    QVector<QLineEdit*> imperativPrasens;           // 6

    QString verb;

    // Save correction result to history
    void saveCorrectionResult();

    // Load a verb and its correction from history
    void loadFromHistory(const QString &verb);

private:
    void setupUI();
    void addSection(const QString &title, const QStringList &pronouns, QVector<QLineEdit*> &fields);

    QPushButton *storicoBtn = nullptr;
private slots:
    void openStoricoDialog();
};

