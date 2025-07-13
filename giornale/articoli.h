#ifndef ARTICOLI_H
#define ARTICOLI_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QTimer>
#include <QProcess>
#include <QElapsedTimer>
#include <QMap>
#include <QDebug>
#include <QPair>
#include <QToolButton>
#include <QTextEdit>

class TranslateHelper : public QObject {
    Q_OBJECT
public:
    TranslateHelper(QTextEdit* edit, QWidget* parentDialog);
    bool eventFilter(QObject* obj, QEvent* event) override;
private slots:
    void onTranslateClicked();
private:
    void showButtonNearSelection();
    QTextEdit* textEdit;
    QWidget* dialog;
    QToolButton* button;
};

class Articoli : public QDialog
{
    Q_OBJECT
public:
    explicit Articoli(const QList<QPair<QString, QString>> &papers,
                      int numArticles,
                      QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void runPythonScript();
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void updateElapsedTime();
    void onItemDoubleClicked(QListWidgetItem *item);
    void onDeleteArticle(const QString &title);
    void createTranslationDialog(const QString& selectedText);

private:
    void showArticleText(const QString &title, const QString &text);

    QList<QPair<QString, QString>> newspapers;
    int articlesPerPaper;

    QListWidget *articleList;
    QLabel *statusLabel;
    QLabel *timeLabel;

    QTimer *elapsedTimer;
    QElapsedTimer timer;

    QProcess *pythonProcess;

    QMap<QString, QString> articleTexts;
};

#endif // ARTICOLI_H
