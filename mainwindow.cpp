#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->lineEdit->setInputMask("000.000.000.000; ");
    ui->widthLineEdit->setText("800");
    ui->heightLineEdit->setText("600");
    //renderWdg = new QWidget;
    imgScreen = new renderScreen;
    client = new CRClient();


    connect(ui->pushButton,SIGNAL(released()),this,SLOT(connectIp()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(modelFileBrowse()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(uploadFile()));
    connect(ui->renderButton,SIGNAL(released()),this,SLOT(renderModel()));
    connect(client,SIGNAL(updateFrameBuffer_signal(QPixmap*)),this,SLOT(updateFrameBuffer_slot(QPixmap*)));


}

void MainWindow::connectIp()
{
    this->ipAddress = ui->lineEdit->text();
    qDebug() << "ipAddress=" << ipAddress;
    QString fileList =  QString::fromStdString(client->connect(ipAddress.toStdString(),"5555"));
    QStringList fileNames = fileList.split("\n");
    ui->modelCombobox->addItems(fileNames);

}

void MainWindow::modelFileBrowse()
{
    QFileDialog modelFileBrowser;
    modelFilePath = modelFileBrowser.getOpenFileName(this, tr("Model File"),"/home",tr("Models (*.osg *.ive )"));
    qDebug() << "modelFilePath=" << modelFilePath;
    ui->lineEdit_2->setText(modelFilePath);
}

void MainWindow::uploadFile()
{

}

void MainWindow::renderModel()
{
    qDebug() << "Inside renderModel() BEFORE\n";
    client->render(ui->modelCombobox->currentText().toStdString(),ui->FPScomboBox->currentText().toStdString(),
                   ui->widthLineEdit->text().toStdString(),ui->heightLineEdit->text().toStdString());
    qDebug() << "Inside renderModel() AFTER\n";
    //imgScreen->show();
    qDebug() << "imgScreen->show() complete\n";
}

void MainWindow::updateFrameBuffer_slot(QPixmap* myFrame)
{
    qDebug() << "Updating Screen\n";
    imgScreen->updateScreen(myFrame);
    qDebug() << "Updated\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}
