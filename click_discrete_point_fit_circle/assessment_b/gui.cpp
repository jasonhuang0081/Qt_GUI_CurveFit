#include "gui.h"
#include "ui_gui.h"


gui::gui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gui)
{
    ui->setupUi(this);

    // generate initial 20x20 points and initialization for the program
    for(int i = 0;i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            qv_x.append(i);
            qv_y.append(j);
            toggle[i][j] = -1;
        }
    }
    plotGrid();
    connect(ui->myPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));

}

gui::~gui()
{
    delete ui;
}

/*
 * This get the mouse's location when user click
 * the actual location is round up/down to the nearest int
 * for the program to select the point
*/
void gui::onMousePress(QMouseEvent *event)
{
    // access cursor location
    double x, y;
    x = ui->myPlot->xAxis->pixelToCoord(event->pos().x());
    y = ui->myPlot->yAxis->pixelToCoord(event->pos().y());
    int i = (int)round(x);
    int j = (int)round(y);
    toggle[i][j] = toggle[i][j] *-1;
    updateList();
    ui->myPlot->clearPlottables();
    plotGrid();
    plotToggle();
}

/*
 * This function look through the toggle map to check
 * if the value is -1, it means the point is unselected
 * if the value is 1, it means it is selected, so add those points
 * to ch_x, ch_y vector for plotting
*/
void gui::updateList()
{
    ch_x.clear(); ch_y.clear();
    for(int i = 0;i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            if (toggle[i][j] == 1)
            {
                ch_x.append(i);
                ch_y.append(j);
            }
        }
    }
}

/*
 * This function finds the average x and y for all the selected points
 * which serve as the center of the circle
*/
void gui::calCenter()
{
    double sum_x = 0;
    double sum_y = 0;
    center.clear();
    for(int i = 0;i < ch_x.size(); i++)
    {
        sum_x += ch_x[i];
        sum_y += ch_y[i];
    }
    double x = sum_x/ch_x.size();
    double y = sum_y/ch_y.size();
    center.append(x);
    center.append(y);
}

/*
 * This function find the radius of the blue circle by finding the
 * average length between center point and all other selected points
 *
 * If one would like to do ellipse fitting, the longest and shortest distance
 * will be chosen for plotting
*/
double gui::calRadius()
{
    QVector<double> all_radius;
    for (int i = 0; i < ch_x.size(); i++)
    {
        all_radius.append(getRadius(center[0],center[1],ch_x[i],ch_y[i]));
    }
    double sum_r = 0;
    for(int i = 0;i < all_radius.size(); i++)
    {
        sum_r += all_radius[i];
    }
    return sum_r/all_radius.size();
}

/*
 * This function calculate the radius between two given points
*/
double gui::getRadius(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

/*
 * This function plot the initial grids of 20x20
*/
void gui::plotGrid()
{
    // start the graph and set the styles
    ui->myPlot->addGraph();
    ui->myPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, Qt::black, Qt::white, 5));
    ui->myPlot->graph(0)->setLineStyle(QCPGraph::lsNone);

    // set the axis limit
    ui->myPlot->xAxis->setRange(-1, 21);
    ui->myPlot->yAxis->setRange(-1, 21);

    // plot all the data
    ui->myPlot->graph(0)->setData(qv_x,qv_y);
    ui->myPlot->replot();
}

/*
 * This function is to plot the selected points by the user
*/
void gui::plotToggle()
{
    // start the graph and set the styles
    ui->myPlot->addGraph();
    ui->myPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, Qt::blue, Qt::blue, 5));
    ui->myPlot->graph(1)->setLineStyle(QCPGraph::lsNone);

    // plot all the data
    ui->myPlot->graph(1)->setData(ch_x,ch_y);
    ui->myPlot->replot();
}

/*
 * This function is to plot the blue circle after calculating the center and radius
*/
void gui::plotCircle(int r, int g, int b, QVector<double> center, double radius)
{
    // use QCP Curve object so the circle is not shadded and has more features to use
    QCPCurve *circle = new QCPCurve(ui->myPlot->xAxis, ui->myPlot->yAxis);
    const int pointCount = 500;
    QVector<QCPCurveData> dataCircle(pointCount);

    // going around all the degrees in the circle to track all the points for plotting
    for (int i=0; i<pointCount; ++i)
    {
        double phi = (i/(double)(pointCount-1))*2*M_PI;
        dataCircle[i] = (QCPCurveData(i, center[0] + radius*qCos(phi), center[1] + radius*qSin(phi)));
    }
    circle->data()->set(dataCircle,true);
    circle->setPen(QPen(QColor(r,g,b)));

    ui->myPlot->replot();
    ui->myPlot->update();
}

/*
 * This function is trigger when "generate" button is click which will
 * plot the blue circle
*/
void gui::on_bnt_gen_clicked()
{
    calCenter();
    double r = calRadius();
    plotCircle(0,0,255, center, r);
}
