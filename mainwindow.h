#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QString>
#include<crclient.h>
#include<renderscreen.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString ipAddress;
    QString modelFilePath;
    //QWidget *renderWdg;// = new QWidget;
    renderScreen* imgScreen;


public:
    CRClient* client;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void connectIp();
    void modelFileBrowse();
    void uploadFile();
    void renderModel();
    void updateFrameBuffer_slot(QPixmap*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
