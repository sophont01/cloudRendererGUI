#ifndef RENDERSCREEN_H
#define RENDERSCREEN_H

#include <QWidget>

namespace Ui {
class renderScreen;
}

class renderScreen : public QWidget
{
    Q_OBJECT

public:
    explicit renderScreen(QWidget *parent = 0);
    void updateScreen(QPixmap*);
    ~renderScreen();

private:
    Ui::renderScreen *ui;
};

#endif // RENDERSCREEN_H
