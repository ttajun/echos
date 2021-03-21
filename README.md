# echo server

c언어로 작성한 간단한 에코서버

## Makefile

컴파일)

```bash
make all
```

삭제)

```bash
make clean
```

## Usage

서버)    
socket permission 때문에 root로 실행해야 합니다.

```bash
$ sudo ./echos <port>
```

클라이언트)

```bash
$ ./echoc <ip> <port>
```
