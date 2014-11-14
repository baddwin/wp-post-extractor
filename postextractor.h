#ifndef POSTEXTRACTOR_H
#define POSTEXTRACTOR_H

#include <QMainWindow>
#include <QSplitter>
#include <QListView>
#include <QPlainTextEdit>
#include <QFileSystemModel>
#include <QLineEdit>

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
    void listview_clicked(const QModelIndex &index);

private:
    Ui::PostExtractor *ui;
    QSplitter *pisah;
    QListView *listView;
    QTextEdit *plainText;
    QLineEdit *jalurSimpan;
    QFileSystemModel *ramban;
};

#endif // POSTEXTRACTOR_H
