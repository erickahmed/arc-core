#include <gp_Pnt.hxx>

Point* make_point(double x, double y, double z) {
    gp_Pnt* p = new gp_Pnt(x, y, z);
    return reinterpret_cast<Point*>(p);
}

void free_point(Point* p) {
    delete reinterpret_cast<gp_Pnt*>(p);
}

void coord_point(const Point* p, double* x, double* y, double* z) {
    const gp_Pnt* gp = reinterpret_cast<const gp_Pnt*>(p);
    *x = gp->X();
    *y = gp->Y();
    *z = gp->Z();
}
