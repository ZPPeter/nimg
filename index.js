
let cpp = require("./build/Release/Release/nimg.node")

let nimg = {
    Image : cpp.Image,
    CompositeOperator: cpp.CompositeOperator,
}

module.exports = nimg

