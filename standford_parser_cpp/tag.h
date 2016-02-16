#ifndef TAG
#define TAG

#include<iostream>
#include<string>
#include<vector>
#include<jni.h>
#include<Windows.h>
#include"stanford.h"
#include"posdep.h"

class tag{
public:
	tag(){init_jvm(PS_JVM);};
	std::string tagging(std::string s);//标注返回说明中指定格式
	std::string tagging(std::string *s);//标注返回说明中指定格式
	std::vector<std::string> tokenize(std::string s);//返回切词后的单词
	std::vector<POS> posTag(std::string s);//返回词性标签
	std::vector<TD> depTag(std::string s);//返回依存关系对
	std::string lemma(std::string s, std::string t);
private:
	stanford *init_jvm(stanford &PS);
	std::string jstring2str(JNIEnv* env, jstring jstr);
	jstring str2jstring(JNIEnv* env,const char* pat);
	TD split(std::string &tdString);
	stanford PS_JVM;
};

std::string tag::tagging(std::string s){
	return jstring2str(PS_JVM.env, (jstring)PS_JVM.env->CallObjectMethod(PS_JVM.cls, PS_JVM.tag, str2jstring(PS_JVM.env, s.c_str())));
}

std::string tag::lemma(std::string s, std::string t){
	return jstring2str(PS_JVM.env, (jstring)PS_JVM.env->CallObjectMethod(PS_JVM.cls, PS_JVM.lemma, str2jstring(PS_JVM.env, s.c_str()), str2jstring(PS_JVM.env, t.c_str())));
}

std::string tag::tagging(std::string *s){
	return tagging(*s);
}

std::vector<std::string> tag::tokenize(std::string s){
	std::vector<std::string> words;
	std::string taggingResult = tagging(&s);
	std::string chunk;
	int tabpos = 0;
	while(tabpos < taggingResult.length() && taggingResult[tabpos] != '\t') tabpos++;
	chunk = taggingResult.substr(0, tabpos);
	int start = 0;
	while(start<chunk.size() - 1){
		while(start < chunk.size() && chunk[start] != '('){
			start++;
		}
		int wd_end = start;
		int pos_end = start;
		while(wd_end < chunk.size() && chunk[wd_end] != ' ') wd_end++;
		while(pos_end < chunk.size() && chunk[pos_end] != ')') pos_end++;

		words.push_back(chunk.substr(start+1, wd_end-start-1));
		
		start = pos_end+1;
	}
	return words;
}

std::vector<POS> tag::posTag(std::string s){
	std::vector<POS> pos;
	std::string taggingResult = tagging(&s);
	std::string chunk;
	int tabpos = 0;
	while(tabpos < taggingResult.length() && taggingResult[tabpos] != '\t') tabpos++;
	chunk = taggingResult.substr(0, tabpos);
	int start = 0;
	int position = 0;
	while(start<chunk.size() - 1){
		while(start < chunk.size() && chunk[start] != '('){
			start++;
		}
		int wd_end = start;
		int pos_end = start;
		while(wd_end < chunk.size() && chunk[wd_end] != ' ') wd_end++;
		while(pos_end < chunk.size() && chunk[pos_end] != ')') pos_end++;

		position++;

		pos.push_back(POS(position, chunk.substr(start+1, wd_end-start-1), chunk.substr(wd_end+1, pos_end-wd_end-1)));
		
		start = pos_end+1;
	}
	return pos;
}

std::vector<TD> tag::depTag(std::string s){
	std::vector<TD> tdl;
	std::string tdList;
	std::string s_tagged = tagging(s);
	int tabpos = 0;
	while(tabpos < s_tagged.length() && s_tagged[tabpos] != '\t') tabpos++;
	tdList = s_tagged.substr(tabpos+2, s_tagged.length()-tabpos-3);
	int start = 0;
	int end = 0;
	while(start < tdList.length()){
		end = start;
		while(end < tdList.length() && tdList[end] != ')') end++;
		std::string td_string = tdList.substr(start, end-start+1);
		tdl.push_back(this->split(td_string));
		start = end +3;
	}
	return tdl;
}

