# Server
## Java:
- Sử dụng jnf-ffi
  - Module c: libchatptclm.so (tên: chatptclm)

## Folder structure:

```bash
./server
├── b                 # Folder tạo ra khi build
├── build             # Folder vscode tạo ra khi build 
├── jni               # Java native library: **deprecated**
│   ├── j_header          # File header tự gen từ java file
│   └── utils             #
├── lib               # Thư viện chung
│   ├── cgen              # data structures (gdlist, rbtree, ...)
│   ├── common            # chứa file header để import cho cả thư viện
│   ├── msg               # mã hóa và giải mã message frame
│   └── utils             # 
├── test              # test lib
│   ├── base              # 
│   ├── ds                # data structures
│   ├── net               # test server / client
│   └── utils             #
└── wasm              # Web asembly: **deprecated**
    ├── html_template     # template 
    └── public            # generate
```
