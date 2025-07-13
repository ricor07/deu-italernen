#include "correction.h"
#include "verbiinterface.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QLineEdit>
#include <QColor>
#include <QPalette>

void correzioneverbi(QWidget *parent)
{
    VerbiInterface *vi = qobject_cast<VerbiInterface*>(parent);
    if (!vi) {
        QMessageBox::warning(parent, "Errore", "Errore interno: finestra non valida.");
        return;
    }

    // 1. Read CSV file
    QString csvPath = "/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/coniugverbi.csv";
    QFile file(csvPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(parent, "Errore", "Impossibile leggere il file coniugverbi.csv");
        return;
    }
    QTextStream in(&file);
    QString header = in.readLine(); // skip header

    // 2. Find the row for the verb
    QString verb = vi->verb.trimmed();
    QStringList row;
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() > 0 && fields[0].trimmed() == verb) {
            row = fields;
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        QMessageBox::warning(parent, "Errore", "Verbo non trovato nel CSV.");
        return;
    }

    // 3. Compare and color bars
    auto checkSection = [&](const QVector<QLineEdit*> &bars, int csvStartIdx) {
        for (int i = 0; i < bars.size(); ++i) {
            if (!bars[i]) continue;
            QString userText = bars[i]->text().trimmed();
            QString correctText = (csvStartIdx + i < row.size()) ? row[csvStartIdx + i].trimmed() : "";
            if (userText == correctText) {
                // Green
                QPalette p = bars[i]->palette();
                p.setColor(QPalette::Base, QColor(180, 255, 180));
                bars[i]->setPalette(p);
            } else {
                // Red and set correct text
                QPalette p = bars[i]->palette();
                p.setColor(QPalette::Base, QColor(255, 180, 180));
                bars[i]->setPalette(p);
                bars[i]->setText(correctText);
            }
        }
    };

    // 4. Map each section to CSV columns according to the new sequence

    int idx = 1; // skip verb

    // Indikativ
    checkSection(vi->indikativPrasens, idx); idx += 6;
    checkSection(vi->indikativPrateritum, idx); idx += 6;
    checkSection(vi->indikativPerfekt, idx); idx += 6;
    checkSection(vi->indikativPlusquamperfekt, idx); idx += 6;
    checkSection(vi->indikativFuturI, idx); idx += 6;
    checkSection(vi->indikativFuturII, idx); idx += 6;

    // Konjunktiv
    checkSection(vi->konjunktivI, idx); idx += 6;
    checkSection(vi->konjunktivII, idx); idx += 6;
    checkSection(vi->konjunktivPerfekt, idx); idx += 6;
    checkSection(vi->konjunktivPlusquamperfekt, idx); idx += 6;
    checkSection(vi->konjunktivFuturI, idx); idx += 6;
    checkSection(vi->konjunktivFuturII, idx); idx += 6;

    // Infinitiv
    checkSection(vi->infinitivI, idx); idx += 2;
    checkSection(vi->infinitivII, idx); idx += 2;

    // Partizip
    checkSection(vi->partizipI, idx); idx += 1;
    checkSection(vi->partizipII, idx); idx += 1;

    // Imperativ Präsens (6 bars)
    checkSection(vi->imperativPrasens, idx); // idx += 6;

    QMessageBox::information(parent, "Correzione", "Correzione completata. Le celle verdi sono corrette, quelle rosse sono state corrette.");

    // Move storico save here: save correction result immediately after correction
    vi->saveCorrectionResult();
}
