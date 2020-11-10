#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class GUI; }
QT_END_NAMESPACE

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget *parent = nullptr);
    ~GUI();

    void plotDynamicCircle();
    void plotCircle(int r, int g, int b,QVector<double> center,double radius);
    void plotInitial();
    void calBluePoints();
    double getRadius(double x1, double y1, double x2, double y2);
    void showBluePoints();
    void plotOuterCircles();


private slots:
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);

private:
    Ui::GUI *ui;
    QVector<double> qv_x, qv_y;     // variables for 20x20 grids
    QVector<double> center;         // 2 values represent the center of blue circle
    QVector<double> shown_x, shown_y;   // variables for the blue points
    double radius;              // radius of the blue circle
    bool notRelease = true;     // check to see if the user release the button
};
#endif // GUI_H
