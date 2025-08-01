#include "flashcards.h"
#include "gioca.h"
#include "account.h"
#include "settings.h"
#include "search.h"
#include "statistiche.h"
#include "scheda.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QDialog>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>

Flashcards::Flashcards(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Flashcards");
    resize(400, 300);

    QPushButton *btnAccount = new QPushButton("Account");
    QPushButton *btnSearch = new QPushButton();
    btnSearch->setIcon(QIcon::fromTheme("edit-find"));
    btnSearch->setFixedSize(30, 30);

    QPushButton *btnSettings = new QPushButton(QString::fromUtf8("⚙️"));
    btnSettings->setFixedSize(30, 30);

    // Storico button
    QPushButton *btnStorico = new QPushButton("Storico");
    btnStorico->setFixedSize(80, 30);

    QHBoxLayout *topBar = new QHBoxLayout;
    topBar->addWidget(btnAccount);
    topBar->addWidget(btnStorico); // Add storico button to the top bar
    topBar->addStretch();
    topBar->addWidget(btnSearch);
    topBar->addSpacing(5);
    topBar->addWidget(btnSettings);

    QPushButton *btnGioca = new QPushButton("Gioca");
    QPushButton *btnStatistiche = new QPushButton("Statistiche");
    QPushButton *btnScheda = new QPushButton("Scheda");

    QPushButton *btnLangToggle = new QPushButton("DE->IT");
    bool *deToIt = new bool(true);  // flag for language direction

    QFont font("Arial", 14);
    btnGioca->setFont(font);
    btnStatistiche->setFont(font);
    btnScheda->setFont(font);

    QHBoxLayout *centerRow = new QHBoxLayout;
    centerRow->addWidget(btnGioca);
    centerRow->addSpacing(20);
    centerRow->addWidget(btnScheda);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topBar);
    mainLayout->addSpacing(50);
    mainLayout->addLayout(centerRow);

    QHBoxLayout *bottomBar = new QHBoxLayout;
    bottomBar->addWidget(btnLangToggle, 0, Qt::AlignLeft);
    bottomBar->addStretch();
    bottomBar->addWidget(btnStatistiche, 0, Qt::AlignRight);

    mainLayout->addLayout(bottomBar);
    setLayout(mainLayout);

    // Single Settings instance
    Settings *settings = new Settings();

    connect(btnLangToggle, &QPushButton::clicked, this, [btnLangToggle, deToIt]() {
        *deToIt = !(*deToIt);
        btnLangToggle->setText(*deToIt ? "DE->IT" : "IT->DE");
    });

    connect(btnGioca, &QPushButton::clicked, this, [deToIt]() {
        Gioca *g = new Gioca(*deToIt);  // pass language direction
        g->show();
    });

    connect(btnScheda, &QPushButton::clicked, this, [deToIt, settings]() {
        // Get parameters from settings (you may need to add getters to Settings class)
        int numeroParole = settings->numeroparole();  // Default: 10
        int numeroTentativi = settings->numerotentativi();  // Default: 3

        Scheda *scheda = new Scheda(*deToIt, numeroParole, numeroTentativi);
        scheda->show();
    });

    connect(btnSettings, &QPushButton::clicked, this, [settings]() {
        settings->show();
    });

    connect(btnAccount, &QPushButton::clicked, this, []() {
        Account *account = new Account();
        account->show();
    });

    connect(btnSearch, &QPushButton::clicked, this, []() {
        Search *search = new Search();
        search->show();
    });

    connect(btnStatistiche, &QPushButton::clicked, this, []() {
        Statistiche *statistiche = new Statistiche();
        statistiche->show();
    });

    connect(btnStorico, &QPushButton::clicked, this, [this]() {
        // Show storico dialog
        QString historyDir = QDir::homePath() + "/Desktop/deutschlernen/scheda_history";
        QDir dir(historyDir);
        if (!dir.exists()) {
            QMessageBox::information(this, "Storico", "Nessuna scheda completata.");
            return;
        }
        QStringList files = dir.entryList(QStringList() << "scheda_*.csv", QDir::Files, QDir::Time);
        if (files.isEmpty()) {
            QMessageBox::information(this, "Storico", "Nessuna scheda completata.");
            return;
        }

        QDialog dlg(this);
        dlg.setWindowTitle("Storico schede completate");
        QVBoxLayout *layout = new QVBoxLayout(&dlg);
        QListWidget *list = new QListWidget(&dlg);
        for (const QString &file : files) {
            list->addItem(file);
        }
        layout->addWidget(list);

        QPushButton *closeBtn = new QPushButton("Chiudi", &dlg);
        layout->addWidget(closeBtn);
        QObject::connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);

        QObject::connect(list, &QListWidget::itemDoubleClicked, &dlg, [&](QListWidgetItem *item){
            QString selectedFile = item->text();
            QString filePath = historyDir + "/" + selectedFile;
            QFile f(filePath);
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(&dlg, "Errore", "Impossibile aprire la scheda.");
                return;
            }
            QDialog schedaDlg(&dlg);
            schedaDlg.setWindowTitle("Scheda: " + selectedFile);
            QVBoxLayout *schedaLayout = new QVBoxLayout(&schedaDlg);
            QTableWidget *table = new QTableWidget(&schedaDlg);
            table->setColumnCount(2);
            table->setHorizontalHeaderLabels(QStringList() << "DE" << "IT");
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            QStringList lines;
            QTextStream in(&f);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty())
                    lines << line;
            }
            table->setRowCount(lines.size());
            for (int i = 0; i < lines.size(); ++i) {
                QStringList parts = lines[i].split(',');
                table->setItem(i, 0, new QTableWidgetItem(parts.value(0)));
                table->setItem(i, 1, new QTableWidgetItem(parts.value(1)));
            }
            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            schedaLayout->addWidget(table);

            QPushButton *closeSchedaBtn = new QPushButton("Chiudi", &schedaDlg);
            schedaLayout->addWidget(closeSchedaBtn);
            QObject::connect(closeSchedaBtn, &QPushButton::clicked, &schedaDlg, &QDialog::accept);

            schedaDlg.exec();
        });

        dlg.exec();
    });
}