# Network programming project

## Build

```shell
mkdir b
cd b
cmake ..
make
```

## Run

```shell
make && ./exec
```

## Debug

### Memory leak
```shell
make && valgrind --leak-check=full  --show-leak-kinds=all  ./exec
```

### Vscode

- Open debug tab --> Start debugging

## Test
```shell
make && make test
```

```shell
netcat localhost 5551 
```