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
#include <QDebug>

PostExtractor::PostExtractor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostExtractor)
{
    ui->setupUi(this);
    pisah = new QSplitter(parent);
    listView = new QListView;
    plainText = new QTextEdit;
    QPushButton *namaSimpan = new QPushButton("Simpan sbg");
    QPushButton *simpan = new QPushButton("Simpan");
    jalurSimpan = new QLineEdit();
    QGridLayout *kisi = new QGridLayout(parent);
    QHBoxLayout *jajar = new QHBoxLayout(parent);
    jajar->addWidget(namaSimpan);
    jajar->addWidget(jalurSimpan);
    jajar->addWidget(simpan);
    pisah->addWidget(listView);
    pisah->addWidget(plainText);
    pisah->setStretchFactor(1,1);
    kisi->addWidget(pisah,0,0);

    kisi->addLayout(jajar,1,0);
    kisi->setColumnStretch(0,1);
    ui->centralWidget->setLayout(kisi);
    ramban = new QFileSystemModel;

    connect(namaSimpan,SIGNAL(clicked()),this,SLOT(namaSimpan_clicked()));
    connect(simpan,SIGNAL(clicked()),this,SLOT(simpan_clicked()));
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
    qDebug() << "Clicked namasimpan";
}

void PostExtractor::simpan_clicked()
{
    qDebug() << "Clicked simpan";
}

void PostExtractor::listview_clicked(const QModelIndex &index)
{
    //qDebug() << index;
    QString jalurFile = ramban->fileInfo(index).absoluteFilePath();
    QFile file(jalurFile);
    jalurSimpan->setText(jalurFile.replace(".json",".html"));
    //QFileInfo info(file);
    //qDebug() << info.absoluteFilePath();
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray isiJson = file.readAll();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(isiJson,&err);
    QJsonObject objek = doc.object();
    QJsonValue posting = objek.value("posts");
    QJsonArray isiPos = posting.toArray();
    //QJsonArray isi = ;
    //qDebug() << objek["found"];
    //qDebug() << posting.isArray();
    //qDebug() << isiPos.first().type();
    //QJsonObject isi = isiPos.first().toObject();
    //qDebug() << isi.value("ID");
    plainText->setText("");
    for(int i = 0; i < isiPos.size(); ++i){
        QJsonObject isi = isiPos.at(i).toObject();
        plainText->append(isi.value("title").toString());
    }
    //plainText->append("</ol>");

    /**
      Kerangka dokument JSON:

    "found":
    "posts":
     "ID":
     "site_ID":
     "author":
         "ID":
         "login":
         "email":
         "name":
         "nice_name":
         "URL":
         "avatar_URL":
         "profile_URL":
         "site_ID":
     "date":
     "modified":
     "title":
     "URL":
     "short_URL":
     "content":
     "excerpt":
     "slug":
     "guid":
     "status":
     "sticky":
     "password":
     "parent":
     "type":
     "comments_open":
     "pings_open":
     "likes_enabled":
     "sharing_enabled":
     "comment_count":
     "like_count":
     "i_like":
     "is_reblogged":
     "is_following":
     "global_ID":
     "featured_image":
     "post_thumbnail":
     "format":
     "geo":
     "publicize_URLs":
     "tags": []
     "categories":
     "attachments":
     "metadata":
     "meta":
     "current_user_can":
    */
}
