
```c
EXTERN EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char **argv)
{
  printf("MyFunction Called\n");
}
```

```bash
  mkdir b; cd b;
  cmake ..;
  make;

  python -m http.server
```