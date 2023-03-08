#define _USE_MATH_DEFINES
#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

struct Point
{
    double data[3];
};
typedef vector<Point> Curve;
typedef vector<Curve> Surface;
bool surfaceLoft(Curve c1, Curve c2, int numV, Surface& loftSurface)
{
    if (c1.size() != c2.size())
        return false;
    double deltaV = 1.0 / numV;
    for (double v = 0; v <= 1.0; v += deltaV)
    {
        Curve c;
        c.resize(c1.size());
        for (int i = 0; i < c1.size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                c[i].data[j] = c1[i].data[j] * (1 - v) + c2[i].data[j] * v;
            }
        }
        loftSurface.push_back(c);
    }
    return true;
}
void generateCircle(Point centrePoint, double radius, int numU, Curve& circle)
{
    double deltaU = 2 * M_PI / numU;
    for (double i = 0; i <= 2 * M_PI ; i+= deltaU)
    {
        Point p;
        p.data[0] = centrePoint.data[0] + radius * cos(i);
        p.data[1] = centrePoint.data[1] + radius * sin(i);
        p.data[2] = centrePoint.data[2];
        circle.push_back(p);
    }
}
void generateEllipse(Point centrePoint, double radiusA, double radiusB, int numU, Curve& ellipse)
{
    double deltaU = 2 * M_PI / numU;
    for (double i = 0; i <= 2 * M_PI ; i+= deltaU)
    {
        Point p;
        p.data[0] = centrePoint.data[0] + radiusA * cos(i);
        p.data[1] = centrePoint.data[1] + radiusB * sin(i);
        p.data[2] = centrePoint.data[2] + 50 ;
        ellipse.push_back(p);
    }
}
void displayGNU(Surface surface)
{
    ofstream outfile("G:\\CCTech\\CAD_Training\\loft_surfaceC2E.dat");
    outfile << setprecision(4);
    for (Curve c : surface)
    {
        for (Point p : c)
        {
            outfile << p.data[0] << " " << p.data[1] << " " << p.data[2] << endl;
        }
        //outfile << endl;
    }
    outfile.close();
}
int main()
{
    Curve circle;
    Curve ellipse;
    Surface loftSurface;
    int numU = 50;
    int numV = 50;
    Point centrePoint = { {0, 0, 0} };
    double radius = 10;
    double radiusA = 15;
    double radiusB = 5;
    generateCircle(centrePoint, radius, numU, circle);
    generateEllipse(centrePoint, radiusA, radiusB, numU, ellipse);
    surfaceLoft(circle, ellipse, numV, loftSurface);
    displayGNU(loftSurface);
    return 0;
}