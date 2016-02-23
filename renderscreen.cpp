#include "renderscreen.h"
#include "ui_renderscreen.h"

renderScreen::renderScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::renderScreen)
{
    ui->setupUi(this);
}

renderScreen::~renderScreen()
{
    delete ui;
}

void renderScreen::updateScreen(QPixmap * myFrame)
{
    ui->screen->setPixmap(*myFrame);
}
