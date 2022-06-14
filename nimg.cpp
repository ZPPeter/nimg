#include <stddef.h>
#include <node.h>
#include <node_buffer.h>
#include <napi.h>

#define cimg_display 0
#define cimg_use_png 1
#define cimg_use_jpeg 1
#include "lib/cimg/CImg.h"

typedef cimg_library::CImg<unsigned char> Image;

Napi::FunctionReference constructor;

class NodeImage : public Napi::ObjectWrap<NodeImage>
{
public:
    Image *_image = nullptr;

    NodeImage(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NodeImage>(info)
    {
        if (info.Length() == 0)
            return;

        if (info.Length() == 1)
        {
            if (info[0].IsString())
            {
                std::string s = info[0].As<Napi::String>();
                this->_image = new Image(s.c_str());
            }
            else if (info[0].IsObject() && info[0].ToObject().InstanceOf(constructor.Value()))
            {
                auto img = Unwrap(info[0].As<Napi::Object>());
                this->_image = new Image(*img->_image);
            }
            else
            {
                throw Napi::Error::New(info.Env(), "argument error.");
            }
        }
        else if (info.Length() == 2)
        {
            int w = info[0].As<Napi::Number>().Int32Value();
            int h = info[1].As<Napi::Number>().Int32Value();

            this->_image = new Image(w, h, 1, 4, 0);
        }
        else
        {
            throw Napi::Error::New(info.Env(), "argument error.");
        }
    }

    ~NodeImage()
    {
        if (_image)
        {
            delete _image;
        }
    }

    Napi::Value width(const Napi::CallbackInfo &info)
    {
        return Napi::Number::New(info.Env(), _image->width());
    }

    Napi::Value height(const Napi::CallbackInfo &info)
    {
        return Napi::Number::New(info.Env(), _image->height());
    }

    Napi::Value crop(const Napi::CallbackInfo &info)
    {
        int x = info[0].As<Napi::Number>().Int32Value();
        int y = info[1].As<Napi::Number>().Int32Value();
        int w = info[2].As<Napi::Number>().Int32Value();
        int h = info[3].As<Napi::Number>().Int32Value();

        return wrap(_image->crop(x, y, x + w - 1, y + h - 1));
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

    Napi::Value save(const Napi::CallbackInfo &info)
    {
        std::string path = info[0].As<Napi::String>();
        std::string magic = "PNG";

        if (info.Length() == 2)
        {
            magic = info[1].As<Napi::String>();
        }

        if (magic == "PNG")
        {
            _image->save_png(path.c_str());
        }
        else if (magic == "JPG")
        {
            _image->save_jpeg(path.c_str());
        }

        return Napi::Value();
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
                                              InstanceMethod("save", &NodeImage::save),
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
