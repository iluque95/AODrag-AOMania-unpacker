# AODrag and AOMania unpacker

Is only supported old versions before than 6.0 and 5.0 respectively.

## How it works

Program reads the binary file which contains all graphics (bmp images) one behind the other, searching bmp header, 
which is the two first bytes of the structure (0x424D) and it saves in an auxiliary file the offset pointer where begins the image. Also contains
image size.

When the first loop is finished, the program reads the auxiliary file to move the pointer to the beginning of the first image and 
gets it as a block. Block size is the amount of bytes that image size is and the program saves that buffer in the output folder as an integrity image.

## Future work

Maybe is not required export all data to an auxiliary file but for understanding purposes perhaps is preferably.
Another thing to keep in mind is avoiding to mixing different standards and languages. For example, I am using C syntax in several cases. Maybe it will be great use a modern C++ standard such as C++17 and take benefit from the STL and its templates.

## Source

Code was redesigned from unknown source in that moment (https://pastebin.com/m8DgAgmk). Up to now is known as [@Wolftein](https://github.com/Wolftein).


***CRaW***

## License
[GNU](https://opensource.org/licenses/GPL-2.0)
