#include "verbiinterface.h"
#include "correction.h" // Add this include for correzioneverbi
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFont>
#include <QMessageBox>
#include <QScrollArea>
#include <QToolButton>
#include <QFrame>
#include <QFile>
#include <QEvent>
#include <QKeyEvent>
#include <QTextStream>
#include <QRandomGenerator>
#include <QDialog>
#include <QListWidget>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

// Helper for collapsible section
class CollapsibleSection : public QWidget {
public:
    CollapsibleSection(const QString &title, QWidget *content, QWidget *parent = nullptr)
        : QWidget(parent), contentWidget(content) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0,0,0,0);

        QToolButton *toggleButton = new QToolButton(this);
        toggleButton->setText(title);
        toggleButton->setCheckable(true);
        toggleButton->setChecked(true);
        toggleButton->setStyleSheet("QToolButton { font-weight: bold; font-size: 14px; }");
        toggleButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

        mainLayout->addWidget(toggleButton);
        mainLayout->addWidget(content);

        QObject::connect(toggleButton, &QToolButton::toggled, content, &QWidget::setVisible);
    }
private:
    QWidget *contentWidget;
};

VerbiInterface::VerbiInterface(QWidget *parent)
    : QDialog(parent)
{
    // Indikativ
    indikativPrasens.resize(6);
    indikativPrateritum.resize(6);
    indikativPerfekt.resize(6);
    indikativPlusquamperfekt.resize(6);
    indikativFuturI.resize(6);
    indikativFuturII.resize(6);

    // Konjunktiv
    konjunktivI.resize(6);
    konjunktivII.resize(6);
    konjunktivPerfekt.resize(6);
    konjunktivPlusquamperfekt.resize(6);
    konjunktivFuturI.resize(6);
    konjunktivFuturII.resize(6);

    // Infinitiv
    infinitivI.resize(2);
    infinitivII.resize(2);

    // Partizip
    partizipI.resize(1);
    partizipII.resize(1);

    // Imperativ Präsens
    imperativPrasens.resize(6);

    // Randomly choose a verb from csv
    QString csvPath = "/Users/riccardoorsi/Desktop/deutschlernen/csvfiles/flashcards.csv";
    QFile file(csvPath);
    QStringList eligibleVerbs;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(',');
            if (parts.size() >= 3 && parts[2].trimmed() == "V") {
                eligibleVerbs << parts[0].trimmed();
            }
        }
        file.close();
    }
    if (!eligibleVerbs.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(eligibleVerbs.size());
        verb = eligibleVerbs[idx];
    } else {
        verb = "machen"; // fallback
    }

    setupUI();
}

// Add this constructor for storico/history loading
VerbiInterface::VerbiInterface(const QString &verbToLoad, QWidget *parent)
    : QDialog(parent)
{
    // Indikativ
    indikativPrasens.resize(6);
    indikativPrateritum.resize(6);
    indikativPerfekt.resize(6);
    indikativPlusquamperfekt.resize(6);
    indikativFuturI.resize(6);
    indikativFuturII.resize(6);

    // Konjunktiv
    konjunktivI.resize(6);
    konjunktivII.resize(6);
    konjunktivPerfekt.resize(6);
    konjunktivPlusquamperfekt.resize(6);
    konjunktivFuturI.resize(6);
    konjunktivFuturII.resize(6);

    // Infinitiv
    infinitivI.resize(2);
    infinitivII.resize(2);

    // Partizip
    partizipI.resize(1);
    partizipII.resize(1);

    // Imperativ Präsens
    imperativPrasens.resize(6);

    verb = verbToLoad;
    setupUI();
    // Immediately load the correction from history
    loadFromHistory(verbToLoad);
}

