#include "articoli.h"
#include "impostazioni.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QTextEdit>
#include <QDebug>
#include <QProcess>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QToolButton>
#include <QMouseEvent>
#include <QPointer>
#include <QLineEdit> // <-- Add this include

// viene creato il bottone per la traduzione
TranslateHelper::TranslateHelper(QTextEdit* edit, QWidget* parentDialog)
    : QObject(edit), textEdit(edit), dialog(parentDialog) {
    button = new QToolButton(edit);
    button->setText("Traduci");
    button->setVisible(false);
    button->setStyleSheet(
        "QToolButton { background: #1976D2; color: white; border-radius: 5px; padding: 4px 10px; }"
        "QToolButton:hover { background: #2196F3; }"
    );
    connect(button, &QToolButton::clicked, this, &TranslateHelper::onTranslateClicked);
    edit->viewport()->installEventFilter(this);
    edit->installEventFilter(this);
}

// eventFilter per gestire il mouse e la tastiera
// Mostra il bottone di traduzione quando c'è una selezione di testo
// e lo nasconde quando non c'è selezione o il focus esce dal QTextEdit
// Gestisce anche il mouse move per mostrare il bottone vicino alla selezione
// e il click per avviare la traduzione
// Il bottone viene mostrato vicino alla selezione di testo
// e nascosto quando non c'è selezione o il focus esce dal QTextEdit

bool TranslateHelper::eventFilter(QObject* obj, QEvent* event) {
    if (obj == textEdit->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            QTextCursor cursor = textEdit->cursorForPosition(me->pos());
            if (textEdit->textCursor().hasSelection()) {
                int selStart = textEdit->textCursor().selectionStart();
                int selEnd = textEdit->textCursor().selectionEnd();
                int pos = cursor.position();
                if (pos >= selStart && pos <= selEnd) {
                    showButtonNearSelection();
                }
                // Do not hide the button on mouse move outside selection
            }
        }
    }
    if (obj == textEdit) {
        if (event->type() == QEvent::FocusOut) {
            button->setVisible(false);
        }
        if (event->type() == QEvent::KeyRelease || event->type() == QEvent::MouseButtonRelease) {
            // Hide button if selection is cleared, show if selection exists
            if (textEdit->textCursor().hasSelection()) {
                showButtonNearSelection();
            } else {
                button->setVisible(false);
            }
        }
    }
    return QObject::eventFilter(obj, event);
}


// onTranslateClicked viene chiamato quando si clicca il bottone di traduzione
// Crea un dialogo per mostrare la traduzione del testo selezionato
// Se non c'è testo selezionato, non fa nulla
// Il testo selezionato viene tradotto in italiano usando il modello "gemma3n:e2b"
// Il comando viene eseguito tramite QProcess e il risultato viene mostrato in un
// QPlainTextEdit all'interno del dialogo di traduzione


void TranslateHelper::onTranslateClicked() {
    QString selectedText = textEdit->textCursor().selectedText();
    if (selectedText.trimmed().isEmpty())
        return;

    QString prompt = QString("Traduci in italiano questa frase senza commenti ulteriori: \"%1\"").arg(selectedText);

    // Escape characters for AppleScript
    QString escapedPrompt = prompt;
    escapedPrompt.replace("\\", "\\\\")
                 .replace("\"", "\\\"")
                 .replace("$", "\\$");

    QString outputFilePath = "/Users/riccardoorsi/Desktop/deutschlernen/ollamaoutput.txt";

    // Fix: add missing terminating '"' character at the end of the string
    QString shellCommand = QString("OLLAMA_NO_TTY=1 NO_COLOR=1 ollama run gemma3n:e2b \\\"%1\\\" > \\\"%2\\\"").arg(escapedPrompt, outputFilePath);
    QString osaScript = QString("do shell script \"%1\"").arg(shellCommand);

    // Run osascript silently (doesn't show Terminal)
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus status) {
        if (exitCode != 0 || status != QProcess::NormalExit) {
            QMessageBox::warning(dialog, "Errore", "Errore nell'esecuzione del comando via osascript.");
            process->deleteLater();
            return;
        }

        // Wait a bit for the file to be written
        QTimer::singleShot(1000, [=]() {
            QFile file(outputFilePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(dialog, "Errore", "Impossibile leggere il file di output.");
                return;
            }

            QString result = QTextStream(&file).readAll();
            file.close();

            // Show result in popup
            QDialog* resultDialog = new QDialog(dialog);
            resultDialog->setWindowTitle("Traduzione");
            QVBoxLayout* layout = new QVBoxLayout(resultDialog);
            QTextEdit* view = new QTextEdit(result);
            view->setReadOnly(true);
            layout->addWidget(view);
            resultDialog->resize(500, 300);
            resultDialog->exec();
        });

        process->deleteLater();
    });

    process->start("osascript", QStringList() << "-e" << osaScript);

    button->setVisible(false);
}
void TranslateHelper::showButtonNearSelection() {
    QTextCursor cursor = textEdit->textCursor();
    QRect rect = textEdit->cursorRect(cursor);
    QPoint globalPos = textEdit->viewport()->mapToGlobal(rect.bottomRight());
    QPoint localPos = textEdit->viewport()->mapFromGlobal(globalPos);
    button->move(localPos + QPoint(0, 5));
    button->setVisible(true);
    button->raise();
}

