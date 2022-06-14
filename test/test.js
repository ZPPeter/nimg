
process.chdir(__dirname);

var nimg = require('..');
var fs = require("fs")
var path = require("path")

function write(img, out) {
    img.save(out, "PNG")
}

let img = new nimg.Image("./test.png")
let ori = new nimg.Image(img)

img = img.crop(0, 0, 100, 100)
write(img, "out1.png")
img = img.rotate(90)
write(img, "out2.png")

img = img.resize(200,200)
write(img, "out3.png")


let wh = new nimg.Image(300, 300)
write(wh, "wh.png")
img = wh.draw(img, 50,50)
write(img, "out4.png")
write(ori, "ori.png")
