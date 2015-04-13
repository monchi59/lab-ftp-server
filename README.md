# lab-ftp-server
Very Simple FTP server implemented in C.

Handles concurrent client connections on different processes.

## Build
```
make
./ftp_server
```

## Available commands:
Login commands desn't check credentials:
* USER
* PASS

System is always "UNIX":
* SYST

No additional features are proposed:
* FEAT

Handled:
* POTR
* RETR
* STOR
* LIST
* TYPE
* PWD
* QUIT

## Tested on
GNU/Linux 3.18.5 Ubuntu 14.10

Built with gcc 4.9.1