void VerbiInterface::setupUI()
{
    setWindowTitle("Coniuga il verbo");
    resize(700, 900);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Scroll area for all content
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // Verb title
    QLabel *verbLabel = new QLabel(QString("Verbo da coniugare: <b>%1</b>").arg(verb), this);
    QFont font = verbLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    verbLabel->setFont(font);
    scrollLayout->addWidget(verbLabel);

    QStringList pronouns = {"ich", "du", "er/sie/es", "wir", "ihr", "sie/Sie"};

    // Indikativ tenses
    struct IndikativTense {
        QString title;
        QVector<QLineEdit*> *fields;
    };
    QVector<IndikativTense> indikativTenses = {
        {"Präsens", &indikativPrasens},
        {"Perfekt", &indikativPerfekt},
        {"Präteritum", &indikativPrateritum},
        {"Plusquamperfekt", &indikativPlusquamperfekt},
        {"Futur I", &indikativFuturI},
        {"Futur II", &indikativFuturII}
    };

    // Remove QGroupBox and use QWidget instead
    QWidget *indikativBox = new QWidget(this);
    QVBoxLayout *indikativLayout = new QVBoxLayout(indikativBox);

    // Collect all QLineEdit pointers for tab order
    QVector<QLineEdit*> allLineEdits;

    for (const auto &tense : indikativTenses) {
        // Allocate line edits
        QVector<QLineEdit*> &fields = *tense.fields;
        fields.clear();
        QWidget *content = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout(content);
        for (const QString &pronoun : pronouns) {
            QHBoxLayout *row = new QHBoxLayout;
            row->addWidget(new QLabel(pronoun));
            QLineEdit *edit = new QLineEdit;
            fields.append(edit);
            allLineEdits.append(edit); // collect for tab order
            row->addWidget(edit);
            vbox->addLayout(row);
        }
        CollapsibleSection *section = new CollapsibleSection(tense.title, content, indikativBox);
        indikativLayout->addWidget(section);
    }
    // Remove label by not using QGroupBox, just add the collapsible section
    CollapsibleSection *indikativSection = new CollapsibleSection("Indikativ", indikativBox, this);
    scrollLayout->addWidget(indikativSection);


        struct KonjunktivTense {
        QString title;
        QVector<QLineEdit*> *fields;
    };
    QVector<KonjunktivTense> konjunktivTenses = {
        {"Konjunktiv I", &konjunktivI},
        {"Konjunktiv II", &konjunktivII},
        {"Konjunktiv Perfekt", &konjunktivPerfekt},
        {"Konjunktiv Plusquamperfekt", &konjunktivPlusquamperfekt},
        {"Konjunktiv Futur I", &konjunktivFuturI},
        {"Konjunktiv Futur II", &konjunktivFuturII}
    };

    // Remove QGroupBox and use QWidget instead
    QWidget *konjunktivBox = new QWidget(this);
    QVBoxLayout *konjunktivLayout = new QVBoxLayout(konjunktivBox);

    for (const auto &tense : konjunktivTenses) {
        // Allocate line edits
        QVector<QLineEdit*> &fields = *tense.fields;
        fields.clear();
        QWidget *content = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout(content);
        for (const QString &pronoun : pronouns) {
            QHBoxLayout *row = new QHBoxLayout;
            row->addWidget(new QLabel(pronoun));
            QLineEdit *edit = new QLineEdit;
            fields.append(edit);
            allLineEdits.append(edit); // collect for tab order
            row->addWidget(edit);
            vbox->addLayout(row);
        }
        CollapsibleSection *section = new CollapsibleSection(tense.title, content, konjunktivBox);
        konjunktivLayout->addWidget(section);
    }
    // Remove label by not using QGroupBox, just add the collapsible section
    CollapsibleSection *konjunktivSection = new CollapsibleSection("Konjunktiv", konjunktivBox, this);
    scrollLayout->addWidget(konjunktivSection);

    // Other sections
    struct Section {
        QString title;
        QVector<QLineEdit*> *fields;
    };
    QVector<Section> otherSections = {
        {"Infinitiv I", &infinitivI},
        {"Infinitiv II", &infinitivII},
        {"Partizip I", &partizipI},
        {"Partizip II", &partizipII},
    };
    for (const Section &sec : otherSections) {
        QVector<QLineEdit*> &fields = *sec.fields;
        fields.clear();
        QWidget *content = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout(content);
        for (int i = 0; i < 2; i++) {
            if(i == 0 || i == 1 && (sec.title == "Infinitiv I" || sec.title == "Infinitiv II")) {
                QHBoxLayout *row = new QHBoxLayout;
                row->addWidget(new QLabel(" "));
                QLineEdit *edit = new QLineEdit;
                fields.append(edit);
                allLineEdits.append(edit); // collect for tab order
                row->addWidget(edit);
                vbox->addLayout(row);
            }
        }
        CollapsibleSection *section = new CollapsibleSection(sec.title, content, this);
        scrollLayout->addWidget(section);
    }

    Section imperativSection = {"Imperativ", &imperativPrasens};
    QVector<QLineEdit*> &imperativFields = *imperativSection.fields;
    imperativFields.clear();
    QWidget *imperativContent = new QWidget;
    QVBoxLayout *imperativLayout = new QVBoxLayout(imperativContent);
    for (const QString &pronoun : pronouns) {
        if(pronoun != "ich" && pronoun != "er/sie/es") {
            QHBoxLayout *row = new QHBoxLayout;
            row->addWidget(new QLabel(pronoun));
            QLineEdit *edit = new QLineEdit;
            imperativFields.append(edit);
            allLineEdits.append(edit); // collect for tab order
            row->addWidget(edit);
            imperativLayout->addLayout(row);
        }
    }

    CollapsibleSection *imperativSectionWidget = new CollapsibleSection("Imperativ", imperativContent, this);
    scrollLayout->addWidget(imperativSectionWidget);
    // Spacer to push invio button to bottom
    scrollLayout->addStretch(1);

    // Set scroll area
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // Invio button
    QPushButton *invioBtn = new QPushButton("Invio", this);
    invioBtn->setFixedSize(120, 40);
    invioBtn->setAutoDefault(false);
    invioBtn->setDefault(false);
    mainLayout->addWidget(invioBtn, 0, Qt::AlignCenter);

    // Storico button
    storicoBtn = new QPushButton("Storico", this);
    storicoBtn->setFixedSize(120, 40);
    mainLayout->addWidget(storicoBtn, 0, Qt::AlignCenter);

    connect(invioBtn, &QPushButton::clicked, this, [this, invioBtn]() {
        correzioneverbi(this);
        saveCorrectionResult();
    });

    connect(storicoBtn, &QPushButton::clicked, this, &VerbiInterface::openStoricoDialog);

    // Connect Enter key to move to next QLineEdit
    for (int i = 0; i < allLineEdits.size(); ++i) {
        QLineEdit *edit = allLineEdits[i];
        QObject::connect(edit, &QLineEdit::returnPressed, this, [allLineEdits, i]() {
            int next = i + 1;
            if (next < allLineEdits.size()) {
                allLineEdits[next]->setFocus();
                allLineEdits[next]->selectAll();
            }
        });
    }
}

