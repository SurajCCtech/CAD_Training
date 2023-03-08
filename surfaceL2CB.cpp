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

bool surfaceLoft(Curve c1, Curve c2, int numV, Surface &loftSurface)
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
void generateline(Point LP1, Point LP2, int numU, Curve &line)
{
    double deltaU = 1.0 / numU;
    for (double i = 0; i <= 1; i += deltaU)
    {
        Point p;
        p.data[0] = LP1.data[0] * (1 - i) + LP2.data[0] * i;
        p.data[1] = LP1.data[1] * (1 - i) + LP2.data[1] * i;
        p.data[2] = LP1.data[2] * (1 - i) + LP2.data[2] * i;
        line.push_back(p);
    }
}
void generateCubicBez(Point CB1, Point CB2, Point CB3, Point CB4, int numU, Curve &cubicBez)
{
    double deltaU = 1.0 / numU;
    for (double i = 0; i <= 1; i += deltaU)
    {
        Point p;
        p.data[0] = ((CB1.data[0] * (1 - i) * (1 - i) * (1 - i)) + (CB2.data[0] * i * (1 - i) * (1 - i)) + (CB3.data[0] * i * i * (1 - i)) + (CB1.data[0] * i * i * i));
        p.data[1] = ((CB1.data[1] * (1 - i) * (1 - i) * (1 - i)) + (CB2.data[1] * i * (1 - i) * (1 - i)) + (CB3.data[1] * i * i * (1 - i)) + (CB1.data[1] * i * i * i));
        p.data[0] = ((CB1.data[2] * (1 - i) * (1 - i) * (1 - i)) + (CB2.data[2] * i * (1 - i) * (1 - i)) + (CB3.data[2] * i * i * (1 - i)) + (CB1.data[2] * i * i * i));

        cubicBez.push_back(p);
    }
}
void displayGNU(Surface surface)
{
    ofstream outfile("G:\\CCTech\\CAD_Training\\loft_surfaceL2CB.dat");
    outfile << setprecision(4);
    for (Curve c : surface)
    {
        for (Point p : c)
        {
            outfile << p.data[0] << " " << p.data[1] << " " << p.data[2] << endl;
        }
    }
    outfile.close();
}
int main()
{
    Curve line;
    Curve cubicBez;
    Surface loftSurface;
    int numU = 40;
    int numV = 40;

    Point LP1 = {{5, 10, 0}};
    Point LP2 = {{6, 12, 0}};

    Point CB1 = {{1, 10, 20}};
    Point CB2 = {{3, 8, 20}};
    Point CB3 = {{5, 9, 20}};
    Point CB4 = {{7, 12, 20}};

    generateline(LP1, LP2, numU, line);
    generateCubicBez(CB1, CB2, CB3, CB4, numU, cubicBez);
    surfaceLoft(line, cubicBez, numV, loftSurface);
    displayGNU(loftSurface);
    return 0;
}