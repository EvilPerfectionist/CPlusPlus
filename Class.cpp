// This is a test file to learn C++ class
#include <iostream>

using namespace std;

class Rectangle
{
public:
    int length;         //length of rectangle
    int breadth;        //breadth of rectangle
    int height;
    int width;

    static int a;

    /* declaring member functions */
    void setLength( int l );
    void setBreadth( int b );
    int getArea();
    int getWholeLength()
    {
        return length + breadth;
    }
    Rectangle()
    {
        height = 10;
        width = 10;
    }
    static void printArea( int l, int b )
    {
        cout << l*b << endl;
    }
    static Rectangle BigRectangle( Rectangle a, Rectangle b )
    {
        Rectangle c;
        c.length = a.length + b.length;
        c.breadth = a.breadth + b.breadth;
        return c;
    }
};

/* defining member functions */
void Rectangle::setLength( int l )
{
    length = l;
}
void Rectangle::setBreadth( int b )
{
    breadth = b;
}
int Rectangle::getArea()
{
    return length * breadth;
}

int Rectangle::a = 8;

int main()
{
    Rectangle rt;
    rt.setLength(7);
    rt.setBreadth(4);
    int area = rt.getArea();
    int WholeLength = rt.getWholeLength();
    cout << "Area : " << area << endl;
    cout << "WholeLength : " << WholeLength << endl;
    cout << "Height : " << rt.height << endl;
    cout << "Width : " << rt.width << endl;
    cout << Rectangle::a << endl;
    Rectangle::printArea(4,7);
    Rectangle rt2;
    rt2.setLength(3);
    rt2.setBreadth(6);
    Rectangle rt3 = Rectangle::BigRectangle(rt, rt2);
    cout << rt3.getArea() << endl;
    return 0;
}