Articoli::Articoli(const QList<QPair<QString, QString>> &papers,
                   int numArticles,
                   QWidget *parent)
    : QDialog(parent),
      newspapers(papers),
      articlesPerPaper(numArticles)
{
    setWindowTitle("Articoli");
    resize(800, 600);

    articleList = new QListWidget(this);
    articleList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    articleList->setWordWrap(true);

    connect(articleList, &QListWidget::itemDoubleClicked,
            this, &Articoli::onItemDoubleClicked);

    statusLabel = new QLabel("Ricerca articoli in corso...", this);
    timeLabel = new QLabel("Tempo trascorso: 0:00", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(statusLabel);
    layout->addWidget(timeLabel);
    layout->addWidget(articleList);

    elapsedTimer = new QTimer(this);
    connect(elapsedTimer, &QTimer::timeout, this, &Articoli::updateElapsedTime);
    elapsedTimer->start(1000);
    timer.start();

    pythonProcess = new QProcess(this);
    pythonProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Articoli::handleProcessFinished);
    connect(pythonProcess, &QProcess::errorOccurred,
            this, &Articoli::onProcessError);

    runPythonScript();
}

void Articoli::runPythonScript()
{
    // Prepare JSON input
    QJsonObject input;
    QJsonArray papersArray;
    for (const auto &paper : newspapers) {
        QJsonObject paperObj;
        paperObj["name"] = paper.first;
        paperObj["url"] = paper.second;
        papersArray.append(paperObj);
    }
    input["papers"] = papersArray;
    input["num_articles"] = articlesPerPaper;

    QByteArray jsonData = QJsonDocument(input).toJson(QJsonDocument::Compact);

    // Assuming the Python script is in the current working directory
    QString scriptPath = QDir::current().absoluteFilePath("returnarticles.py");

    // Start Python process with python3 (adjust if needed)
    pythonProcess->start("python3", QStringList() << scriptPath);

    if (!pythonProcess->waitForStarted(3000)) {
        statusLabel->setText("Error: Failed to start Python script");
        return;
    }

    // Write JSON input to the Python script's stdin
    pythonProcess->write(jsonData);
    pythonProcess->write("\n"); // ensure newline to flush
    pythonProcess->closeWriteChannel();

    statusLabel->setText("Processing...");
}

