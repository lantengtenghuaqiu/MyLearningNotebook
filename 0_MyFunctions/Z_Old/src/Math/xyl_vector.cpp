#include"xyl_vector.hpp"

xyl::coordinate::Point3::Point3(){}

xyl::coordinate::Point3::Point3(float x, float y , float z)
{
    this->point3[0] = x;
    this->point3[1] = y;
    this->point3[2] = z;
}

xyl::coordinate::Vec3::Vec3()
{
    this->point3[0]=0;
    this->point3[1]=0;
    this->point3[2]=0;
}

xyl::coordinate::Vec3::Vec3(const VEC_TYPE &v1, const VEC_TYPE &v2, const VEC_TYPE &v3)
{
    this->point3[0] = v1;
    this->point3[1] = v2;
    this->point3[2] = v3;
}

VEC_TYPE xyl::coordinate::Vec3::get_point_x() const
{
    return this->origin[0];
}

VEC_TYPE xyl::coordinate::Vec3::get_point_y() const
{
    return this->origin[1];
}

VEC_TYPE xyl::coordinate::Vec3::get_point_z() const
{
    return this->origin[2];
}

xyl::coordinate::transform::transform():Vec3(){}

