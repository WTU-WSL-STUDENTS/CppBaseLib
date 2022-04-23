//
// Created by lk on 2022/4/8.
//

#ifndef MyApplication_JNIComplierDef_H
#define MyApplication_JNIComplierDef_H
#include <jni.h>
#include "MacroHelper.h"

#ifdef ASSERT_ENABLE
#   define VOIDRET_ASSERT(condition)    \
        do                              \
        {                               \
            if(!(condition)){return;}   \
        } while (0)
#   define VALRET_ASSERT(condition, retVal) \
        do                                  \
        {                                   \
            if(!(condition)){return retVal;}\
        }while(0)
#else
#   define VOIDRET_ASSERT(condition)
#   define VALRET_ASSERT(condition, retVal)
#endif

/* c++ log 打印 */
#define JNI_DEBUG(...)HILOG_FATAL(LOG_APP, __VA_ARGS__)

/* 最多支持 6 层 Bundle : n1_n2_n3_n4_n5_n6_ClassType */
#define DECLARE_BUNDLE_NAME_1(classType) \
    classType
#define DECLARE_BUNDLE_NAME_2(n1, classType) \
    n1##_##classType
#define _DECLARE_BUNDLE_NAME_3(n1, n2, classType) \
    n1##_##n2##_##classType
#define DECLARE_BUNDLE_NAME_3(n1, n2, classType) \
    n1##_##n2##_##classType
#define DECLARE_BUNDLE_NAME_4(n1, n2, n3, classType) \
    n1##_##n2##_##n3##_##classType
#define DECLARE_BUNDLE_NAME_5(n1, n2, n3, n4, classType) \
    n1##_##n2##_##n3##_##n4##_##classType
#define DECLARE_BUNDLE_NAME_6(n1, n2, n3, n4, n5, classType) \
    n1##_##n2##_##n3##_##n4##_##n5##_##classType
#define DECLARE_BUNDLE_NAME_7(n1, n2, n3, n4, n5, n6, classType) \
    n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##classType

#define __DECLARE_BUNDLE_NAME(size, ...)	\
	DECLARE_BUNDLE_NAME_##size(__VA_ARGS__)
#define _DECLARE_BUNDLE_NAME(size, ...) \
    __DECLARE_BUNDLE_NAME(size, __VA_ARGS__)
#define DECLARE_BUNDLE_NAME(...) \
	_DECLARE_BUNDLE_NAME(MACRO_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

#define DECLARE_BUNDLE_NAMESPACE(...) DECLARE_BUNDLE_NAME(Java,__VA_ARGS__)
#define FUNC_JNIENV env
#define FUNC_OBJECT obj
/*
 * 调用 DECLARE_CPP_FUNC_FOR_JAVA 来申明 java 的 c++ 接口时, 需要先申明宏定义 USING_BUNDLE_NAMESPACE ,写法如下:
 * #define USING_BUNDLE_NAMESPACE DECLARE_BUNDLE_NAME(com, example, jnilearn, MainAbilitySlice)
 * */
#define __DECLARE_CPP_FUNC_FOR_JAVA(namespace, returnType, func, codeblock, ...)\
    extern "C" JNIEXPORT            \
    returnType JNICALL              \
    namespace##_##func(             \
        __VA_ARGS__) {              \
            codeblock               \
        }
#define _DECLARE_CPP_FUNC_FOR_JAVA(namespace, returnType, func, codeblock, ...) \
    __DECLARE_CPP_FUNC_FOR_JAVA(namespace, returnType, func, codeblock, ##__VA_ARGS__)
#define DECLARE_CPP_FUNC_FOR_JAVA(returnType, func, codeblock, ...)\
    _DECLARE_CPP_FUNC_FOR_JAVA(USING_BUNDLE_NAMESPACE, returnType, func, codeblock,JNIEnv* FUNC_JNIENV, jobject FUNC_OBJECT, ##__VA_ARGS__)

using Boolean = jboolean;
using Byte    = jbyte;
using Char    = jchar;
using Short   = jshort;
using Int     = jint;
using Long    = jlong;
using Float   = jfloat;
using Double  = jdouble;
using String  = jstring;
/* const char* to String in java
 * 通过在头文件的开始添加宏定义 JAVA_STRING_ENCODING 修改 java string 的编码
 * "utf-8" #define JAVA_ENCODING UTF
 * */
#ifdef JAVA_STRING_UTF_ENABLE
#   define CSTRING2JSTRING(str) (FUNC_JNIENV->NewStringUTF(str))
#else
#   define CSTRING2JSTRING(str) ({              \
            size_t len = strlen(str);           \
            Char res[len];                      \
            for(size_t i = 0; i < len; i++) {   \
                res[i] = str[i];                \
            }                                   \
            (FUNC_JNIENV->NewString(res, len)); \
        })
