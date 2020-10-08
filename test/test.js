
process.chdir(__dirname);

var nimg = require('..');
var fs = require("fs")

function write(img, out) {
    fs.writeFileSync(out, img.toBuffer())
}

let pngdata = fs.readFileSync("./test.png")


let img = new nimg.Image(pngdata)
let ori = new nimg.Image(img)

img = img.crop(0, 0, 100, 100)
write(img, "out1.png")

img = img.rotate(90)
write(img, "out2.png")

img = img.resize(200,200)
write(img, "out3.png")


img = img.fill(255,0,0,255)
img = ori.draw(img, 50,50,  nimg.CompositeOperator.OverCompositeOp)
write(img, "out4.png")
