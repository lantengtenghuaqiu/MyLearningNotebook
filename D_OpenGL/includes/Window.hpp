#ifndef CALLBACK
#define CALLBACK
#include "./glad/glad.h"
#include "./GLFW/glfw3.h"
#include "./BasicIncludes.hpp"
#include "../includes/xyl_tools.hpp"
#include "../includes/SceneObject.hpp"
#include "./GlobalConfig.hpp"

#define ShaderChecker                               \
    printf("Wrong Compile (%d)\n", checker);        \
    char info[512];                                 \
    glGetShaderInfoLog(shader_id, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                   \
    {                                               \
        printf("%c", info[i]);                      \
    }                                               \
    printf("\n");

#define LinkChecker                                      \
    printf("Wrong Link (%d):\n", checker);               \
    char info[512];                                      \
    glGetShaderInfoLog(shader_program, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                        \
    {                                                    \
        printf("%c", info[i]);                           \
    }                                                    \
    printf("\n");

#ifdef __APPLE__
#define BASEPATH "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week"
#else
#define BASEPATH "G:/user/desktop/C++/GraphicLearning"
#endif

static int width = 1080;
static int height = 720;
static int frameBufferWidth = 0;
static int frameBufferHeight = 0;

class GLFW
{
public:
    GLFWwindow *window;
    GLFW() {}
    int InitGlfw(const int &width, const int &height, const char *title, GLFWmonitor *glfwmonitor = nullptr, GLFWwindow *share = nullptr)
    {
        printf("Init Glfw\n");
        if (glfwInit() != FAIL)
        {
            printf("Init Glfw Succeed\n");

            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
            glfwWindowHint(GLFW_DECORATED, GL_TRUE);
            glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_DEPTH_BITS, 32);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);

            glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            this->window = glfwCreateWindow(width, height, title, glfwmonitor, share);

            if (this->window != NULL)
            {
                glfwMakeContextCurrent(this->window);
            }
            else
            {
                printf("window is NULL!!!\n");
                glfwTerminate();
            }
        }
        else
        {
            printf("glfwInit with something wrong,initiation is failed!!!\n");
            glfwTerminate();
        }

        return SUCCESS;
    }
    ~GLFW()
    {
        glfwTerminate();
    }
};

typedef struct BufferBinderAttributes
{
    int BUFFERTYPE = GL_ARRAY_BUFFER;
    int DRAWYPE = GL_STATIC_DRAW;

    int Location = -1;
    int DataSize = 3;
    int DATATYPE = GL_FLOAT;

    unsigned char NORMILAZED = GL_FALSE;
    int Stride = 0;
    int StartPointer = 0;
    bool AttributeChecker() const
    {
        if (this->DRAWYPE != 0 && this->Location >= 0 && DataSize > 0)
            return true;
        else
            return false;
    }
} Binder;

class GLAD
{
public:
    int status = 0;

    GLAD() {}

    void InitGlad(GLFWwindow *window, int &frameWidth, int &frameHeight)
    {
        printf("Init Glad\n");
        this->status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (this->status == SUCCESS)
        {
            printf("Init Glad Succeed\n");
            glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
            glViewport(0, 0, frameWidth, frameHeight);
        }
        else
        {
            printf("Init the glad failed\n");
        }
    }

    void BindEBO(unsigned int &EBO, const int data_size, const unsigned int *index, int ebo_draw_type = GL_STATIC_DRAW)
    {
        printf("Bind EBO : %d\n", EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, index, ebo_draw_type);
    }

    void BindVAO(unsigned int &VAO)
    {
        printf("VAO Index : %d \n", VAO);
        glBindVertexArray(VAO);
    }

    void GetShadersData(Tools::TheFile *&file, const char *revalpath, char *&data)
    {
        if (file->path != NULL)
        {
            delete[] file->path;
            file->path = NULL;
        }

        if (data != NULL)
        {
            delete[] data;
            data = NULL;
        }

        const char *basePath = BASEPATH;

        file->path = new char[strlen(basePath) + strlen(revalpath) + 1];
        snprintf(file->path, strlen(basePath) + strlen(revalpath) + 1, "%s%s", basePath, revalpath);

#ifdef FILE_DEBUG
        printf("basePath size : %zu\n", strlen(basePath));
        printf("revalpath size : %zu\n", strlen(revalpath));
        printf("file->path size : %zu\n", strlen(file->path));
        for (size_t i = 0; i < strlen(basePath) + strlen(revalpath) + 1; i++)
        {
            printf("%c", file->path[i]);
        }
        printf("\n");
#endif

        file->GetContent(file->path, "rb", data);
    }