void Articoli::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    elapsedTimer->stop();

    if (exitCode != 0 || exitStatus != QProcess::NormalExit) {
        QString err = pythonProcess->readAllStandardError();
        statusLabel->setText("Errore nell'esecuzione dello script");
        QMessageBox::critical(this, "Error", "Script error: " + err);
        return;
    }

    QByteArray output = pythonProcess->readAllStandardOutput();
    if (output.isEmpty()) {
        statusLabel->setText("Script did not return any output");
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(output, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = QString("Error parsing JSON: %1").arg(parseError.errorString());
        statusLabel->setText(errorMsg);
        QMessageBox::critical(this, "JSON Parse Error", errorMsg + "\nOutput:\n" + QString(output));
        return;
    }

    if (!doc.isArray()) {
        statusLabel->setText("JSON output is not an array");
        QMessageBox::critical(this, "JSON Format Error", "Expected JSON array of articles");
        return;
    }

    QJsonArray articles = doc.array();
    articleList->clear();
    articleTexts.clear();
    
    for (const auto &articleVal : articles) {
        if (!articleVal.isObject())
            continue;

        QJsonObject articleObj = articleVal.toObject();
        QString title = articleObj.value("title").toString();
        QString text = articleObj.value("text").toString();

        if (!title.isEmpty() && !text.isEmpty()) {
            QWidget* itemWidget = new QWidget;
            QHBoxLayout* layout = new QHBoxLayout(itemWidget);
            
            QLabel* titleLabel = new QLabel(title);
            QPushButton* deleteBtn = new QPushButton("🗑", itemWidget);  // Add parent
            deleteBtn->setFixedSize(24, 24);
            deleteBtn->setStyleSheet(
                "QPushButton { border: none; border-radius: 12px; }"
                "QPushButton:hover { background-color: #ffebee; }"
            );
            
            layout->addWidget(titleLabel);
            layout->addWidget(deleteBtn);
            layout->addStretch();
            layout->setContentsMargins(5, 2, 5, 2);
            
            QListWidgetItem* item = new QListWidgetItem();
            item->setSizeHint(itemWidget->sizeHint());
            articleList->addItem(item);
            articleList->setItemWidget(item, itemWidget);
            
            articleTexts.insert(title, text);
            
            connect(deleteBtn, &QPushButton::clicked, this, [this, title]() {
                onDeleteArticle(title);
            });
        }
    }
    
    statusLabel->setText(QString("Trovati %1 articoli").arg(articleTexts.size()));
}

void Articoli::onDeleteArticle(const QString &title)
{
    // Remove article from map and list
    articleTexts.remove(title);
    
    // Find and remove the item from the list widget
    for (int i = 0; i < articleList->count(); ++i) {
        QWidget* widget = articleList->itemWidget(articleList->item(i));
        if (widget) {
            QLabel* label = widget->findChild<QLabel*>();
            if (label && label->text() == title) {
                delete articleList->takeItem(i);
                break;
            }
        }
    }
    
    // If we need more articles, run the Python script again
    if (articleTexts.size() < articlesPerPaper) {
        runPythonScript();
    }
}

void Articoli::onProcessError(QProcess::ProcessError error)
{
    QString errorMessage;
    switch (error) {
        case QProcess::FailedToStart: errorMessage = "Failed to start Python script"; break;
        case QProcess::Crashed: errorMessage = "Python script crashed"; break;
        case QProcess::Timedout: errorMessage = "Operation timed out"; break;
        case QProcess::WriteError: errorMessage = "Failed to write to Python script"; break;
        case QProcess::ReadError: errorMessage = "Failed to read from Python script"; break;
        default: errorMessage = "Unknown error occurred"; break;
    }

    statusLabel->setText("Error: " + errorMessage);
    elapsedTimer->stop();

    QString stdErr = pythonProcess->readAllStandardError();
    QMessageBox::critical(this, "Error",
                          "Failed to retrieve articles: " + errorMessage + "\n" + stdErr);
}

void Articoli::updateElapsedTime()
{
    qint64 elapsed = timer.elapsed();
    int seconds = static_cast<int>(elapsed / 1000);
    int minutes = seconds / 60;
    seconds %= 60;

    timeLabel->setText(QString("Tempo trascorso: %1:%2")
                       .arg(minutes)
                       .arg(seconds, 2, 10, QChar('0')));
}

void Articoli::onItemDoubleClicked(QListWidgetItem *item)
{
    QWidget* widget = articleList->itemWidget(item);
    if (!widget)
        return;
        
    QLabel* label = widget->findChild<QLabel*>();
    if (!label)
        return;
        
    QString title = label->text();
    QString text = articleTexts.value(title);
    showArticleText(title, text);
}

QString escapeForAppleScriptShell(const QString &input) {
    QString output = input;

    // Remove newlines, tabs, carriage returns
    output.replace(QRegularExpression("[\\n\\r\\t]"), " ");

    // Collapse multiple spaces into one
    output.replace(QRegularExpression(" {2,}"), " ");

    // Escape shell-sensitive characters
    output.replace("\\", "\\\\");   // backslash
    output.replace("\"", "\\\"");   // double quote
    output.replace("$", "\\$");     // dollar sign
    output.replace("`", "\\`");     // backtick

    // Remove content inside parentheses (optional, but your previous code tried)
    // If you want to remove all content inside parentheses, use a regex:
    output.replace(QRegularExpression("\\([^\\)]*\\)"), "");

    return output.trimmed();
}


