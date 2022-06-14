/// <reference types="node" />
export = nimg;

declare namespace nimg {

    class Image {
        constructor()
        constructor(w: number, h: number)
        constructor(file: string)
        constructor(img: Image)

        width(): number;
        height(): number;
        save(file: string, magic: string = "PNG"): Buffer;
        crop(x: number, y: number, w: number, h: number): Image;
        draw(img: Image, x: number, y: number): Image;
        resize(w: number, h: number): Image;
        rotate(degree: number): Image;
    }
}