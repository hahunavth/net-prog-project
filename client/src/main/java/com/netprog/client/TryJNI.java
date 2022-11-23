package com.netprog.client;

import java.util.Objects;

public class TryJNI {
    static {
        System.loadLibrary(
//                Objects.requireNonNull(TryJNI.class.getClassLoader().getResource("libchatptcl.so")).getPath()
                "/home/kryo/Desktop/net-prog/project/client/src/main/resources/libchatptcl.so"
        );
    }

    private native void trim();
    private native int trim(String a);
    private native String trim(Double b, int c);

    public static void main(String[] args) {
        new TryJNI();
    }
}
