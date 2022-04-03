# ETECubeChat
Simple Chat Application based on TCP/IP, made with C++


### Summary

   * It's basically a normal chat room. TCP-based server listens for connections and they're connected with WinSockets.

   * .clang-format is made according to my own preference, you can change it to your liking.

   * added `libsodium` for encrpytion/decrpytion. Plan was to secure messages with RSA (Rivest–Shamir–Adleman) public-key cryptosystem, it has some API for the encryption flow but couldn't finished it on time.

   * Every C++ chat application example I saw on the internet was either based on QT's example or done in UNIX/LINUX, but none were using WinSocks or CMAKE whatsoever  

   * Very little comments on the code, pretty straightforward

   * To make it work, either create to src/libsodium folder and clone [libsodium](https://github.com/jedisct1/libsodium/tree/4f5e89fa84ce1d178a6765b8b46f2b6f91216677) into it or comment out its part on CMakeLists.txt

### To-Do:

   * Implement the encryption work-flow
   
   * Syntax is kinda mixed, might spend time on standardizing modern C++

   * There are surely const and/or constexpr applicaple things all over the code. Might take a look at it for a better performance

   * Some functions can be decoupled. Not every function has 100% single responsibility, but it's not too bad.

   * There are some warnings. Treat Warnings as Error is closed now but regardless it's technical debt.