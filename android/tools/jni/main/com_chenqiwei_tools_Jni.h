/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_chenqiwei_tools_Jni */

#ifndef _Included_com_chenqiwei_tools_Jni
#define _Included_com_chenqiwei_tools_Jni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_chenqiwei_tools_Jni
 * Method:    version
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_chenqiwei_tools_Jni_version
  (JNIEnv *, jclass);

/*
 * Class:     com_chenqiwei_tools_Jni
 * Method:    enable
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_enable
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     com_chenqiwei_tools_Jni
 * Method:    exec
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_exec
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_chenqiwei_tools_Jni
 * Method:    fileSync
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_chenqiwei_tools_Jni_fileSync
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
