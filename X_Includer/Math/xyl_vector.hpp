#ifndef XYLVECTOR_H
#define XYLVECTOR_H


#include"xyl_return_def_type.hpp"

namespace xyl
{
    namespace coordinate
    {
        struct Point3
        {
            /* data */
            VEC_TYPE point3[3];
            Point3();
            Point3(float x, float y , float z);

        };
        
        struct Vec3 : public Point3
        {
            private:
            /* data */
            //矢量或者向量,在实际上没有具体的位置信息(没有绝对位置),而是相对位置信息.它包含一个位置到另一个位置移动的方向,和程度(距离/大小)
            //对于位移:"向前迈进3步",这里指的就是相对位移
                VEC_TYPE origin[3];
                VEC_TYPE magnitude;
                VEC_TYPE direction[3];
                VEC_TYPE tail[3];
                VEC_TYPE head[3];
            public:
                Vec3();
    
                Vec3(const VEC_TYPE& v1 , const VEC_TYPE& v2 , const VEC_TYPE& v3);

                VEC_TYPE get_point_x()const;
                VEC_TYPE get_point_y()const;
                VEC_TYPE get_point_z()const;
        };
        class transform : public Vec3
        {
            public:
                Vec3 position;
                Vec3 rotation;
                Vec3 scale;

                transform();

        };


    }


    
} // 

#endif