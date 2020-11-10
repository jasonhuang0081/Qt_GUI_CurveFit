#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class gui; }
QT_END_NAMESPACE

class gui : public QMainWindow
{
    Q_OBJECT

public:
    gui(QWidget *parent = nullptr);
    ~gui();
    void plotGrid();
    void plotToggle();
    void updateList();
    void calCenter();
    double calRadius();
    double getRadius(double x1, double y1, double x2, double y2);
    void plotCircle(int r, int g, int b,QVector<double> center,double radius);

private slots:
    void onMousePress(QMouseEvent *event);
    void on_bnt_gen_clicked();

private:
    Ui::gui *ui;
    QVector<double> qv_x, qv_y;     //use to store initial grid points
    QVector<double> ch_x, ch_y;     // use to store points that user toggle
    int toggle[20][20];             // a map to see whether the toggle is on or off
    QVector<double> center;         // the center of circle for all toggle points calculated
};
#endif // GUI_H