// Save correction result to a JSON file per verb
void VerbiInterface::saveCorrectionResult()
{
    QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/history";
    QDir().mkpath(historyDir);
    QString filePath = historyDir + "/" + verb + ".json";

    QJsonObject obj;
    obj["verb"] = verb;

    auto saveSection = [](const QVector<QLineEdit*> &fields, QJsonArray &arr) {
        for (const QLineEdit *edit : fields) {
            QJsonObject entry;
            entry["text"] = edit->text();
            QColor bg = edit->palette().color(QPalette::Base);
            entry["correct"] = (bg.green() > bg.red()) ? true : false;
            arr.append(entry);
        }
    };

    QJsonArray arr;
    saveSection(indikativPrasens, arr);
    saveSection(indikativPrateritum, arr);
    saveSection(indikativPerfekt, arr);
    saveSection(indikativPlusquamperfekt, arr);
    saveSection(indikativFuturI, arr);
    saveSection(indikativFuturII, arr);

    saveSection(konjunktivI, arr);
    saveSection(konjunktivII, arr);
    saveSection(konjunktivPerfekt, arr);
    saveSection(konjunktivPlusquamperfekt, arr);
    saveSection(konjunktivFuturI, arr);
    saveSection(konjunktivFuturII, arr);

    saveSection(infinitivI, arr);
    saveSection(infinitivII, arr);

    saveSection(partizipI, arr);
    saveSection(partizipII, arr);

    saveSection(imperativPrasens, arr);

    obj["fields"] = arr;

    QFile f(filePath);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QJsonDocument(obj).toJson());
        f.close();
    }
}

