#include <stddef.h>
#include <node.h>
#include <node_buffer.h>
#include <napi.h>

#define cimg_display 0
#include "cimg/CImg.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

typedef cimg_library::CImg<unsigned char> Image;

Napi::FunctionReference constructor;

class NodeImage : public Napi::ObjectWrap<NodeImage>
{
public:
    NodeImage(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NodeImage>(info)
    {
        Napi::Error err;

        if (info.Length() == 0)
        {
            return;
        }
        else if (info.Length() == 1)
        {
            if (info[0].IsBuffer())
            {
                auto buffer = info[0].As<Napi::Buffer<uint8_t>>();
                this->initWithBuffer(buffer);
            }
            else if (info[0].IsObject() && info[0].ToObject().InstanceOf(constructor.Value()))
            {
                auto img = Unwrap(info[0].As<Napi::Object>());
                this->initWithImage(img);
            }
            else
            {
                err = Napi::Error::New(info.Env(), "argument error.");
            }
        }
        else if (info.Length() == 2)
        {
            int w = info[0].As<Napi::Number>().Int32Value();
            int h = info[1].As<Napi::Number>().Int32Value();
            this->initWithWH(w, h);
        }
        else
        {
            err = Napi::Error::New(info.Env(), "argument error.");
        }

        if (!err.IsEmpty())
        {
#ifdef NAPI_DISABLE_CPP_EXCEPTIONS
            error.ThrowAsJavaScriptException();
#else
            throw err;
#endif // NAPI_DISABLE_CPP_EXCEPTIONS
        }
    }

    ~NodeImage()
    {
        if (_image)
        {
            delete _image;
        }
    }

    Image *_image = {nullptr};
    void initWithImage(NodeImage *img)
    {
        this->_image = new Image(*img->_image);
    }

    void initWithWH(int w, int h)
    {
        this->_image = new Image(nullptr, w, h, 1, 4, false);
    }

    void initWithBuffer(Napi::Buffer<uint8_t> buffer)
    {
        int w, h, comp;
        stbi_uc *data = stbi_load_from_memory(buffer.Data(), buffer.Length(), &w, &h, &comp, 0);
        this->_image = new Image(data, w, h, comp, false);
    }

    Napi::Value width(const Napi::CallbackInfo &info)
    {
        int width = this->_image->width();

        return Napi::Number::New(info.Env(), width);
    }

    Napi::Value height(const Napi::CallbackInfo &info)
    {
        int height = this->_image->height();

        return Napi::Number::New(info.Env(), height);
    }

    Napi::Value fill(const Napi::CallbackInfo &info)
    {

        auto r = (unsigned char)info[0].As<Napi::Number>().DoubleValue();
        auto g = (unsigned char)info[1].As<Napi::Number>().DoubleValue();
        auto b = (unsigned char)info[2].As<Napi::Number>().DoubleValue();
        auto a = (unsigned char)info[3].As<Napi::Number>().DoubleValue();

        return wrap(_image->fill(r, g, b, a));
    }

    Napi::Value crop(const Napi::CallbackInfo &info)
    {

        int x = info[0].As<Napi::Number>().Int32Value();
        int y = info[1].As<Napi::Number>().Int32Value();
        int w = info[2].As<Napi::Number>().Int32Value();
        int h = info[3].As<Napi::Number>().Int32Value();

        return wrap(_image->crop(x, y, x + w, y + h));
    }

    Napi::Value rotate(const Napi::CallbackInfo &info)
    {
        double degrees_ = info[0].As<Napi::Number>();
        return wrap(_image->rotate(degrees_));
    }

    Napi::Value resize(const Napi::CallbackInfo &info)
    {
        int w = info[0].As<Napi::Number>().Int32Value();
        int h = info[1].As<Napi::Number>().Int32Value();

        return wrap(_image->resize(w, h));
    }

    Napi::Value draw(const Napi::CallbackInfo &info)
    {
        NodeImage *other = Unwrap(info[0].As<Napi::Object>());
        int x = info[1].As<Napi::Number>().Int32Value();
        int y = info[2].As<Napi::Number>().Int32Value();

        return wrap(_image->draw_image(x, y, *other->_image));
    }

    Napi::Value toBuffer(const Napi::CallbackInfo &info)
    {

        std::string magic = "PNG";

        if (info.Length() == 1 && info[0].IsString())
        {
            magic = info[0].ToString();
        }

        unsigned char *out;
        int size;
        if (magic == "PNG")
        {
            out = stbi_write_png_to_mem(_image->data(), 0, _image->width(), _image->height(), _image->depth(), &size);
        }
        else if (magic == "JPG")
        {
            //            out = stbi_write_jpg_to_mem(_image->data(), 0, _image->width(), _image->height(), _image->depth(), &size);
        }

        auto buf = Napi::Buffer<uint8_t>::Copy(info.Env(), (uint8_t *)out, size);
        STBIW_FREE(out);

        return buf;
    }

    static Napi::Value wrap(const Image &img)
    {
        auto value = constructor.New({});

        auto subwrap = NodeImage::Unwrap(value);
        subwrap->_image = new Image(img);

        return value;
    }

    static Napi::Object Init(Napi::Env env, Napi::Object exports)
    {
        Napi::Function func = DefineClass(env, "Image",
                                          {
                                              InstanceMethod("width", &NodeImage::width),
                                              InstanceMethod("height", &NodeImage::height),
                                              InstanceMethod("toBuffer", &NodeImage::toBuffer),
                                              InstanceMethod("fill", &NodeImage::fill),
                                              InstanceMethod("crop", &NodeImage::crop),
                                              InstanceMethod("draw", &NodeImage::draw),
                                              InstanceMethod("resize", &NodeImage::resize),
                                              InstanceMethod("rotate", &NodeImage::rotate),
                                          });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();

        exports.Set("Image", func);

        return exports;
    }
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return NodeImage::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