void Articoli::showArticleText(const QString &title, const QString &text)
{
    QDialog *textDialog = new QDialog(this);
    textDialog->setWindowTitle(title);
    textDialog->resize(800, 500);

    QHBoxLayout *mainLayout = new QHBoxLayout(textDialog);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, textDialog);

    QTextEdit *textEdit = new QTextEdit(textDialog);
    textEdit->setReadOnly(true);
    textEdit->setText(text);

    static QPointer<TranslateHelper> helper;
    helper = new TranslateHelper(textEdit, textDialog);

    QWidget *rightWidget = new QWidget(textDialog);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QPushButton *generateBtn = new QPushButton("Genera domande", rightWidget);
    generateBtn->setFixedSize(150, 40);
    generateBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
    );

    // Container for questions and answers (will be filled after generation)
    QWidget *qaContainer = new QWidget(rightWidget);
    QVBoxLayout *qaLayout = new QVBoxLayout(qaContainer);
    qaLayout->setSpacing(10);
    qaLayout->setContentsMargins(0, 0, 0, 0);
    qaContainer->setLayout(qaLayout);

    rightLayout->addWidget(generateBtn);
    rightLayout->addWidget(qaContainer, 1);
    rightWidget->setLayout(rightLayout);

    splitter->addWidget(textEdit);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({textDialog->width() / 2, textDialog->width() / 2});

    mainLayout->addWidget(splitter);
    textDialog->setLayout(mainLayout);

    connect(generateBtn, &QPushButton::clicked, textDialog, [=]() {
        int n = Impostazioni::getNumeroDomande();
        QString prompt = QString("Genera %1 domande in lingua TEDESCA a partire da questo testo, fornendo solo queste domande, una per paragrafo, senza commenti ulteriori prima, durante o dopo le domande: \"%2\"").arg(n).arg(text);

        QString escapedPrompt = escapeForAppleScriptShell(prompt);

        QString outputPath = "/Users/riccardoorsi/Desktop/deutschlernen/domande_output.txt";
        // Use -y to disable spinner, and redirect stderr to stdout to avoid blocking
        QString shellCommand = QString("OLLAMA_NO_TTY=1 NO_COLOR=1 ollama run gemma3n:e2b \\\"%1\\\" > \\\"%2\\\"").arg(escapedPrompt, outputPath);
        QString osaScript = QString("do shell script \"%1\"").arg(shellCommand);

        QProcess *process = new QProcess(rightWidget);
        QObject::connect(process, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus status) {
            // Read the output from the process for debugging
            QString stdOut = QString::fromUtf8(process->readAllStandardOutput());
            QString stdErr = QString::fromUtf8(process->readAllStandardError());
            if (exitCode != 0 || status != QProcess::NormalExit) {
                QMessageBox::warning(textDialog, "Errore", "Errore durante la generazione delle domande.\n\nstdout:\n" + stdOut + "\nstderr:\n" + stdErr);
                process->deleteLater();
                return;
            }

            QTimer::singleShot(1000, [=]() {
                QFile file(outputPath);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QMessageBox::warning(textDialog, "Errore", "Impossibile leggere il file domande_output.txt");
                    return;
                }

                QString content = QTextStream(&file).readAll();
                file.close();

                // Parse questions and create answer fields
                // Clear previous widgets
                QLayoutItem *child;
                while ((child = qaLayout->takeAt(0)) != nullptr) {
                    if (child->widget()) child->widget()->deleteLater();
                    delete child;
                }

                QStringList questions;
                for (const QString &line : content.split('\n')) {
                    QString trimmed = line.trimmed();
                    if (!trimmed.isEmpty())
                        questions << trimmed;
                }

                for (int i = 0; i < questions.size(); ++i) {
                    const QString &q = questions[i];
                    // Extract only the short prefix (up to and including the first '.')
                    QString shortPrefix = q;
                    int dotIdx = shortPrefix.indexOf('.');
                    if (dotIdx != -1)
                        shortPrefix = shortPrefix.left(dotIdx + 1);

                    QWidget *row = new QWidget(qaContainer);
                    QHBoxLayout *rowLayout = new QHBoxLayout(row);
                    rowLayout->setContentsMargins(0, 0, 0, 0);

                    QLabel *qLabel = new QLabel(shortPrefix, row);
                    qLabel->setWordWrap(true);

                    QLineEdit *answerEdit = new QLineEdit(row);
                    answerEdit->setPlaceholderText("Risposta...");

                    QPushButton *zoomBtn = new QPushButton("🔍", row);
                    zoomBtn->setFixedWidth(32);

                    QPushButton *invioBtn = new QPushButton("Invio", row);
                    invioBtn->setFixedWidth(60);

                    rowLayout->addWidget(qLabel, 1);
                    rowLayout->addWidget(answerEdit, 2);
                    rowLayout->addWidget(zoomBtn, 0);
                    rowLayout->addWidget(invioBtn, 0);
                    row->setLayout(rowLayout);
                    qaLayout->addWidget(row);

                    // Zoom dialog for this question
                    connect(zoomBtn, &QPushButton::clicked, row, [q, answerEdit, textDialog]() {
                        QDialog *zoomDialog = new QDialog(textDialog);
                        zoomDialog->setWindowTitle("Risposta estesa");
                        zoomDialog->resize(600, 350);
                        QVBoxLayout *zoomLayout = new QVBoxLayout(zoomDialog);

                        QLabel *fullQ = new QLabel(q, zoomDialog);
                        fullQ->setWordWrap(true);
                        QFont boldFont = fullQ->font();
                        boldFont.setBold(true);
                        fullQ->setFont(boldFont);

                        QTextEdit *bigAnswer = new QTextEdit(zoomDialog);
                        bigAnswer->setPlaceholderText("Scrivi la tua risposta qui...");
                        bigAnswer->setText(answerEdit->text());

                        QPushButton *okBtn = new QPushButton("OK", zoomDialog);
                        okBtn->setDefault(true);

                        zoomLayout->addWidget(fullQ);
                        zoomLayout->addWidget(bigAnswer, 1);
                        zoomLayout->addWidget(okBtn, 0, Qt::AlignRight);

                        QObject::connect(okBtn, &QPushButton::clicked, zoomDialog, [=]() {
                            answerEdit->setText(bigAnswer->toPlainText());
                            zoomDialog->accept();
                        });

                        zoomDialog->exec();
                        delete zoomDialog;
                    });

                    // Invio button: run ollama correction via osascript
                    connect(invioBtn, &QPushButton::clicked, row, [=]() {
                        QString rispostaUtente = answerEdit->text();
                        if (rispostaUtente.trimmed().isEmpty()) {
                            QMessageBox::information(textDialog, "Invio", "Risposta vuota.");
                            return;
                        }

                        // Compose the prompt
                        QString correzionePrompt = QString(
                            "domanda: %1, risposta %2, Correggi la risposta a questa domanda, fornendo solo la risposta corretta"
                        ).arg(q, rispostaUtente);

                        // Escape for AppleScript
                        QString correzionePromptEscaped = escapeForAppleScriptShell(correzionePrompt);

                        QString outputPath = "/Users/riccardoorsi/Desktop/deutschlernen/controlla_risposte.txt";
                        QString shellCommand = QString("OLLAMA_NO_TTY=1 NO_COLOR=1 ollama run gemma3n:e2b \\\"%1\\\" > \\\"%2\\\"")
                            .arg(correzionePromptEscaped, outputPath);
                        QString osaScript = QString("do shell script \"%1\"").arg(shellCommand);

                        QProcess *proc = new QProcess(row);
                        QObject::connect(proc, &QProcess::finished, row, [=](int exitCode, QProcess::ExitStatus status) {
                            QTimer::singleShot(1000, [=]() {
                                QFile file(outputPath);
                                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                                    QMessageBox::warning(textDialog, "Errore", "Impossibile leggere il file controlla_risposte.txt");
                                    proc->deleteLater();
                                    return;
                                }
                                QString correzione = QTextStream(&file).readAll();
                                file.close();

                                // Show result in a dialog
                                QDialog *correzioneDialog = new QDialog(textDialog);
                                correzioneDialog->setWindowTitle("Risposta corretta");
                                QVBoxLayout *layout = new QVBoxLayout(correzioneDialog);
                                QTextEdit *view = new QTextEdit(correzione);
                                view->setReadOnly(true);
                                layout->addWidget(view);
                                correzioneDialog->resize(500, 200);
                                correzioneDialog->exec();
                                delete correzioneDialog;
                                proc->deleteLater();
                            });
                        });

                        proc->start("osascript", QStringList() << "-e" << osaScript);
                    });
                }
                qaLayout->addStretch(1);
            });

            process->deleteLater();
        });

        // Clear previous questions/answers
        QLayoutItem *child;
        while ((child = qaLayout->takeAt(0)) != nullptr) {
            if (child->widget()) child->widget()->deleteLater();
            delete child;
        }

        process->start("osascript", QStringList() << "-e" << osaScript);
    });

    textDialog->exec();
    delete textDialog;
}