stanford *tag::init_jvm(stanford &PS){
	/*jvm init*/
	JavaVMOption options[1];
	JNIEnv* env;
	JavaVM* jvm;
	JavaVMInitArgs vm_args;
	jclass cls;//cls_parser;
	//jmethodID mid;
	jobject jobj;//jobj_parser;
	options[0].optionString = "-Djava.class.path=C:\\dym\\my\\F\\论文\\词性标注依存标注";
	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = 1;
	vm_args.options = options;
	
	
	HINSTANCE hinstLib = LoadLibrary(TEXT("C:\\Program Files\\Java\\jdk1.8.0_60\\jre\\bin\\server\\jvm.dll"));
	typedef jint (JNICALL *PtrCreateJavaVM)(JavaVM **, void **, void *);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(hinstLib,"JNI_CreateJavaVM");
	jint res = ptrCreateJavaVM(&jvm, (void**)&env, &vm_args);

	//int res = JNI_CreateJavaVM(&jvm,(void **)&env, &vm_args);
	std::cout<<res<< std::endl;
	if(res != JNI_ERR)
	{
		cls = env->FindClass("NNDepParser");
		std::cout<<res<< std::endl;
		if(cls)
		{
			jobj = env->AllocObject(cls);
			if(jobj)
			{
				PS.env = env;
				PS.cls = cls;
				PS.tag = env->GetStaticMethodID(cls, "parse", "(Ljava/lang/String;)Ljava/lang/String;");
				PS.lemma = env->GetStaticMethodID(cls, "lemma", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			}
			else
			{
				std::cout << "jvm jobj init error" << std::endl;
				exit(0);
			}
		}
		else
		{
			std::cout << "jvm cls init error" << std::endl;
			exit(0);
		}
	}
	else
	{
		std::cout << "jvm init error" << std::endl;
		exit(0);
	}
}

std::string tag::jstring2str(JNIEnv* env, jstring jstr){
	char* rtn = NULL;   
	jclass clsstring = env->FindClass("java/lang/String");   
	jstring strencode = env->NewStringUTF("GB2312");   
	jmethodID mid = env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");   
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);   
	jsize alen = env->GetArrayLength(barr);   
	jbyte* ba = env->GetByteArrayElements(barr,JNI_FALSE);   
	if(alen > 0)   
	{   
		rtn = (char*)malloc(alen+1);         
		memcpy(rtn,ba,alen);   
		rtn[alen]=0;   
	}   
	env->ReleaseByteArrayElements(barr,ba,0);   
	std::string stemp(rtn);
	free(rtn);
	return stemp;
}

jstring tag::str2jstring(JNIEnv* env,const char* pat)
{
	//定义java String类 strClass
	jclass strClass = (env)->FindClass("Ljava/lang/String;");
	//获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
	jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	//建立byte数组
	jbyteArray bytes = (env)->NewByteArray(strlen(pat));
	//将char* 转换为byte数组
	(env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	// 设置String, 保存语言类型,用于byte数组转换至String时的参数
	jstring encoding = (env)->NewStringUTF("GB2312"); 
	//将byte数组转换为java String,并输出
	return (jstring)(env)->NewObject(strClass, ctorID, bytes, encoding);
}

TD tag::split(std::string &tdString){
	TD result;
	int start = 0;
	int end = 0;
	while(start < tdString.length() && end < tdString.length()){
		if(tdString[end] == '('){
			result.reln = tdString.substr(start, end-start);
			start = end;
			end++;
			continue;
		}
		if(tdString[end] == ',' && tdString[start] == '(' && tdString[end+1] == ' '){
			int pos_begin = end-1;
			while(tdString[pos_begin] != '-') pos_begin--;
			result.gov = tdString.substr(start+1, pos_begin-start-1);
			result.gov_position = atoi(tdString.substr(pos_begin+1, end-pos_begin-1).c_str());
			start = end;
			end++;
			continue;
		}
		if(tdString[end] == ')' && tdString[start] == ','){
			int pos_begin = end-1;
			while(tdString[pos_begin] != '-') pos_begin--;
			result.dep = tdString.substr(start+2, pos_begin-start-2);
			result.dep_position = atoi(tdString.substr(pos_begin+1, end-pos_begin-1).c_str());
			start = end;
			end++;
			continue;
		}
		end++;
	}
	return result;
}

#endif