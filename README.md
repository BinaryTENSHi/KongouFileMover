KongouFileMover
===============

*A utility program, which moves and renames files based on custom regular expressions.*

*Warning: This is my first and (somewhat) usable program written in C++. Please don't cringe about horrible code.*<br>
***You have been warned***

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
root=M:\
folder {
 remove \[.*?\]
 remove \s-\s.*
 trim
}
file {
 remove \[.*?\]
 trim
}
```
Usable methods:
- remove *$regex*
- replace *$regex*,*'$value'*
- trim

**Extensions will be ignored**

Generated folder: "M:\Kill la Kill\Kill la Kill - 23.mkv"

Features
--------
- Custom regular expressions
- Progress notifications as popups
- Switch the graphic on the popup

Why Kongou you ask? I like the name. That's all.
