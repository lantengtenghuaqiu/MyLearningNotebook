#ifndef CAMERA
#define CAMERA

namespace xyl
{
    typedef class CameraAttribute
    {
    private:
        CameraAttribute() {}

    public:
        float near = 0.1f;
        float far = 100.0f;
        float width = 300.0f;
        float height = 300.0f;
        CameraAttribute(float n, float f, float w, float h) : near(n), far(f), width(w), height(h) {}

    } Camera;
}

#endif