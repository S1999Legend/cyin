# Cyin - Coin Quest RPG

yo this is a small console app I made where you can create an adventurer profile and collect/spend gold coins. you can also send coins to other adventurers if you know their ID. it's not super fancy or anything, just something I put together for fun.

run it in any C++ compiler and it should work. enjoy lol

I might add a direct executable thingy in releases, so watch out buddy.


## build instructions

Here's a video of build process on Linux, same goes for Mac users.

[LINK](https://hc-cdn.hel1.your-objectstorage.com/s/v3/6ad1e52ebfff8b1ce6328ca8f08d36aeddae4d80_video.mp4)

to build cyin from source:

### windows
- make sure you have a c++ compiler installed (mingw or visual studio).
- open terminal or cmd in the project folder.
- run:
  ```
   g++ cyin.cpp -o cyin
  ```
  run the game:
  ```
    ./cyin
  ```
### linux / macos
  - open terminal in the project folder.
  - make sure g++ is installed. (on ubuntu/debian: sudo apt install g++)
  - build the game:
  ```
    g++ cyin.cpp -o cyin
  ```
  run it:
  ```
    ./cyin
  ```
no external libraries needed, it only uses the c++ standard library.
