package com.netprog.client.Experiment;

import com.netprog.client.MsgHeader;
import jnr.ffi.LibraryLoader;
import jnr.ffi.Pointer;
import jnr.ffi.Runtime;
import jnr.ffi.Struct;

public class TryJNRFFI {
    public interface Chatptcl {
        int encode_msg_header_partial(MsgHeader h, char[] buffer, int buffer_size, long offset);
        Pointer decode_msg_header_partial(MsgHeader h, char[] buffer, int buffer_size);
    }

    public static void main(String[] args) {
        Chatptcl libc = LibraryLoader.create(Chatptcl.class).load("chatptclm");
        Runtime runtime = Runtime.getRuntime(libc);
        MsgHeader h = new MsgHeader(runtime);

        char[] bytes = new char[100];
        bytes[0] = 'a';
        bytes[1] = 'b';
        bytes[2] = 'v';
        System.out.println(libc.decode_msg_header_partial(h, bytes, 100));
        System.out.println(h);

        char[] bytes2 = new char[100];
        libc.encode_msg_header_partial(h, bytes2, 100, 0);
        System.out.println(bytes2);
    }
}
