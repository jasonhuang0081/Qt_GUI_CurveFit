#include "gui.h"
#include "ui_gui.h"
#include <math.h>

GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI)
{
    ui->setupUi(this);

    // generate initial 20x20 points
    for(int i = 0;i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            qv_x.append(i);
            qv_y.append(j);
        }
    }
    plotInitial();

    // set up the signal to the corresponding listener
    connect(ui->myPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));
    connect(ui->myPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(onMousePress(QMouseEvent*)));
    connect(ui->myPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(onMouseRelease(QMouseEvent*)));
}

GUI::~GUI()
{
    delete ui;
}

/*
 * This function plot the initial grids of 20x20
*/
void GUI::plotInitial()
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
 * This function the red circle dynamically by clear the plot and re-plot the frame
*/
void GUI::plotDynamicCircle()
{
    ui->myPlot->clearPlottables();
    plotInitial();
    plotCircle(0, 0, 255, center, radius);

}

/*
 * This is function traverse the points of the circle 360 degrees by the given radisu
 * and center of the circle, using the object Curve
 *
*/
void GUI::plotCircle(int r, int g, int b, QVector<double> center, double radius)
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
 * Listener to detect the mose movement
*/
void GUI::onMouseMove(QMouseEvent *event)
{
    // access cursor location
    double x, y;
    x = this->ui->myPlot->xAxis->pixelToCoord(event->pos().x());
    y = this->ui->myPlot->yAxis->pixelToCoord(event->pos().y());

    // check if the center of the circle has been click yet so it won't radomly trigger
    // also check if the user release the mouse yet so it won't continuously draw circles
    if (center.size() != 0 && notRelease)
    {
        radius = getRadius(x,y,center[0],center[1]);
        plotDynamicCircle();
    }
}

/*
 * Listener to detect the mose press
*/
void GUI::onMousePress(QMouseEvent *event)
{
    // access cursor location
    double x, y;
    x = ui->myPlot->xAxis->pixelToCoord(event->pos().x());
    y = ui->myPlot->yAxis->pixelToCoord(event->pos().y());

    // record center
    center.append(x);
    center.append(y);

    // reset this so user can draw circle multiple times without restarting program
    notRelease = true;

}

/*
 * Listener to detect the mose release and execute a series of function to plot blue dots
 * and two red circles
*/
void GUI::onMouseRelease(QMouseEvent *event)
{
    // access cursor location
    double x, y;
    x = ui->myPlot->xAxis->pixelToCoord(event->pos().x());
    y = ui->myPlot->yAxis->pixelToCoord(event->pos().y());

    // set this to false so movement listener won't continuous draw circles
    notRelease = false;
    plotDynamicCircle();
    calBluePoints();
    showBluePoints();
    plotOuterCircles();

    // clear the center point for the next circle
    center.clear();
}

/*
 * This function calculate the radius of the maximum and minimum radius by using the
 * selected shown_x and shown_y which are the blue dots closest to the blue circle
 * If one wants to have red circle completely untouched the blue points, one can add
 * some small value to the radius
*/
void GUI::plotOuterCircles()
{
    // set the initial value of the max to be the smallest possible value
    // while the min to be the largest possible value
    double max_radius = 0;
    double min_radius = 20;

    for (int i = 0; i < shown_x.size(); i++)
    {
        double r = getRadius(shown_x[i],shown_y[i],center[0],center[1]);
        if(r > max_radius)
        {
            max_radius = r;
        }
        if (r < min_radius)
        {
            min_radius = r;
        }
    }

    plotCircle(255, 0, 0, center, max_radius);
    plotCircle(255, 0, 0, center, min_radius);
}

/*
 * This function calculate the radius between two given points
*/
double GUI::getRadius(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

/*
 * This function plot the blue points that are closest to the blue
 * circle
*/
void GUI::showBluePoints()
{
    // use another graph to overlay the blue points on top of the gray points
    ui->myPlot->addGraph();
    ui->myPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, Qt::blue, Qt::blue, 5));
    ui->myPlot->graph(1)->setLineStyle(QCPGraph::lsNone);

    ui->myPlot->graph(1)->setData(shown_x,shown_y);
    ui->myPlot->replot();
    ui->myPlot->update();
}

/*
 * This function calculate the blue points by checking the dist between
 * itself and center minus radius is within half of the grid
 * other smaller value can used
*/
void GUI::calBluePoints()
{
    shown_x.clear();shown_y.clear();
    for(int i = 0;i < 400; i++)
    {
        if (abs(getRadius(qv_x[i],qv_y[i],center[0],center[1]) - radius) < 0.5)
        {
            shown_x.append(qv_x[i]);
            shown_y.append(qv_y[i]);
        }
    }


}