    void CompileAndAttachShader(unsigned int shader_program, unsigned int shader_id, char *&shader_data, const char *shader_type)
    {
        printf("Compile Shader %s : %d\n", shader_type, shader_id);

        glShaderSource(shader_id, 1, &shader_data, NULL);
        glCompileShader(shader_id);

        int checker;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &checker);

        if (checker == FAIL)
        {
            ShaderChecker
        }
        else
        {
            printf("Attached shader program(%d -> %d).\n", shader_id, shader_program);
            glAttachShader(shader_program, shader_id);
        }
        printf("\n");
        if (shader_data != NULL)
        {
            delete[] shader_data;
            shader_data = NULL;
        }
    }
    inline void LinkShaderProgram(unsigned int &shader_program)
    {
        glLinkProgram(shader_program);
        int checker;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &checker);
        if (checker == FAIL)
        {
            LinkChecker
        }
    }

    inline void DetachAndDeleteShaders(Shader shader)
    {
        // Detch And Delete Shaders
        glDetachShader(shader.Program, shader.Vertex);
        glDetachShader(shader.Program, shader.Fragment);
        glDeleteShader(shader.Vertex);
        glDeleteShader(shader.Fragment);
    }

    void UseShaderProgram(unsigned int shader_program)
    {
        if (shader_program != FAIL)
        {
            printf("Use the shader program : %d\n", shader_program);
            glUseProgram(shader_program);
        }
        else
            printf("Shader program has something wrong\n");
    }

    void DeleteShaders(unsigned int shader)
    {
        glDeleteShader(shader);
    }
    int GetUniformLoaction(const unsigned int sahder_program, const char *keyword)
    {
        return glGetUniformLocation(sahder_program, keyword);
    }
};

void BindTransformUniformBufferObject(ObjectID *OID, Camera *camera)
{
    glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[OID->GetCID_UBO('r')]);
    glBufferData(GL_UNIFORM_BUFFER, MAT4_SIZE * 4, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, MAT4_SIZE, camera->CameraProjection);
    glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE, MAT4_SIZE, camera->TranslateMatrix);
    glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 2, MAT4_SIZE, camera->RotationMatrix);
    glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 3, MAT4_SIZE, camera->CameraSpace);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, OID->UBO[OID->GetCID_UBO('w')]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T, size_t N1, size_t N2, size_t N3>
