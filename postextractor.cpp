#include "postextractor.h"
#include "ui_postextractor.h"
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QSize>
#include <QDateTime>
#include <QWebFrame>
#include <QMessageBox>
#include <QDebug>

PostExtractor::PostExtractor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostExtractor)
{
    ui->setupUi(this);
    pisah = new QSplitter(parent);
    listView = new QListView;
    tampilHtml = new QWebView(parent);
    QPushButton *namaSimpan = new QPushButton("Simpan sbg");
    QPushButton *simpan = new QPushButton("Simpan HTML");
    QPushButton *simpanTxt = new QPushButton("Simpan Text");
    jalurSimpan = new QLineEdit();
    QGridLayout *kisi = new QGridLayout(parent);
    QHBoxLayout *jajar = new QHBoxLayout(parent);
    jajar->addWidget(namaSimpan);
    jajar->addWidget(jalurSimpan);
    jajar->addWidget(simpan);
    jajar->addWidget(simpanTxt);
    pisah->addWidget(listView);
    pisah->addWidget(tampilHtml);
    pisah->setStretchFactor(1,1);
    kisi->addWidget(pisah,0,0);

    kisi->addLayout(jajar,1,0);
    kisi->setColumnStretch(0,1);
    ui->centralWidget->setLayout(kisi);
    ramban = new QFileSystemModel;

    connect(namaSimpan,SIGNAL(clicked()),this,SLOT(namaSimpan_clicked()));
    connect(simpan,SIGNAL(clicked()),this,SLOT(simpan_clicked()));
    connect(simpanTxt,SIGNAL(clicked()),this,SLOT(simpanTxt_clicked()));
    connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(listview_clicked(QModelIndex)));
}

PostExtractor::~PostExtractor()
{
    delete ui;
}

void PostExtractor::on_actionQuit_triggered()
{
    qApp->quit();
}

void PostExtractor::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void PostExtractor::on_actionOpen_Folder_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this,"Pilih direktori",QDir::homePath(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) {
    ramban->setRootPath(dir);
    ramban->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ramban->setNameFilters(QStringList() << "*.json");
    ramban->setNameFilterDisables(false);
    listView->setModel(ramban);
    listView->setRootIndex(ramban->index(dir));
    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(80,80));
    }
}

void PostExtractor::namaSimpan_clicked()
{
    QString simpanNama = QFileDialog::getSaveFileName(this,"Simpan dengan nama",
                                                      jalurSimpan->text(),
                                                      "Teks atau HTML (*.txt *.html *.htm)");
    if(!simpanNama.isEmpty())
        jalurSimpan->setText(simpanNama);
}

void PostExtractor::simpan_clicked()
{
    QFile simpanan(jalurSimpan->text());
    if(simpanan.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString simpanTeks = tampilHtml->page()->currentFrame()->toHtml();
        simpanan.write(simpanTeks.toUtf8());
        QMessageBox::information(this,"Berhasil","File sudah tersimpan: "+jalurSimpan->text());
    }
    else
    {
        tampilHtml->setHtml("<h2>Tidak bisa menyimpan</h2>");
    }
    simpanan.close();
}

void PostExtractor::simpanTxt_clicked()
{
    QFile simpanan(jalurSimpan->text());
    if(simpanan.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString simpanTeks = tampilHtml->page()->currentFrame()->toPlainText();
        simpanan.write(simpanTeks.toUtf8());
        QMessageBox::information(this,"Berhasil","File sudah tersimpan: "+jalurSimpan->text());
    }
    else
    {
        tampilHtml->setHtml("<h2>Tidak bisa menyimpan teks</h2>");
    }
    simpanan.close();
}

void PostExtractor::listview_clicked(const QModelIndex &index)
{
    QString jalurFile = ramban->fileInfo(index).absoluteFilePath();
    QFile file(jalurFile);
    jalurSimpan->setText(jalurFile.replace(".json",".html"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray isiJson = file.readAll();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(isiJson,&err);
    QJsonObject objek = doc.object();
    if(!objek.value("found").isNull()){
        QJsonValue posting = objek.value("posts");
        QJsonArray isiPos = posting.toArray();

        QString html;
        ui->statusBar->showMessage("Loading....");
        for(int i = isiPos.size(); i-- > 0;){
            QJsonObject isi = isiPos.at(i).toObject();
            QDateTime tanggal = QDateTime::fromString(isi.value("date").toString(),Qt::ISODate);
            QString tanggalPos = tanggal.toString("<p>dd MMMM yyyy hh:mm</p>");

            html.append("<h2>"+isi.value("title").toString()+"</h2>\n");
            html.append(tanggalPos);
            html.append("<p>"+isi.value("short_URL").toString()+"</p>");
            html.append(isi.value("content").toString());
        }
        tampilHtml->setHtml(html);
        ui->statusBar->showMessage("Selesai.");
    }
    else
    {
        tampilHtml->setHtml("<h2>Bukan file yang dimaksud</h2>");
    }
    file.close();

}

void PostExtractor::on_actionAbout_triggered()
{
    QMessageBox tentang;
    tentang.setText(tr("Ekstraktor postingan blog WP"));
    tentang.setInformativeText(tr("Mengubah postingan blog wordpress.com yang diunduh dengan API.\n\n"
                                  "(c) 2014 Slamet Badwi"));
    tentang.setIcon(QMessageBox::Information);
    tentang.exec();
}
