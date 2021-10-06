/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-01 22:47:04
 * @LastEditors: like
 * @LastEditTime: 2021-10-03 20:47:07
 */
#ifndef SYSTEM_FORM_OPENGL_HPP
#define SYSTEM_FORM_OPENGL_HPP

/**
 *  opengl采用右手准则 
 *          +Y
 *          ^
 *          |    /
 *          |  /                    |---------------|
 *          | /                     |               |
 * ---------------------> +X        |    ViewPort   | Height
 *         /|                       |               |
 *       /  |                       *---------------|
 *     /    |                     (x,y)     Width
 *    /     |
 *  +Z    
 *  
 *                    
 *  \             /---------}
 *   \           /          |
 *    \         /           |
 *     \-------/ (0,0,0)    |far(100)
 *      \     /             |
 *       \   /              | 
 *        \ /---} near(1)   }
 *         * 
 *       camera(0,0,50)
 * 
 * */

// #include <glad/glad.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define GL_GET_STRING(flag) (const char*)glGetString(flag)
#define DEGREE_TO_RADIAN(degree) (float)(M_PI  * degree / 180)

namespace System::Form::GL
{
    typedef struct _Point3D
    {
        float x;
        float y;
        float z;
    }Point3D, *pPoint3D;
    typedef Point3D Verter3D;
    
    class GLWidgetInfo;
    class Viewport;
    class Camera;
    class GLWidget;
    class GlInfo;
}

class System::Form::GL::GLWidgetInfo
{
protected:
    GLFWwindow* window;
public:
    int width;
    int height;
    const char* title;
    GLWidgetInfo(int w, int h, const char* windowTitle) : width(w), height(h), title(windowTitle)
    {
        window = glfwCreateWindow(width, height, title, NULL, NULL);
#ifdef GL_DEBUG
        printf("GLWidgetInfo : %d, %d\n", width, height);
#endif
    }
    ~GLWidgetInfo()
    {
        glfwDestroyWindow(window);
    }
};

class System::Form::GL::Viewport
{
public:
    int left;
    int bottom;
    int width;
    int height;
    Viewport(){}
    Viewport(int l, int b, int w, int h) : left(l), bottom(b), width(w), height(h){}
    static inline void FromRectangle(int top, int left, int width, int height , Viewport& view)
    {
        view.left   = left;
        view.bottom = top - height;
        view.width  = width;
        view.height = height;
    }
    void UpdateViewport()
    {
        glViewport(left, bottom, width, height);
#ifdef GL_DEBUG
        printf("UpdateViewport x, y, width, height : %d, %d, %d, %d\n", left, bottom, width, height);
#endif
    }
};

class System::Form::GL::Camera
{
public:
    Point3D location;
    Point3D focus;
    Verter3D normalVector;  /* 相机视线法向量 */
    float halfRadian;   /* 弧度，视场角的一半 */
    float aspect;       /* viewport宽高比 */
    float nearLen; 
    float farLen;
    Camera(){}
    inline void UpdatePerspective()
    {
        mat4x4 projection;
        glMatrixMode(GL_PROJECTION);
        mat4x4_perspective(projection, 2 * halfRadian, aspect, nearLen, farLen);
        glLoadMatrixf((const GLfloat*)projection);
#ifdef GL_DEBUG
        printf("UpdatePerspective halfFOV, aspect, n, f : %f, %f, %f, %f\n", halfRadian, aspect, nearLen, farLen);
#endif
    }
    inline void UpdateLookAt()
    {
        mat4x4 view;
        glMatrixMode(GL_MODELVIEW);
        mat4x4_look_at(view, (float*)(&location), (float*)(&focus), (float*)(&normalVector));
        glLoadMatrixf((const GLfloat*) view);
#ifdef GL_DEBUG
        printf("UpdateLookAt location, focus, normalVector : (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n",
            location.x, location.y, location.z, focus.x, focus.y, focus.z, normalVector.x, normalVector.y, normalVector.z);
#endif
    }
    void UpdateCamera()
    {
        UpdatePerspective();
        UpdateLookAt();
    }
};

class System::Form::GL::GLWidget : protected GLWidgetInfo, public Viewport, public Camera
{ 
public:
    GLWidget(int width, int height, const char* title) : 
        GLWidgetInfo(width, height, title), Viewport(), Camera()
    {
        glfwInit();
        /* 3.3版本opengl */
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);        
        /* 用core模式, 3.0之前只有上下文模式 */
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if(NULL == (window = glfwCreateWindow(width, height, title, NULL, NULL)))
        {
            glfwTerminate();
            return;
        }
        /* 宽高比 */
        glfwSetWindowAspectRatio(window, width, height);
        /* 当前进程和Opengl绑定 */
        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);
    }
    GLWidget(int width, int height, const char* title, 
        GLFWframebuffersizefun Resized, GLFWkeyfun KeyPressed,
        GLFWmousebuttonfun MouseClick, GLFWcursorposfun MouseMove) : 
    GLWidgetInfo(width, height, title), Viewport(), Camera()
    {
        glfwInit();
        if(NULL == (window = glfwCreateWindow(width, height, title, NULL, NULL)))
        {
            glfwTerminate();
            return;
        }
        /* 宽高比 */
        glfwSetWindowAspectRatio(window, width, height);
        /* 窗体控制事件 */
        glfwSetFramebufferSizeCallback(window,  Resized);
        glfwSetKeyCallback(window,              KeyPressed);
        glfwSetMouseButtonCallback(window,      MouseClick);
        glfwSetCursorPosCallback(window,        MouseMove);
        /* 当前进程和Opengl绑定 */
        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);
    }
    ~GLWidget()
    {    
        glfwTerminate();
    }
    void UpdateOrtho()
    {
        float ver = (Viewport::height - bottom)>>1;
        float hor = (Viewport::width - left)>>1;
	    glOrtho(-1 * hor, hor, -1 * ver, ver, -1 * location.z - farLen, location.z);
    }
    virtual void DrawFrame()
    {
        
    }
    void Run()
    {
        for (;;)
        {
            DrawFrame();
            /* Swap buffers */
            glfwSwapBuffers(window);
            glfwPollEvents();

            /* Check if we are still running */
            if (glfwWindowShouldClose(window))
                break;
        }
    }
};

class System::Form::GL::GlInfo
{
public:
    typedef struct _Version
    {
        unsigned int major;
        unsigned int minor;
        unsigned int revision;
    }Version;

    static inline void GetHeaderVersion(Version& ver) 
    {
        ver = 
        {
            GLFW_VERSION_MAJOR,
            GLFW_VERSION_MINOR,
            GLFW_VERSION_REVISION
        };
    }
    static inline void GetGlfwLibVersion(Version& ver) 
    {
        glfwGetVersion((int*)(&ver.major), (int*)(&ver.minor), (int*)(&ver.revision));
    }
    static inline const char* GetGlfwDescription()
    {
        return glfwGetVersionString();
    }
    static inline const char* GetGlApiDescription()
    {
        return (const char*)glGetString(GL_VERSION);
    }
};

#endif