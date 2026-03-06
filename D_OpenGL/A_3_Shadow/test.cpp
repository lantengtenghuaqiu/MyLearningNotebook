#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// ===================== 配置常量（硬编码）=====================
const int SHADOW_MAP_SIZE = 1024; // 阴影贴图分辨率
const int SCREEN_W = 800;         // 窗口宽度
const int SCREEN_H = 600;         // 窗口高度

// ===================== 阴影缓冲对象 =====================
unsigned int shadowFBO;          // 帧缓冲（渲染深度用）
unsigned int shadowDepthTexture; // 深度纹理（存储阴影贴图）

// ===================== 矩阵：全部 float[16] 列主序 =====================
float lightProj[16];  // 灯光正交投影矩阵
float lightView[16];  // 灯光视角矩阵
float lightSpace[16]; // 灯光空间矩阵 = lightProj * lightView
float cameraView[16]; // 相机视角矩阵
float cameraProj[16]; // 相机透视矩阵

// ===================== 几何体VAO/VBO =====================
unsigned int planeVAO, cubeVAO;
unsigned int planeVBO, cubeVBO;

// ===================== 着色器 =====================
unsigned int depthShader; // 深度着色器（第一步渲染用）
unsigned int sceneShader; // 场景着色器（第二步渲染+阴影用）

// ===========================================================================
// 一、手写矩阵运算（替代GLM，无任何第三方库）
// ===========================================================================
// 1. 单位矩阵：初始化矩阵为默认单位阵
void matrix_identity(float *mat)
{
    memset(mat, 0, sizeof(float) * 16);
    mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

// 2. 正交投影矩阵：方向光阴影专用（平行光无透视）
void matrix_ortho(float left, float right, float bottom, float top, float near, float far, float *mat)
{
    matrix_identity(mat);
    mat[0] = 2.0f / (right - left);
    mat[5] = 2.0f / (top - bottom);
    mat[10] = -2.0f / (far - near);
    mat[12] = -(right + left) / (right - left);
    mat[13] = -(top + bottom) / (top - bottom);
    mat[14] = -(far + near) / (far - near);
}

// 3. 视角矩阵（lookAt）：相机/灯光看向目标点
void matrix_lookAt(float eyeX, float eyeY, float eyeZ,
                   float centerX, float centerY, float centerZ,
                   float upX, float upY, float upZ, float *mat)
{
    // 计算前向/右向/上向向量
    float front[3] = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    float len = sqrt(front[0] * front[0] + front[1] * front[1] + front[2] * front[2]);
    front[0] /= len;
    front[1] /= len;
    front[2] /= len;

    float right[3] = {front[1] * upZ - front[2] * upY, front[2] * upX - front[0] * upZ, front[0] * upY - front[1] * upX};
    len = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
    right[0] /= len;
    right[1] /= len;
    right[2] /= len;

    float up[3] = {right[1] * front[2] - right[2] * front[1], right[2] * front[0] - right[0] * front[2], right[0] * front[1] - right[1] * front[0]};

    // 构造矩阵
    matrix_identity(mat);
    mat[0] = right[0];
    mat[1] = up[0];
    mat[2] = -front[0];
    mat[4] = right[1];
    mat[5] = up[1];
    mat[6] = -front[1];
    mat[8] = right[2];
    mat[9] = up[2];
    mat[10] = -front[2];
    mat[12] = -right[0] * eyeX - right[1] * eyeY - right[2] * eyeZ;
    mat[13] = -up[0] * eyeX - up[1] * eyeY - up[2] * eyeZ;
    mat[14] = front[0] * eyeX + front[1] * eyeY + front[2] * eyeZ;
}

// 4. 矩阵乘法：C = A * B（核心变换运算）
void matrix_mul(float *A, float *B, float *C)
{
    float res[16] = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res[i * 4 + j] =
                A[i * 4 + 0] * B[0 + j] +
                A[i * 4 + 1] * B[4 + j] +
                A[i * 4 + 2] * B[8 + j] +
                A[i * 4 + 3] * B[12 + j];
        }
    }
    memcpy(C, res, sizeof(float) * 16);
}

