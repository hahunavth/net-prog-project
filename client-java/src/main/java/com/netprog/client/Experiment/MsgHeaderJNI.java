package com.netprog.client.Experiment;

public class MsgHeaderJNI {
    public int content_type;
    public long content_len;
    public long offset;
    public long body_size;

    @Override
    public String toString() {
        return "MsgHeader{" +
                "content_type=" + content_type +
                ", content_len=" + content_len +
                ", offset=" + offset +
                ", body_size=" + body_size +
                '}';
    }
}