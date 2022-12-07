package com.netprog.client;

import jnr.ffi.Struct;

public class MsgHeader extends Struct {
    public Struct.Signed32 content_type = new Struct.Signed32();
    public Struct.SignedLong content_len = new Struct.SignedLong();
    public Struct.SignedLong offset = new Struct.SignedLong();
    public Struct.SignedLong body_size = new Struct.SignedLong();

    // Necessary constructor that takes a Runtime
    public MsgHeader(jnr.ffi.Runtime runtime) {
        super(runtime);
    }

    // You can add your own methods of choice
    public void setTime(int content_type, long content_len, long offset, long body_size) {

    }
}