// ===========================================================================
// 二、着色器工具函数
// ===========================================================================
unsigned int create_shader(const char *vs_code, const char *fs_code)
{
    // 编译顶点着色器
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_code, NULL);
    glCompileShader(vs);

    // 编译片段着色器
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_code, NULL);
    glCompileShader(fs);

    // 链接程序
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // 清理
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// ===========================================================================
// 三、创建阴影贴图（核心：帧缓冲+深度纹理）
// ===========================================================================
void init_shadow_map()
{
    // 1. 创建帧缓冲FBO
    glGenFramebuffers(1, &shadowFBO);

    // 2. 创建深度纹理（存储阴影的核心）
    glGenTextures(1, &shadowDepthTexture);
    glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // 纹理参数：最近滤波+边界 clamping（避免阴影边缘失真）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    // 3. 绑定深度纹理到FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthTexture, 0);
    // 关闭颜色缓冲：只渲染深度，不渲染颜色
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // 检查FBO是否正常
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("阴影帧缓冲创建失败！\n");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ===========================================================================
// 四、初始化场景几何体：地面 + 2个立方体
// ===========================================================================
void init_scene_geometry()
{
    // ---------------- 1. 地面平面（大平面，承载阴影）----------------
    float plane_vertices[] = {
        // 位置 xyz
        -15.0f, 0.0f, -15.0f,
        15.0f, 0.0f, -15.0f,
        15.0f, 0.0f, 15.0f,
        -15.0f, 0.0f, -15.0f,
        15.0f, 0.0f, 15.0f,
        -15.0f, 0.0f, 15.0f};

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // ---------------- 2. 立方体（完整立方体，两个实例不同位置）----------------
    float cube_vertices[] = {
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

// ===========================================================================
// 五、着色器源码（硬编码）
// ===========================================================================
// 1. 深度着色器：第一步渲染用，只输出灯光空间的位置，写入深度
const char *DEPTH_VS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;
void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

const char *DEPTH_FS = R"(
#version 330 core
// 空片段着色器：只需要深度值，不需要颜色
void main() { }
)";

// 2. 场景着色器：第二步渲染，计算阴影+显示颜色
const char *SCENE_VS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 FragPos;
out vec4 FragPosLightSpace;
void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
)";

const char *SCENE_FS = R"(
#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

// 阴影计算函数
float calculate_shadow(vec4 fragPosLightSpace) {
    // 转换到[0,1]纹理坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // 采样阴影贴图的深度
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 当前像素的深度
    float currentDepth = projCoords.z;

    // 阴影偏移：解决阴影失真（条纹）
    float bias = 0.005;
    // 对比深度：当前深度 > 贴图深度 = 阴影
    return currentDepth - bias > closestDepth ? 0.3 : 1.0;
}

void main() {
    float shadow = calculate_shadow(FragPosLightSpace);
    // 最终颜色：基础颜色 * 阴影系数（阴影区域变暗）
    FragColor = vec4(vec3(0.8f) * shadow, 1.0f);
}
)";

// ===========================================================================
// 六、渲染函数（核心两步渲染逻辑）
// ===========================================================================
void render()
{
}

// ===========================================================================
// 七、主函数
// ===========================================================================
int main()
{
    // 初始化窗口
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Shadow Mapping (No GLM)", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    // OpenGL 基础配置
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // 初始化
    init_shadow_map();     // 创建阴影贴图
    init_scene_geometry(); // 创建地面+立方体
    depthShader = create_shader(DEPTH_VS, DEPTH_FS);
    sceneShader = create_shader(SCENE_VS, SCENE_FS);

    // ===================== 硬编码所有矩阵 =====================
    // 1. 灯光矩阵（方向光）
    matrix_ortho(-15, 15, -15, 15, 1.0f, 50.0f, lightProj);
    matrix_lookAt(10.0f, 12.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, lightView);
    matrix_mul(lightProj, lightView, lightSpace);

    // 2. 相机矩阵
    matrix_lookAt(3.0f, 6.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, cameraView);
    matrix_identity(cameraProj);
    float aspect = (float)SCREEN_W / SCREEN_H;
    float fov = 0.8f;
    cameraProj[0] = 1.0f / (aspect * tan(fov / 2));
    cameraProj[5] = 1.0f / tan(fov / 2);
    cameraProj[10] = -100.0f / 99.9f;
    cameraProj[11] = -1.0f;
    cameraProj[14] = -10.0f / 99.9f;

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // ===================== 第一步：渲染深度到阴影贴图 =====================
        glUseProgram(depthShader);
        glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceMatrix"), 1, GL_FALSE, lightSpace);

        // 绑定阴影FBO，渲染到深度纹理
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

        // 渲染地面
        float model_mat[16];
        matrix_identity(model_mat);
        glUniformMatrix4fv(glGetUniformLocation(depthShader, "model"), 1, GL_FALSE, model_mat);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // 渲染立方体1（位置：-2, 1, 0）
        glBindVertexArray(cubeVAO);

        matrix_identity(model_mat);
        model_mat[12] = -2.0f;
        model_mat[13] = 1.0f; // 平移
        glUniformMatrix4fv(glGetUniformLocation(depthShader, "model"), 1, GL_FALSE, model_mat);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        // 渲染立方体2（位置：2, 1, 2）
        matrix_identity(model_mat);
        model_mat[12] = 2.0f;
        model_mat[13] = 1.0f;
        model_mat[14] = 2.0f;
        glUniformMatrix4fv(glGetUniformLocation(depthShader, "model"), 1, GL_FALSE, model_mat);
        glDrawArrays(GL_TRIANGLES, 0, 24);
        
        glBindVertexArray(0);

        // ===================== 第二步：渲染带阴影的场景 =====================
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCREEN_W, SCREEN_H);
        glUseProgram(sceneShader);

        // 传入矩阵
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "lightSpaceMatrix"), 1, GL_FALSE, lightSpace);
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "view"), 1, GL_FALSE, cameraView);
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "proj"), 1, GL_FALSE, cameraProj);
        // 绑定阴影贴图到纹理单元1
        glUniform1i(glGetUniformLocation(sceneShader, "shadowMap"), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);

        // 渲染地面
        matrix_identity(model_mat);
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "model"), 1, GL_FALSE, model_mat);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 渲染立方体1
        matrix_identity(model_mat);
        model_mat[12] = -2.0f;
        model_mat[13] = 1.0f;
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "model"), 1, GL_FALSE, model_mat);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        // 渲染立方体2
        matrix_identity(model_mat);
        model_mat[12] = 2.0f;
        model_mat[13] = 1.0f;
        model_mat[14] = 2.0f;
        glUniformMatrix4fv(glGetUniformLocation(sceneShader, "model"), 1, GL_FALSE, model_mat);
        glDrawArrays(GL_TRIANGLES, 0, 24);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}