#endif

enum class ArrayMemoryReleaseOperate
{
    DropToJava = 0, // 将数据 Commit 到 java 并释放 c++ 数组空间
    Commit,         // 将数据 Commit 到 java 但不释放 c++ 数组空间
    Abort           // 放弃修改并释放 c++ 数组空间
};
#define _ARRAY_COUNT(env, jResource)(env->GetArrayLength(jResource))
#define ARRAY_COUNT(jResource) _ARRAY_COUNT(FUNC_JNIENV, jResource)
#define DECLARE_RALL_JAVA_ARRAY_IN_CPP(elementType) \
class elementType##ArrayCWrapper{                   \
private:                            \
    JNIEnv* env;                    \
    elementType##Array jResource;   \
    elementType *p;                 \
public:                             \
    elementType##ArrayCWrapper(                                                     \
        JNIEnv* pEnv, elementType##Array pJResource, Boolean iscopy)                \
        :env(pEnv), jResource(pJResource),                                          \
        p((elementType *)pEnv->Get##elementType##ArrayElements(jResource, &iscopy)) \
    {}                                                                              \
    ~ elementType##ArrayCWrapper(){Dispose(ArrayMemoryReleaseOperate::DropToJava);} \
    void Dispose(ArrayMemoryReleaseOperate op){     \
        if(nullptr == p) return;                    \
        env->Release##elementType##ArrayElements(   \
            jResource, p, (int)op); p = nullptr;    \
    }                                               \
    int Count(){                         \
        return ARRAY_COUNT(jResource);  \
    }                                   \
    elementType & operator[](int i){\
        return p[i];                \
    }                               \
};                                  \
template<>                                                                          \
elementType##Array JavaArray::Create<elementType##Array>(JNIEnv* env, jsize len){   \
    return env->New##elementType##Array(len);                                       \
}
using BooleanArray  = jbooleanArray;
using ByteArray     = jbyteArray;
using CharArray     = jcharArray;
using ShortArray    = jshortArray;
using IntArray      = jintArray;
using LongArray     = jlongArray;
using FloatArray    = jfloatArray;
using DoubleArray   = jdoubleArray;

#define JAVA_ARRAY_SLICE_IO(elementType)    \
static void ToSpan(JNIEnv* env, elementType##Array src, jsize start, jsize len, elementType *span){ \
    env->Get##elementType##ArrayRegion(src, start, len, span);                                      \
}                                                                                                   \
static void Flush(JNIEnv* env, elementType##Array dest, jsize start, jsize len, elementType* src){  \
    env->Set##elementType##ArrayRegion(dest, start, len, src);                                      \
}
/* java 内存数组在 c++ 的通用方法 */
class JavaArray{
public:
    /* 暂停 GC 并返回固定 java 数据的指针 */
    void* LockArrayNativePtr(JNIEnv* env, jarray src){
        return env->GetPrimitiveArrayCritical(src, nullptr);
    }
    /* 恢复 GC */
    void UnlockArrayNativePtr(JNIEnv* env, jarray src, void* carraier,
        ArrayMemoryReleaseOperate op = ArrayMemoryReleaseOperate::DropToJava){
        env->ReleasePrimitiveArrayCritical(src, carraier, (jint)op);
    }
    /* 在 java 虚拟机中创建一个指定数组 */
    template<typename JArrayType>
    static JArrayType Create(JNIEnv* env, jsize len){ throw "Not support";}
    /* java 数组转数组指针, 指针的内存由用户显示释放 */
    JAVA_ARRAY_SLICE_IO(Boolean);
    JAVA_ARRAY_SLICE_IO(Byte);
    JAVA_ARRAY_SLICE_IO(Char);
    JAVA_ARRAY_SLICE_IO(Short);
    JAVA_ARRAY_SLICE_IO(Int);
    JAVA_ARRAY_SLICE_IO(Long);
    JAVA_ARRAY_SLICE_IO(Float);
    JAVA_ARRAY_SLICE_IO(Double);
};
/* 从 java 虚拟机加载 java 数组到 c++ 的 RALL 类型 wrapper
 * 支持以下类型 :
 * BooleanArrayCWrapper,ByteArrayCWrapper,  CharArrayCWrapper,
 * ShortArrayCWrapper,  IntArrayCWrapper,   LongArrayCWrapper,
 * FloatArrayCWrapper,  DoubleArrayCWrapper
 * */
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Boolean);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Byte);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Char);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Short);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Int);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Long);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Float);
DECLARE_RALL_JAVA_ARRAY_IN_CPP(Double);

#endif //MyApplication_JNIComplierDef.h_H
