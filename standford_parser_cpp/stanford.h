#ifndef STANFORD
#define STANFORD


#include<jni.h>
struct stanford
{
	JNIEnv* env;
	jclass cls;
	jmethodID tag;
	jmethodID lemma;
};

typedef struct stanford SF;

#endif