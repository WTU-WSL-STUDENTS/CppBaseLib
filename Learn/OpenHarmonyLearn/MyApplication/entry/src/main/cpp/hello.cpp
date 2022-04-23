#include <jni.h>
#define JAVA_STRING_UTF_ENABLE /* set java string UTF8 */

#include "JNIComplierDef.h"
#include <string>
#include <hilog/log.h>

#define USING_BUNDLE_NAMESPACE DECLARE_BUNDLE_NAMESPACE(com,example,myapplication,slice,MainAbilitySlice)
#define _TEST_MACRO_STR(str) MARCRO_VAL_STR(str)
#define TEST_MACRO_STR(str) _TEST_MACRO_STR(str)

DECLARE_CPP_FUNC_FOR_JAVA(String, stringFromJNI,
{
    return CSTRING2JSTRING(TEST_MACRO_STR(USING_BUNDLE_NAMESPACE));
})
DECLARE_CPP_FUNC_FOR_JAVA(String, JNITypeTest,
{
    const char* format = "输入参数 : %s , %f, %lld";
    JNI_DEBUG(format, js ? "True" : "False", f1, l1);
    char buffer[1024];
    sprintf(buffer, format, js ? "True" : "False", f1, l1);
    IntArrayCWrapper pNums(FUNC_JNIENV, nums, JNI_TRUE);
    for(int i = 0; i < pNums.Count(); i++)
    {
        pNums[i] *= 100;
    }
    pNums.Dispose(ArrayMemoryReleaseOperate::DropToJava);

    /* malloc 4MB memory in java */
    IntArray intArrayInJava = JavaArray::Create <IntArray>(FUNC_JNIENV, 0x100000);
    JNI_DEBUG("malloc %d (byte) memory in java", sizeof(Int) * ARRAY_COUNT(intArrayInJava));

    /* load java memory to c++ heap,  flush to java after edit itself */
    Int* arrayInC = new Int[ARRAY_COUNT(nums)];
    JavaArray::ToSpan(FUNC_JNIENV, nums, 0, ARRAY_COUNT(nums), arrayInC);
    for(int i = 0; i < ARRAY_COUNT(nums); i++)
    {
        arrayInC[i] += i;
    }
    JavaArray::Flush(FUNC_JNIENV, nums, 0, ARRAY_COUNT(nums), arrayInC);
    delete[] arrayInC;

    return CSTRING2JSTRING(buffer);
}, Boolean js, Float f1, Long l1, IntArray nums)