void BindSceneObject(ObjectID *OID, T (&mesh)[N1], T (&normal)[N2], T (&uv)[N3])
{
    size_t float_size = sizeof(float);
    glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
    glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
    glBufferData(GL_ARRAY_BUFFER, (N1 + N2 + N3) * float_size, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, N1 * float_size, mesh);
    glBufferSubData(GL_ARRAY_BUFFER, N1 * float_size, N2 * float_size, normal);
    glBufferSubData(GL_ARRAY_BUFFER, (N1 + N2) * float_size, N3 * float_size, uv);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)(N1 * float_size));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)((N1 + N2) * float_size));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BindCubeMapTexture(ObjectID *OID, Tools::ImageManager *&image)
{
    if (image->path != NULL)
    {
        delete[] image->path;
        image->path = NULL;
    }
#ifdef __APPLE__
    const char *base_path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Textures/skybox";
    size_t base_path_size = strlen(base_path);
    image->path = new char[base_path_size + 1];
    strcpy(image->path, base_path);
#else
    const char *base_path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\skybox";
    size_t base_path_size = strlen(base_path);
    image->path = new char[base_path_size + 1];
    strcpy(image->path, base_path);
#endif

    const char *faces[] =
        {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"};

    glBindTexture(GL_TEXTURE_CUBE_MAP, OID->TEX[OID->GetCID_TEX('w')]);
    // printf(" Tex : %d\n", OID->TEX[OID->GetCID_TEX('r')]);
    for (int i = 0; i < 6; i++)
    {
        size_t reval_path = strlen(faces[i]);
        size_t whole_path_size = base_path_size + reval_path + 2;
        char *path = new char[whole_path_size];
        for (size_t j = 0; j < whole_path_size; j++)
        {
            if (j < base_path_size)
                path[j] = image->path[j];
            else if (j == base_path_size)
                path[j] = '/';
            else
                path[j] = faces[i][(j - 1) - base_path_size];
#ifdef FILE_DEBUG
            printf("%c", path[j]);
#endif
        }
#ifdef FILE_DEBUG
        printf("\n");
#endif

        path[whole_path_size] = '\0';

        // stbi_set_flip_vertically_on_load(1);
        image->data = stbi_load(path, &image->width, &image->height, &image->channels, 0);
        if (image->data != NULL)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
            stbi_image_free(image->data);
        }
        else
        {
            printf("Wrong image data");
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // stbi_set_flip_vertically_on_load(0);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void ReadVertexData(Vertices &vertex, float *vertices, unsigned long long size)
{
    vertex.vertices = vertices;
    vertex.size = size;
    if (vertex.vertices != NULL)
    {
        printf("Vertex Size : %llu\n", vertex.size);
    }
    else
        printf("Vertex is Null\n");
}

bool Init_GLFW_GLAD(GLFW &glfw, GLAD &glad)
{
    if (glfw.InitGlfw(width, height, "Shadow", nullptr, nullptr) == SUCCESS)
    {
        glad.InitGlad(glfw.window, frameBufferWidth, frameBufferHeight);
        return true;
    }
    printf("Init Glfw with something wrong!!!\n");
    return false;
}

void BindVBO(ObjectID &attri, const int &currentIndex, Vertices &vertex)
{
    printf("Bind VBO : %d\n", attri.VBO[currentIndex]);

    glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[currentIndex]);

    glBufferData(GL_ARRAY_BUFFER, vertex.size, vertex.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SetTextures(ObjectID *attri, TexAttri *texAttri, Shader shaders)
{
    unsigned char *image_date;
    const char *path;

    // Texture Albedo-----------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, attri->TEX[0]);
    printf("Bind Container Albedo TEX : %d\n", attri->TEX[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef __APPLE__
    path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Textures/container2.png";
#else
    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\container2.png";
#endif
    image_date = stbi_load(path, &texAttri[0].width, &texAttri[0].height, &texAttri[0].nChanel, 4);
    printf("Texture Container Albedo width : %d , height : %d , nChanel : %d\n", texAttri[0].width, texAttri[0].height, texAttri[0].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[0].width, texAttri[0].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);

    texAttri[0].textureLocation = glGetUniformLocation(shaders.Program, "_Conatiner");
    printf("Texture Container Albedo Id : %d\n", texAttri[0].textureLocation);

    // Texture Specular---------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, attri->TEX[1]);
    printf("Bind Conatiner Specular TEX : %d\n", attri->TEX[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef __APPLE__
    path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Textures/container2_specular.png";
#else
    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\container2_specular.png";
#endif

    image_date = stbi_load(path, &texAttri[1].width, &texAttri[1].height, &texAttri[1].nChanel, 4);
    printf("Texture Conatiner Specular width : %d , height : %d , nChanel : %d\n", texAttri[1].width, texAttri[1].height, texAttri[1].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[1].width, texAttri[1].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);

    texAttri[1].textureLocation = glGetUniformLocation(shaders.Program, "_ConatinerSpecular");
    printf("Texture Conatiner Specular Id : %d\n", texAttri[1].textureLocation);
}

void SetBindLogoMesh(ObjectID *attri, TexAttri *texAttri, Shader shaders)
{
    unsigned char *image_date;
    const char *path;
    // Logo texture:
    glBindTexture(GL_TEXTURE_2D, attri->TEX[2]);
    printf("\nBind Logo Mesh TEX : %d\n", attri->TEX[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef __APPLE__
    path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Textures/maizhimeng.png";

#else
    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\maizhimeng.png";
#endif
    image_date = stbi_load(path, &texAttri[2].width, &texAttri[2].height, &texAttri[2].nChanel, 4);
    printf("Texture Logo Mesh width : %d , height : %d , nChanel : %d\n", texAttri[2].width, texAttri[2].height, texAttri[2].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[2].width, texAttri[2].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);

    texAttri[2].textureLocation = glGetUniformLocation(shaders.Program, "_maizhimeng");
    printf("Texture Logo Mesh Id : %d\n", texAttri[2].textureLocation);
}
#endif
