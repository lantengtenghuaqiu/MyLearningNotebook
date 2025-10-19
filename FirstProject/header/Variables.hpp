#ifndef Variables
#define Variables

#include <iostream>
#include <cmath>

using uint = unsigned int;

struct Color_255RGB
{
    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;
        Color_255RGB(){};
        Color_255RGB(unsigned char _r , unsigned char _g , unsigned char _b);

};

struct UV
{
    public:
        float u = 0;
        float v = 0;
        UV();
        UV(int u , int v);
    
};


struct IMAGEPROPERY{ 
     public:
        UV uv;
        
        IMAGEPROPERY(){};
        IMAGEPROPERY(int width, int height);
};
using Img = IMAGEPROPERY;

class FX_LINEARFUNCTION_INT{
    FX_LINEARFUNCTION_INT();
    public:
        FX_LINEARFUNCTION_INT(float k , float a , float b,int width ,int height);
        IMAGEPROPERY *img;
        int fx = 0;
        int f = 0;
        float k = 0;
        float a = 0;
        float b = 0;
        float x = 0;
};

namespace VECTOR
{
    enum class VEC_CHECKER : char
    {
        e_vec = 1,
        e_col = 2
    };
    class vec3
    {
        private:
            double e[3];
        public:
            const double *v = e;
            vec3() : e{0,0,0}{}

            vec3(double e0 , double e1 , double e2):e{e0,e1,e2}{}

            const double x() const{return e[0];}//使返回的值不能被修改
            const double y() const{return e[1];}
            const double z() const{return e[2];}


            //使用-号时,如果现在有vec3 direction变量,-direction可以使其返回相反数,但因为是const不会改变e所以只能接收.
            vec3 operator-() const 
            {
                return vec3(-e[0],-e[1],-e[2]);
            }

            double operator[](int i) const 
            {
                if(i<0 || i>3){
                    throw std::out_of_range("vec3 : the index out of the range");
                }
                return e[i];
            }
            
            double& operator[](int i) 
            {
                if(i<0 || i>3){
                    throw std::out_of_range("vec3 : the index out of the range");
                }
                return e[i];
            }

            vec3& operator+=(const vec3&v)
            {
                e[0] += v.x();
                e[1] += v.y();
                e[2] += v.z();
                return *this;
            }

            vec3& operator*=(double num)
            {
                e[0] *= num;
                e[1] *= num;
                e[1] *= num;
                return *this;
            }

            vec3& operator/=(double num)
            {
                e[0] /= num;
                e[1] /= num;
                e[2] /= num;
                return *this;
            }

            double length_squared() const
            {
                return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
            }

            double length() const
            {
                return std::sqrt(length_squared());
            }
    };  


    inline std::ostream& operator<<(std::ostream & out , const vec3& v)
    {
        return out<<v.x()<<' '<<v.y()<<' '<<v.z();
    }

    inline vec3 operator+(const vec3& v1 , const vec3& v2)
    {
        return vec3(v1.x() + v2.x() , v1.y() + v2.y() , v1.z() + v2.z());
    }

    inline vec3 operator-(const vec3& v1 , const vec3& v2)
    {
        return vec3(v1.x() - v2.x() , v1.y() - v2.y() , v1.z() - v2.z());
    }

    inline vec3 operator*(const vec3& v1 , const vec3& v2)
    {
        return vec3(v1.x() * v2.x() , v1.y() * v2.y() , v1.z() * v2.z());
    }

    inline vec3 operator*(const vec3& v1 , double num)
    {
        return vec3(v1.x() * num , v1.y() * num , v1.z() * num);
    }

    inline vec3 operator/(const vec3& v1 , double num)
    {
        if(num == 0){
            num = 0.000001;
        }
        double div = 1/num;
        return vec3(v1.x() * div , v1.y() * div , v1.z() * div);
    }

    inline double dot(const vec3& v1, const vec3& v2)
    {
        return ((v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z()));
    }

    inline vec3 cross(const vec3& v1 , const vec3& v2)
    {
        return vec3(v1.y() * v2.z() - v1.z() * v2.y() , v1.y() * v2.x() - v1.x() * v2.y() , v1.x() * v2.y() - v1.y() * v1.x());
    }

    inline vec3 normalize(const vec3& v1 , const VEC_CHECKER vecchacker)
    {
        if(vecchacker == VEC_CHECKER::e_vec)
        {
            return v1/v1.length();
        }
        else if(vecchacker == VEC_CHECKER::e_col)
        {
            return vec3(0,0,0);
        }
        return v1;
    }
    inline vec3 color(const vec3& v )
    {
        return vec3(v.x() * 255 , v.y() * 255 , v.z() * 255);
    }
}
namespace VEC = VECTOR;
using point = VEC::vec3;
using color = VEC::vec3;





#endif//Variables