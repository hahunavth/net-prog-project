#include "../lib/common/all.h"
#include "./j_header/com_netprog_client_MsgExtractorJNI.h"

// ----------------------------------
// MsgHeader
// ----------------------------------

typedef struct
{
  jobject obj;
  //
  jfieldID content_type;
  jfieldID content_len;
  jfieldID offset;
  jfieldID body_size;
} MsgHeader;

MsgHeader *newMsgHeader(JNIEnv *env)
{
  // Create the object of the class MsgHeader
  jclass MsgHeaderCls = (*env)->FindClass(env, "com/netprog/client/MsgHeader");

  MsgHeader *instance = (MsgHeader *)calloc(1, sizeof(MsgHeader));

  instance->obj = (*env)->AllocObject(env, MsgHeaderCls);
  instance->content_type = (*env)->GetFieldID(env, MsgHeaderCls, "content_type", "I");
  instance->content_len = (*env)->GetFieldID(env, MsgHeaderCls, "content_len", "J");
  instance->offset = (*env)->GetFieldID(env, MsgHeaderCls, "offset", "J");
  instance->body_size = (*env)->GetFieldID(env, MsgHeaderCls, "body_size", "J");

  return instance;
}

void setMsgHeader(JNIEnv *env, MsgHeader *instance, msg_header_t *header)
{
  (*env)->SetIntField(env, instance->obj, instance->content_type, header->content_type);
  (*env)->SetLongField(env, instance->obj, instance->content_len, header->content_len);
  (*env)->SetLongField(env, instance->obj, instance->body_size, header->body_size);
  (*env)->SetLongField(env, instance->obj, instance->offset, header->offset);
}

// ----------------------------------
// EXPORT FUNCTION
// ----------------------------------

JNIEXPORT
jobject JNICALL
Java_com_netprog_client_MsgExtractorJNI_extractHeader(
    JNIEnv *env,
    jobject thisObject,
    jcharArray frame)
{
  MsgHeader *instance = newMsgHeader(env);

  jsize len = (*env)->GetArrayLength(env, frame);
  jchar *jbuffer = (jchar *)calloc(len, sizeof(jchar));
  char *buffer = (char *)calloc(len, sizeof(char));
  (*env)->GetCharArrayRegion(env, frame, 0, len, jbuffer);
  for (int i = 0; i < len; i++)
    buffer[i] = jbuffer[i];
  msg_header_t *header = create_msg_header(0, 100);
  decode_msg_header_partial(header, buffer, len);
  // PRINT_INT(header->content_type);
  // PRINT_LONG(header->offset);
  setMsgHeader(env, instance, header);

  // return
  jobject result = instance->obj;
  free(jbuffer);
  free(buffer);
  free(header);
  free(instance);
  return result;
}

JNIEXPORT jobject JNICALL
Java_com_netprog_client_MsgExtractorJNI_extractFrame(
    JNIEnv *env,
    jobject thisObject,
    jcharArray frame)
{
  return NULL;
}

JNIEXPORT jobject JNICALL
Java_com_netprog_client_MsgExtractorJNI_extractExperiment(
    JNIEnv *env,
    jobject thisObject,
    jcharArray frame)
{
  return NULL;
}