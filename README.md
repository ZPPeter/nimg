# nimg

Cross-platform image decoder and encoder for Node.js.<br>
Node.js轻量级跨平台图像编解码库

Features

  * Base on [CMake](https://cmake.org/) + [node-addon-api](https://github.com/nodejs/node-addon-api) + [Magick++](http://www.imagemagick.org/Magick++/)
  * 基于[CMake](https://cmake.org/) + [node-addon-api](https://github.com/nodejs/node-addon-api) + [Magick++](http://www.imagemagick.org/Magick++/), 可靠，且易于扩展
  * Easy to use
  * 简单易用

Useage
```typescript
class Image {
    constructor()
    constructor(w : number, h : number)
    constructor(b : Buffer)
    constructor(img : Image)

    width(): number;
    height(): number;
    toBuffer(magic:string = "PNG") : Buffer;
    fill(color:string) : Image;
    fill(r : number, g : number, b : number, a : number) : Image;
    crop(x : number, y : number, w : number, h : number) : Image;
    draw(img : Image, x : number, y : number, op = OverCompositeOp) : Image;
    resize(w : number, h : number) : Image;
    rotate(degree: number) : Image;
}
```
