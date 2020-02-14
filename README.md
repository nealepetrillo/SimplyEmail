# SimplyEmail
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ee40293144e94e879770c6038540808e)](https://www.codacy.com/manual/nealepetrillo/SimplyEmail?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=nealepetrillo/SimplyEmail&amp;utm_campaign=Badge_Grade)

A simple C++ email implementation leveraging CURL. 

## Building

### Supported Operating Systems
SimplyEmail has been tested the following operating systems:

| OS           | LibCURL Version |
| ------------ | --------------- |
| Ubuntu 18.04 | 7.58.0          |
| RHEL 7.7     | 7.29.0          |

SimplyEmail stictly complies with the C++ 11 standard and so `should` work on any operating system that uspport libcurl and a C++ 11 compiler. However, no testing has been done on any other operating systems. 

### Requirements
The following are required to build SimplyEmail:
-   CMake 2.8+
-   libcurl 7.29+
-   GCC 4.8.5+
 
Installing required packages on Ubuntu 18:
```ShellSession
$ sudo apt-get install cmake libcurl-devel gcc
```

Installing required packages on RHEL 7:
```ShellSession
$ sudo yum install cmake libcurl-devel gcc
```

### Build steps
Cmake build
```ShellSession
$ git clone git@github.com:nealepetrillo/SimplyEmail.git
Cloning into 'SimplyEmail'...
remote: Enumerating objects: 65, done.
remote: Counting objects: 100% (65/65), done.
remote: Compressing objects: 100% (54/54), done.
remote: Total 95 (delta 13), reused 47 (delta 9), pack-reused 30
Receiving objects: 100% (95/95), 62.79 KiB | 1.14 MiB/s, done.
Resolving deltas: 100% (20/20), done.
$ cd SimplyEmail/
$ mkdir build
$ cd build
$ cmake ..
-- The CXX compiler identification is GNU 7.4.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found CURL: /usr/lib/x86_64-linux-gnu/libcurl.so (found version "7.58.0")
-- Configuring done
-- Generating done
-- Build files have been written to: /home/sgnb221/SimplyEmail/build
$ cmake --build .
Scanning dependencies of target simplyemail
[ 25%] Building CXX object CMakeFiles/simplyemail.dir/src/Email.cpp.o
[ 50%] Building CXX object CMakeFiles/simplyemail.dir/src/EmailAttachment.cpp.o
[ 75%] Building CXX object CMakeFiles/simplyemail.dir/src/SMTPConnection.cpp.o
[100%] Linking CXX static library libsimplyemail.a
[100%] Built target simplyemail
```
