/// <reference types="node" />
export = nimg;

declare namespace nimg {

    class Image {
        constructor()
        constructor(w: number, h: number)
        constructor(b: Buffer)
        constructor(img: Image)

        width(): number;
        height(): number;
        toBuffer(magic: string = "PNG"): Buffer;
        fill(color: string): Image;
        fill(r: number, g: number, b: number, a: number): Image;
        crop(x: number, y: number, w: number, h: number): Image;
        draw(img: Image, x: number, y: number): Image;
        resize(w: number, h: number): Image;
        rotate(degree: number): Image;
    }
}