package com.netprog.client;

public class MsgExtractorJNI {
    static {
        System.loadLibrary(
                "msg_extractor_jni"
        );
    }

    public native MsgHeader extractHeader (char[] frame);
    public native MsgHeader extractFrame (char[] frame);
    public native MsgHeader extractExperiment (char[] frame);

    public static void main (String[] args) {
        MsgExtractorJNI msgExtractorJNI = new MsgExtractorJNI();
        char[] l = new char[100];
        l[0] = 'a';
        l[1] = 'b';
        l[2] = 'b';
        l[3] = 'b';
        l[4] = 'b';
        l[5] = 'b';
        l[6] = 'b';
        l[7] = 'b';
        l[8] = 'b';
        l[9] = 'b';
        l[10] = 'b';
        MsgHeader header = (msgExtractorJNI.extractHeader(l));
        System.out.println(header);
    }
}
