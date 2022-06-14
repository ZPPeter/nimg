
#include <sstream>

#define cimg_display 0
#define cimg_use_png 1
#define cimg_use_jpeg 1
#include "cimg/CImg.h"

typedef cimg_library::CImg<unsigned char> Image;

int main() {
    
    Image img = Image("/Users/mac/my/nimg/test/test.png");//data, bpp, w, h, 1);

    Image co = Image(128,128,1,4,0);
    co.save_png("/Users/mac/my/nimg/test/ccc.png");
    
    Image crop = img.crop(0,0,100-1,100-1);
    
    Image ro = crop.get_rotate(90);
    
    crop.save_png("/Users/mac/my/nimg/test/crop.png");
    ro.save_png("/Users/mac/my/nimg/test/ro.png");
}
