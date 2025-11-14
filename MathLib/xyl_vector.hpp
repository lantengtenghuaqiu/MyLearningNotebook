#ifndef XYLVECTOR_H
#define XYLVECTOR_H

// #define XYL_FLOAT
#include"xyl_return_def_type.hpp"

namespace xyl
{
    struct point
    {
        protected:
            VEC_TYPE origin[VEC];
        public:
            VEC_TYPE get_point_x()const;

    };


    namespace _vector
    {
        struct vec : public point
        {
            private:
            /* data */
            //矢量或者向量,在实际上没有具体的位置信息(没有绝对位置),而是相对位置信息.它包含一个位置到另一个位置移动的方向,和程度(距离/大小)
            //对于位移:"向前迈进3步",这里指的就是相对位移
                VEC_TYPE magnitude;
                VEC_TYPE direction[VEC];
                VEC_TYPE tail[VEC];
                VEC_TYPE head[VEC];
            public:
                
                vec(){}
    
                vec(const VEC_TYPE& v1 , const VEC_TYPE& v2 , const VEC_TYPE& v3)
                {
                    this->origin[0] = v1;
                    this->origin[1] = v2;
                    if(VEC==3)
                    {
                        this->origin[2] = v3;
                    }
                }
        }; 
    }


    
} // 

#endif