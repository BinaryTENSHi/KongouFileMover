KongouFileMover
===============

*A utility program, which moves and renames files based on custom regular expressions.*

*Warning: This is my first and (somewhat) usable program written in C++. Please don't cringe about horrible code.*<br>
***You have been warned***

Travis build status
-------------------
[![Build Status](https://travis-ci.org/BinaryTENSHi/KongouFileMover.svg?branch=master)](https://travis-ci.org/BinaryTENSHi/KongouFileMover)

The idea behind KFM
-------------------
Whenever I've downloaded Anime episodes with uTorrent (*don't sue me*), some things bugged me.<br>
Since I have a dedicated partition with all my series on it, I wanted to automate the downloading, moving and renaming of weekly series.
- Automated downloading was no problem, thanks to [ShanaProject](http://www.shanaproject.com/ "ShanaProject").
- Moving wouldn't be a problem too, since uTorrent supports moving files after they're successfully downloaded.<br>
*Unfortunately, you can't rename or move them based on their name.*

The idea is now to write a program, which takes a file name and moves it based on a preconfigured regular expression into a root folder and sub folder.

Example
-------
File name: "[HorribleSubs] Kill la Kill - 23 [1080p].mkv"<br>
Configuration:<br>
```
{
   "root":"M:\\",
   "folderRegex":[
      "remove \\[.*?\\]",
      "remove \\s-\\s.*",
      "trim"
   ],
   "fileRegex":[
      "remove \\[.*?\\]",
      "trim"
   ]
}
```
Generated folder: "M:\Kill la Kill\Kill la Kill - 23.mkv"

Expressions
-----------
### trim
Parameters: none <br>
Trims all leading and trailing spaces from the filename <br>
>Input: " &nbsp; MyFile &nbsp; .mkv" <br>
>Output: "MyFile.mkv" <br>

### remove
Parameters: one: regex <br>
Removes everything that matches the given regular expression <br>
>Config: remove \\\\[.*?\\\\] <br>
>Input: "[Something]Hello_World[Different].mkv" <br>
>Output: "Hello_World.mkv" <br>

### replace
Parameters: two: regex, 'value'<br>
Replaces everything that matches the given regular expression with the given value <br>
>Config: replace _,' ' <br>
>Input: "Hello_World.mkv" <br>
>Output: "Hello World.mkv" <br>

Libraries
---------
KFM requires the following libraries:
- Boost (version 1.56.0)
 - system
 - filesystem
 - regex
 - program_options

Building
--------
KFM utilizes [CMake](http://www.cmake.org/) for building.
The procedure to clone and build KFM could be the following:
```
> git clone https://github.com/BinaryTENSHi/KongouFileMover.git
> cd KongouFileMover
> mkdir build
> cd build
> cmake ..
> make
```
The built binary can be found at build/KongouFileMover/kongoufilemover(.exe|)

Features
--------
- Custom regular expressions

Why Kongou you ask? I like the name. That's all.
