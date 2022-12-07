package com.netprog.client.Experiment;

public class MsgExtractorJNI {
    static {
        System.loadLibrary(
                "msg_extractor_jni"
        );
    }

    public native MsgHeaderJNI extractHeader (char[] frame);
    public native MsgHeaderJNI extractFrame (char[] frame);
    public native MsgHeaderJNI extractExperiment (char[] frame);

    public static void main (String[] args) {
        MsgExtractorJNI msgExtractorJNI = new MsgExtractorJNI();
        char[] l = new char[100];
        MsgHeaderJNI header = (msgExtractorJNI.extractHeader(l));
        System.out.println(header);
    }
}
