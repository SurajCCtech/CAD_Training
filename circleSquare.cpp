#include <vector>
#include <math.h>
#include <fstream>
#include <iomanip>

using namespace std;

class Point;
typedef vector<Point> Curve;
typedef vector<Curve> Surface;

class Point
{
public:
    Point() {}
    Point(double data[3])
    {
        for (int i = 0; i < 3; i++)
        {
            this->data[i] = data[i];
        }
    }
    Point(double x, double y, double z)
    {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
    }
    double data[3];
};

bool generateCircle(Point &centrePoint, double &radius, int &numU, Curve &circle)
{
    double deltatheta = 2 * M_PI / (numU);
    for (double i = 0; i < 2 * M_PI; i += deltatheta)
    {
        Point p;
        p.data[0] = centrePoint.data[0] + radius * cos(i);
        p.data[1] = centrePoint.data[0] + radius * sin(i);
        p.data[2] = 10;
        circle.push_back(p);
    }
    return 1;
}

bool generateLine(Point p1, Point p2, int numU, Curve &line)
{
    for (double u = 0; u < 1; u += 1 / (double)(numU))
    {
        Point p;
        for (int j = 0; j < 3; j++)
            p.data[j] = (p1.data[j] * (1 - u)) + (p2.data[j] * u);
        line.push_back(p);
    }
    return 1;
}

bool generateSquare(Point p1, Point p2, Point p3, Point p4, int &numU, Curve &square)
{
    Curve line[4];
    generateLine(p1, p2, numU/4, line[0]);
    generateLine(p2, p3, numU/4, line[1]);
    generateLine(p3, p4, numU/4, line[2]);
    generateLine(p4, p1, numU/4, line[3]);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < numU/4; j++)
            square.push_back(line[i][j]);
    return 1;
}

bool surfaceLoft(Curve &circle, Curve &square, int &numV, Surface &loftSurface)
{
    if (circle.size() != square.size())
        return 0;
    double deltaV = 1.0 / (numV - 1);
    for (double v = 0; v <= 1.0; v += deltaV)
    {
        Curve c;
        c.resize(circle.size());
        for (int i = 0; i < circle.size(); i++)
        {
            for (int j = 0; j < 3; j++)
                c[i].data[j] = (circle[i].data[j] * (1 - v)) + (square[i].data[j] * v);
        }
        loftSurface.push_back(c);
    }
    return 1;
}

bool displayGNU(Surface loftSurface)
{
    ofstream fout("gnuScript.plt", ios::out | ios::trunc);
    fout << "set title 'Line Bezier Loft'" << endl;
    fout << "set view equal xyz" << endl;
    fout << "set hidden3d" << endl;
    fout << "set view 1,1,1" << endl;
    fout << "splot 'circleSquareOutput.gnu' with linespoints" << endl;
    fout << endl
         << "pause -9 \"Hit Enter\" " << endl;
    fout.close();
    system("gnuplot gnuScript.plt");
    return true;
}

// void displayGNU(Surface surface)
// {
//     ofstream outfile("G:\\CCTech\\CAD_Training\\loft_surfaceL2CB.dat");
//     outfile << setprecision(4);
//     for (Curve c : surface)
//     {
//         for (Point p : c)
//         {
//             outfile << p.data[0] << " " << p.data[1] << " " << p.data[2] << endl;
//         }
//     }
//     outfile.close();
// }

int main()
{
    Curve circle;
    Curve square;
    Surface loftSurface;

    Point centrePoint(5,5,0);
    int numU = 40,numV=50;
    double radius = 5.0;
    generateCircle(centrePoint, radius, numU, circle);

    Point sqP1(1, 1, 0);
    Point sqP2(10, 1, 0);
    Point sqP3(10, 10, 0);
    Point sqP4(1, 10, 0);
    generateSquare(sqP1, sqP2, sqP3, sqP4, numU, square);

    surfaceLoft(circle,square,numV,loftSurface);
    ofstream fout("circleSquareOutput.gnu");
    for (auto item : loftSurface)
    {
        for (auto subItem : item)
            fout << subItem.data[0] << "\t" << subItem.data[1] << "\t" << subItem.data[2] << "\n";
    }
    displayGNU(loftSurface);

    return 1;
}
