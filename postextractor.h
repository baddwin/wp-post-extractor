#ifndef POSTEXTRACTOR_H
#define POSTEXTRACTOR_H

#include <QMainWindow>
#include <QSplitter>
#include <QListView>
#include <QPlainTextEdit>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QWebView>

namespace Ui {
class PostExtractor;
}

class PostExtractor : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostExtractor(QWidget *parent = 0);
    ~PostExtractor();

private slots:
    void on_actionQuit_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionOpen_Folder_triggered();
    void namaSimpan_clicked();
    void simpan_clicked();
    void simpanTxt_clicked();
    void listview_clicked(const QModelIndex &index);

    void on_actionAbout_triggered();

private:
    Ui::PostExtractor *ui;
    QSplitter *pisah;
    QListView *listView;
    QTextEdit *plainText;
    QLineEdit *jalurSimpan;
    QWebView *tampilHtml;
    QFileSystemModel *ramban;
};

#endif // POSTEXTRACTOR_H
