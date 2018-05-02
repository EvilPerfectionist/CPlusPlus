# include <Eigen/Core>
# include <iostream>
using namespace std ;
using namespace Eigen ;

typedef Matrix <float , 3 , 1 > Vector3f ;
typedef Matrix <double , 4 , 1 > Vector4d ;

int main () {
    cout << " Eigen version : " << EIGEN_MAJOR_VERSION << " . "
         << EIGEN_MINOR_VERSION << endl ;

    Matrix3f A ;
    Matrix4d B ;
    Matrix3f C ;
    // Initialize A
    A << 1.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 1.0f;
    cout << A (1 , 2) << "\n";

    C = Matrix3f::Identity();
    cout << C << "\n";

    Matrix4f D = Matrix4f::Ones();
    //D.topRightCorner(4, 1) << 0, 0, 0, -1;
    D.bottomRightCorner(1, 1) << 0;
    cout << D << "\n";

    Matrix4f M1 = Matrix4f::Random();
    Matrix4f M2 = Matrix4f::Constant(2.2);

    cout << M1 + M2 << endl ;
    cout << M1 * M2 << endl ;
    cout << M2 - Matrix4f::Ones() * 2.2 << endl ;
    cout << ( M2 - Matrix4f :: Ones () * 2.2 == Matrix4f :: Zero () )
         << endl ;
    cout << M1.transpose() << endl ;

    //Applying an operation to a matrix element-wise
    cout << M1.array().square() << endl ;
    cout << M1.array() * Matrix4f::Identity().array() << endl ;

    //Vectors
    Vector3f v1 = Vector3f::Ones();
    Vector3f v2 = Vector3f::Zero();
    Vector4d v3 = Vector4d::Random();
    Vector4d v4 = Vector4d::Constant(1.8);

    cout << v1 + v2 << endl << endl ;
    cout << v4 - v3 << endl ;
    cout << v4 * 2 << endl ;
    cout << (Vector2f::Ones() * 3 == Vector2f::Constant(3)) << endl;

    Vector4f v5 = Vector4f(1.0f, 2.0f, 3.0f, 4.0f);
    cout << Matrix4f::Random() * v5 << endl ;

    return 0;
}