// Load a verb and its correction from history
void VerbiInterface::loadFromHistory(const QString &verbToLoad)
{
    QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/history";
    QString filePath = historyDir + "/" + verbToLoad + ".json";
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) return;
    QByteArray data = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();
    if (!obj.contains("fields") || !obj["fields"].isArray()) return;
    QJsonArray arr = obj["fields"].toArray();

    int idx = 0;
    auto loadSection = [&](QVector<QLineEdit*> &fields) {
        for (QLineEdit *edit : fields) {
            if (idx >= arr.size()) return;
            QJsonObject entry = arr[idx++].toObject();
            edit->setText(entry["text"].toString());
            QPalette p = edit->palette();
            if (entry["correct"].toBool())
                p.setColor(QPalette::Base, QColor(180,255,180));
            else
                p.setColor(QPalette::Base, QColor(255,180,180));
            edit->setPalette(p);
            edit->setReadOnly(true);
        }
    };

    loadSection(indikativPrasens);
    loadSection(indikativPrateritum);
    loadSection(indikativPerfekt);
    loadSection(indikativPlusquamperfekt);
    loadSection(indikativFuturI);
    loadSection(indikativFuturII);

    loadSection(konjunktivI);
    loadSection(konjunktivII);
    loadSection(konjunktivPerfekt);
    loadSection(konjunktivPlusquamperfekt);
    loadSection(konjunktivFuturI);
    loadSection(konjunktivFuturII);

    loadSection(infinitivI);
    loadSection(infinitivII);

    loadSection(partizipI);
    loadSection(partizipII);

    loadSection(imperativPrasens);

    verb = verbToLoad;
    setWindowTitle("Storico: " + verb);
}

// Storico dialog
void VerbiInterface::openStoricoDialog()
{
    QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/history";
    QDir dir(historyDir);
    if (!dir.exists()) {
        QMessageBox::information(this, "Storico", "Nessun verbo nello storico.");
        return;
    }
    QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files);
    if (files.isEmpty()) {
        QMessageBox::information(this, "Storico", "Nessun verbo nello storico.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Storico verbi");
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QListWidget *list = new QListWidget(&dlg);
    for (const QString &file : files) {
        QString verbName = QFileInfo(file).baseName();
        list->addItem(verbName);
    }
    layout->addWidget(list);

    QPushButton *closeBtn = new QPushButton("Chiudi", &dlg);
    layout->addWidget(closeBtn);
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);

    connect(list, &QListWidget::itemDoubleClicked, &dlg, [&](QListWidgetItem *item){
        QString selectedVerb = item->text();
        dlg.accept();
        // Open a new VerbiInterface for the selected verb (use new constructor)
        VerbiInterface *v = new VerbiInterface(selectedVerb, nullptr);
        v->setAttribute(Qt::WA_DeleteOnClose);
        v->show();
    });

    dlg.exec();
}

void VerbiInterface::addSection(const QString &title, const QStringList &pronouns, QVector<QLineEdit*> &fields)
{
    Q_UNUSED(title)
    Q_UNUSED(pronouns)
    Q_UNUSED(fields)
}
