
let cpp = require("./build/Release/nimg.node")

let nimg = {
    Image : cpp.Image,
    CompositeOperator: cpp.CompositeOperator,
}

module.exports = nimg

