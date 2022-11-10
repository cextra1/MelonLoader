#include <jni.h>
#include "./Logger.h"

extern "C" {
	void Java_com_melonloader_LogBridge_error_1internal__Ljava_lang_String_2(JNIEnv* env, jclass type, jstring msg)
	{
		const char* cMsg = env->GetStringUTFChars(msg, nullptr);

		Logger::QuickLog(cMsg, LogType::Error);

		env->ReleaseStringUTFChars(msg, cMsg);
	}

	void Java_com_melonloader_LogBridge_warning_1internal__Ljava_lang_String_2(JNIEnv* env, jclass type, jstring msg)
	{
		const char* cMsg = env->GetStringUTFChars(msg, nullptr);

        Logger::QuickLog(cMsg, LogType::Warning);

		env->ReleaseStringUTFChars(msg, cMsg);
	}

	void Java_com_melonloader_LogBridge_msg_1internal__Ljava_lang_String_2(JNIEnv* env, jclass type, jstring msg)
	{
		const char* cMsg = env->GetStringUTFChars(msg, nullptr);

		Logger::QuickLog(cMsg);

		env->ReleaseStringUTFChars(msg, cMsg);
	}
}