// eventFilter rewritten similarly:
bool Articoli::eventFilter(QObject *obj, QEvent *event)
{
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(obj);
    if (textEdit && event->type() == QEvent::MouseButtonDblClick) {
        // Add debugging
        qDebug() << "Double click detected on QTextEdit";
        
        // Get selected text - might need to use a timer to get selection after event processing
        QTimer::singleShot(0, [this, textEdit]() {
            QString selectedText = textEdit->textCursor().selectedText();
            qDebug() << "Selected text:" << selectedText;
            
            if (!selectedText.trimmed().isEmpty()) {
                createTranslationDialog(selectedText);
            } else {
                qDebug() << "No text selected";
            }
        });
        
        return true;
    }
    return QDialog::eventFilter(obj, event);
}

void Articoli::createTranslationDialog(const QString& selectedText)
{
    QDialog *transDialog = new QDialog(this);
    transDialog->setWindowTitle("Traduzione");
    transDialog->resize(500, 300);
    transDialog->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(transDialog);
    QLabel *label = new QLabel("Output comando 'ls':", transDialog);
    QPlainTextEdit *output = new QPlainTextEdit(transDialog);
    output->setReadOnly(true);
    layout->addWidget(label);
    layout->addWidget(output);

    QString command = "ls";
    QStringList args = {};

    QProcess *process = new QProcess(transDialog);
    // Use separate channels for clearer output handling
    process->setProcessChannelMode(QProcess::SeparateChannels);

    // Connect signals BEFORE starting the process
    QObject::connect(process, &QProcess::readyReadStandardOutput, [process, output]() {
        QByteArray data = process->readAllStandardOutput();
        if (!data.isEmpty()) {
            output->moveCursor(QTextCursor::End);
            output->insertPlainText(QString::fromUtf8(data));
            output->ensureCursorVisible();
        }
    });

    QObject::connect(process, &QProcess::readyReadStandardError, [process, output]() {
        QByteArray err = process->readAllStandardError();
        if (!err.isEmpty()) {
            output->moveCursor(QTextCursor::End);
            output->insertPlainText("ERROR: " + QString::fromUtf8(err));
            output->ensureCursorVisible();
        }
    });

    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [process, output](int exitCode, QProcess::ExitStatus exitStatus) {
        // Read any remaining data
        QByteArray data = process->readAllStandardOutput();
        if (!data.isEmpty()) {
            output->moveCursor(QTextCursor::End);
            output->insertPlainText(QString::fromUtf8(data));
        }
        QByteArray err = process->readAllStandardError();
        if (!err.isEmpty()) {
            output->moveCursor(QTextCursor::End);
            output->insertPlainText("ERROR: " + QString::fromUtf8(err));
        }
        output->moveCursor(QTextCursor::End);
        output->insertPlainText(QString("\n--- Processo terminato (Exit Code: %1, Status: %2) ---\n")
                               .arg(exitCode).arg(exitStatus == QProcess::NormalExit ? "Normal" : "Crash"));
        output->ensureCursorVisible();
        process->deleteLater();
    });

    // Handle dialog closure
    QObject::connect(transDialog, &QDialog::finished, [process](int) {
        if (process && process->state() != QProcess::NotRunning) {
            process->kill();
            process->waitForFinished(3000);
        }
    });

    // Alternative: Synchronous approach
    output->clear();
    output->appendPlainText("Esecuzione comando ls...\n");
    
    process->start(command, args);
    
    if (process->waitForStarted(3000)) {
        if (process->waitForFinished(5000)) {
            // Process completed, read all output
            QByteArray stdOut = process->readAllStandardOutput();
            QByteArray stdErr = process->readAllStandardError();
            
            if (!stdOut.isEmpty()) {
                output->appendPlainText(QString::fromUtf8(stdOut));
            }
            if (!stdErr.isEmpty()) {
                output->appendPlainText("ERROR: " + QString::fromUtf8(stdErr));
            }
            
            output->appendPlainText(QString("\n--- Completato (Exit Code: %1) ---")
                                   .arg(process->exitCode()));
        } else {
            output->appendPlainText("Timeout: Il comando ha impiegato troppo tempo");
            process->kill();
        }
    } else {
        output->appendPlainText("Errore: Impossibile avviare il comando ls");
    }

    transDialog->exec();
}