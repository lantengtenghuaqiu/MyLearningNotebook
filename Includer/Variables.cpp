#include "Variables.hpp"

vec3::vec3() : e{0, 0, 0} 
{
    this->checker=VEC_CHECKER::e_vec;
}

vec3::vec3(const double& e0, const double& e1,const double& e2)
{
    this->e[0] = e0;
    this->e[1] = e1;
    this->e[2] = e2;
    this->checker = VEC_CHECKER::e_vec;
}

vec3::vec3(const double& e0, const double& e1,const double& e2 ,const VEC_CHECKER& check)
{
    this->e[0] = e0;
    this->e[1] = e1;
    this->e[2] = e2;
    this->checker = check;
}

double vec3::x() const { return e[0]; }
double vec3::y() const { return e[1]; }
double vec3::z() const  { return e[2]; }

VEC_CHECKER vec3::GetChecker() const
{
    return this->checker;
}

vec3 vec3::operator-()
{
    return vec3(-this->e[0], -this->e[1], -this->e[2]);
}

double vec3::operator[](int i) const
{
    if (i < 0 || i > 3)
    {
        throw std::out_of_range("vec3 : the index out of the range");
    }
    return e[i];
}

double &vec3::operator[](int i)
{
    if (i < 0 || i > 3)
    {
        throw std::out_of_range("vec3 : the index out of the range");
    }
    return e[i];
}

vec3 &vec3::operator+=(const vec3 &v)
{
    e[0] += v.x();
    e[1] += v.y();
    e[2] += v.z();
    return *this;
}

vec3 &vec3::operator()(const double &e0, const double &e1, const double &e2)
{
    this->e[0] = e0;
    this->e[1] = e1;
    this->e[2] = e2;
    return *this;
}

vec3 &vec3::operator*=(double num)
{
    e[0] *= num;
    e[1] *= num;
    e[1] *= num;
    return *this;
}

vec3 &vec3::operator/=(double num)
{
    e[0] /= num;
    e[1] /= num;
    e[2] /= num;
    return *this;
}

double vec3::DotVec3() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

double vec3::length() const
{
    return std::sqrt(DotVec3());
}

vec3 vec3::random() 
{
    return vec3(xyl::math::random_double_normalized(), xyl::math::random_double_normalized(), xyl::math::random_double_normalized());
}

vec3 RandomUnitVector()
{
    vec3 point(0,0,0);
    double lensq;
    while(true)
    {
        xyl::WHILE_PROPECTOR++;

         point = vec3::random<double>(-1.0,1.0);
         lensq = point.DotVec3();

         if( 1e-160 < lensq && lensq <= 1.0)
         {
            return point / sqrt(lensq);
         }

    }
    xyl::WHILE_PROPECTOR = 0;
    return vec3();
}

vec3 RandomOnHemisphere(const vec3 &normal)
{
    vec3 on_unit_shpere = RandomUnitVector();
    if(dot(on_unit_shpere , normal)>0.0)
    {
        return on_unit_shpere;
    }
    else
    {
        return -on_unit_shpere;
    }

}

std::ostream & operator<<(std::ostream & out, const vec3 & v)
{
    // 通过将传入的out进行类型转换,如果转换成功则为文件输出流,否则为标准输出流
    std::ofstream *ofs = dynamic_cast<std::ofstream *>(&out);

    if (ofs != nullptr)
    {
        return out << (unsigned char)(int)v.x() << (unsigned char)(int)v.y() << (unsigned char)(int)v.z();
        // return
        // unsigned int r = v.x();
        // unsigned int g =
        // out.write((const char *)(&v.x()), 1);
        // out.write((const char *)(&v.y()), 1);
        // out.write((const char *)(&v.z()), 1);
        // return out;
    }

    return out<<v.x()<<" "<<v.y()<<" "<<v.z();
}

vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

vec3 operator*(const vec3 &v1, double num)
{
    return vec3(v1.x() * num, v1.y() * num, v1.z() * num);
}

vec3 operator*(const double num, const vec3 &v)
{
    return vec3(v.x() * num, v.y() * num, v.z() * num);
}

vec3 operator/(const vec3 &v1, double num)
{
    if (num == 0)
    {
        num = 0.000001;
    }
    double div = 1 / num;
    return vec3(v1.x() * div, v1.y() * div, v1.z() * div);
}

double dot(const vec3 &v1, const vec3 &v2)
{
    return ((v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z()));
}

vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.y() * v2.z() - v1.z() * v2.y(),
                v1.y() * v2.x() - v1.x() * v2.y(),
                v1.x() * v2.y() - v1.y() * v1.x());
}

vec3 normalize(const vec3 &v1, const VEC_CHECKER vecchacker)
{
    vec3 _v1 = v1;
    if (vecchacker == VEC_CHECKER::e_vec)
    {
        return _v1 / _v1.length();
    }
    else if (vecchacker == VEC_CHECKER::e_col)
    {
        return vec3(255, 100, 155);
    }
    return vec3(0, 0, 0);
}

vec3 color(const vec3 &v)
{
    return vec3(v.x() * 255, v.y() * 255, v.z() * 255);
}
