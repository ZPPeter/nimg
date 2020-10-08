
#include <stddef.h>
#include <node.h>
#include <node_buffer.h>
#include <Magick++.h>
#include <napi.h>

using namespace Magick;

Napi::FunctionReference constructor;

class NodeImage : public Napi::ObjectWrap<NodeImage>  {
public:
    NodeImage(const Napi::CallbackInfo& info) :
        Napi::ObjectWrap<NodeImage>(info)
    {
        Napi::Error err;

        if (info.Length() == 0) {
            return;
        } else if (info.Length() == 1) {
            if (info[0].IsBuffer()) {
                auto buffer = info[0].As<Napi::Buffer<uint8_t>>();
                this->initWithBuffer(buffer);
            } else if (info[0].IsObject() && info[0].ToObject().InstanceOf(constructor.Value())) {
                auto img = Unwrap(info[0].As<Napi::Object>());
                this->initWithImage(img);
            } else {
                err = Napi::Error::New(info.Env(), "argument error.");
            }
        } else if (info.Length() == 2) {
            int w = info[0].As<Napi::Number>().Int32Value();
            int h = info[1].As<Napi::Number>().Int32Value();
            this->initWithWH(w, h);
        } else {
            err = Napi::Error::New(info.Env(), "argument error.");
        }
        
        if (!err.IsEmpty()) {
#ifdef NAPI_DISABLE_CPP_EXCEPTIONS
            error.ThrowAsJavaScriptException();
#else
            throw err;
#endif  // NAPI_DISABLE_CPP_EXCEPTIONS
        }
    }
    
    ~NodeImage() {
        if (_image) {
            delete _image;
        }
    }

    Image* _image = { nullptr };
    void initWithImage(NodeImage* img) {
        this->_image = new Image(*img->_image);
    }

    void initWithWH(int w, int h) {
        this->_image = new Image(Geometry(w, h), ColorRGB(0, 0, 0, 0));
    }

    void initWithBuffer(Napi::Buffer<uint8_t> buffer) {
        Blob b(buffer.Data(), buffer.ByteLength());
        this->_image = new Image(b);
    }

    Napi::Value width(const Napi::CallbackInfo& info) {
        int width = this->_image->columns();

        return Napi::Number::New(info.Env(), width);
    }

    Napi::Value height(const Napi::CallbackInfo& info) {
        int height = this->_image->rows();

        return Napi::Number::New(info.Env(), height);
    }
    
    Napi::Value fill(const Napi::CallbackInfo& info) {
        
        Color c;
        
        if (info.Length() == 1 && info[0].IsString()) {
            c = Color(info[0].ToString());
        } else {
            double r = info[0].As<Napi::Number>();
            double g = info[1].As<Napi::Number>();
            double b = info[2].As<Napi::Number>();
            double a = info[3].As<Napi::Number>();
            
            c = ColorRGB(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
        }
        
        Image img(Geometry(this->_image->columns(), this->_image->rows()), c);
//        img.fillColor(c);
        return wrap(img);
    }
    
    Napi::Value crop(const Napi::CallbackInfo& info) {
        
        Image img(*this->_image);
        
        int x = info[0].As<Napi::Number>().Int32Value();
        int y = info[1].As<Napi::Number>().Int32Value();
        int w = info[2].As<Napi::Number>().Int32Value();
        int h = info[3].As<Napi::Number>().Int32Value();
        
        img.chop(Geometry(x,y));
        img.crop(Geometry(w,h));
        
        return wrap(img);
    }
    
    Napi::Value rotate(const Napi::CallbackInfo& info) {
        double degrees_ = info[0].As<Napi::Number>();
        
        Image img(*this->_image);
        img.rotate(degrees_);
        return wrap(img);
    }
    
    Napi::Value resize(const Napi::CallbackInfo& info) {
        int w = info[0].As<Napi::Number>().Int32Value();
        int h = info[1].As<Napi::Number>().Int32Value();
        
        Image img(*this->_image);
        img.resize(Geometry(w, h));
        return wrap(img);
    }
    
    Napi::Value draw(const Napi::CallbackInfo& info) {
        NodeImage* other = Unwrap(info[0].As<Napi::Object>());
        int x = info[1].As<Napi::Number>().Int32Value();
        int y = info[2].As<Napi::Number>().Int32Value();
        
        int op = InCompositeOp;
        if (info.Length() == 4) {
            op = info[3].ToNumber().Int32Value();
        }
        
        Image img(*this->_image);
        img.composite(*other->_image, x, y, (CompositeOperator)op);
        return wrap(img);
    }
    
    Napi::Value toBuffer(const Napi::CallbackInfo& info) {
        
        std::string magic = "PNG";
        
        if (info.Length() == 1 && info[0].IsString()) {
            magic = info[0].ToString();
        }
        
        Blob b;
        this->_image->write(&b, magic);
        return Napi::Buffer<uint8_t>::Copy(info.Env(), (uint8_t*)b.data(), b.length());
    }
    
    static Napi::Value wrap(const Image& img) {
        auto value = constructor.New({});
        
        auto subwrap = NodeImage::Unwrap(value);
        subwrap->_image = new Image(img);
        
        return value;
    }
    
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
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
        
        Napi::Object op = Napi::Object::New(env);
        
        op["UndefinedCompositeOp"] = Napi::Number::New(env, UndefinedCompositeOp);
        op["AlphaCompositeOp"] = Napi::Number::New(env, AlphaCompositeOp);
        op["AtopCompositeOp"] = Napi::Number::New(env, AtopCompositeOp);
        op["BlendCompositeOp"] = Napi::Number::New(env, BlendCompositeOp);
        op["BlurCompositeOp"] = Napi::Number::New(env, BlurCompositeOp);
        op["BumpmapCompositeOp"] = Napi::Number::New(env, BumpmapCompositeOp);
        op["ChangeMaskCompositeOp"] = Napi::Number::New(env, ChangeMaskCompositeOp);
        op["ClearCompositeOp"] = Napi::Number::New(env, ClearCompositeOp);
        op["ColorBurnCompositeOp"] = Napi::Number::New(env, ColorBurnCompositeOp);
        op["ColorDodgeCompositeOp"] = Napi::Number::New(env, ColorDodgeCompositeOp);
        op["ColorizeCompositeOp"] = Napi::Number::New(env, ColorizeCompositeOp);
        op["CopyBlackCompositeOp"] = Napi::Number::New(env, CopyBlackCompositeOp);
        op["CopyBlueCompositeOp"] = Napi::Number::New(env, CopyBlueCompositeOp);
        op["CopyCompositeOp"] = Napi::Number::New(env, CopyCompositeOp);
        op["CopyCyanCompositeOp"] = Napi::Number::New(env, CopyCyanCompositeOp);
        op["CopyGreenCompositeOp"] = Napi::Number::New(env, CopyGreenCompositeOp);
        op["CopyMagentaCompositeOp"] = Napi::Number::New(env, CopyMagentaCompositeOp);
        op["CopyAlphaCompositeOp"] = Napi::Number::New(env, CopyAlphaCompositeOp);
        op["CopyRedCompositeOp"] = Napi::Number::New(env, CopyRedCompositeOp);
        op["CopyYellowCompositeOp"] = Napi::Number::New(env, CopyYellowCompositeOp);
        op["DarkenCompositeOp"] = Napi::Number::New(env, DarkenCompositeOp);
        op["DarkenIntensityCompositeOp"] = Napi::Number::New(env, DarkenIntensityCompositeOp);
        op["DifferenceCompositeOp"] = Napi::Number::New(env, DifferenceCompositeOp);
        op["DisplaceCompositeOp"] = Napi::Number::New(env, DisplaceCompositeOp);
        op["DissolveCompositeOp"] = Napi::Number::New(env, DissolveCompositeOp);
        op["DistortCompositeOp"] = Napi::Number::New(env, DistortCompositeOp);
        op["DivideDstCompositeOp"] = Napi::Number::New(env, DivideDstCompositeOp);
        op["DivideSrcCompositeOp"] = Napi::Number::New(env, DivideSrcCompositeOp);
        op["DstAtopCompositeOp"] = Napi::Number::New(env, DstAtopCompositeOp);
        op["DstCompositeOp"] = Napi::Number::New(env, DstCompositeOp);
        op["DstInCompositeOp"] = Napi::Number::New(env, DstInCompositeOp);
        op["DstOutCompositeOp"] = Napi::Number::New(env, DstOutCompositeOp);
        op["DstOverCompositeOp"] = Napi::Number::New(env, DstOverCompositeOp);
        op["ExclusionCompositeOp"] = Napi::Number::New(env, ExclusionCompositeOp);
        op["HardLightCompositeOp"] = Napi::Number::New(env, HardLightCompositeOp);
        op["HardMixCompositeOp"] = Napi::Number::New(env, HardMixCompositeOp);
        op["HueCompositeOp"] = Napi::Number::New(env, HueCompositeOp);
        op["InCompositeOp"] = Napi::Number::New(env, InCompositeOp);
        op["IntensityCompositeOp"] = Napi::Number::New(env, IntensityCompositeOp);
        op["LightenCompositeOp"] = Napi::Number::New(env, LightenCompositeOp);
        op["LightenIntensityCompositeOp"] = Napi::Number::New(env, LightenIntensityCompositeOp);
        op["LinearBurnCompositeOp"] = Napi::Number::New(env, LinearBurnCompositeOp);
        op["LinearDodgeCompositeOp"] = Napi::Number::New(env, LinearDodgeCompositeOp);
        op["LinearLightCompositeOp"] = Napi::Number::New(env, LinearLightCompositeOp);
        op["LuminizeCompositeOp"] = Napi::Number::New(env, LuminizeCompositeOp);
        op["MathematicsCompositeOp"] = Napi::Number::New(env, MathematicsCompositeOp);
        op["MinusDstCompositeOp"] = Napi::Number::New(env, MinusDstCompositeOp);
        op["MinusSrcCompositeOp"] = Napi::Number::New(env, MinusSrcCompositeOp);
        op["ModulateCompositeOp"] = Napi::Number::New(env, ModulateCompositeOp);
        op["ModulusAddCompositeOp"] = Napi::Number::New(env, ModulusAddCompositeOp);
        op["ModulusSubtractCompositeOp"] = Napi::Number::New(env, ModulusSubtractCompositeOp);
        op["MultiplyCompositeOp"] = Napi::Number::New(env, MultiplyCompositeOp);
        op["NoCompositeOp"] = Napi::Number::New(env, NoCompositeOp);
        op["OutCompositeOp"] = Napi::Number::New(env, OutCompositeOp);
        op["OverCompositeOp"] = Napi::Number::New(env, OverCompositeOp);
        op["OverlayCompositeOp"] = Napi::Number::New(env, OverlayCompositeOp);
        op["PegtopLightCompositeOp"] = Napi::Number::New(env, PegtopLightCompositeOp);
        op["PinLightCompositeOp"] = Napi::Number::New(env, PinLightCompositeOp);
        op["PlusCompositeOp"] = Napi::Number::New(env, PlusCompositeOp);
        op["ReplaceCompositeOp"] = Napi::Number::New(env, ReplaceCompositeOp);
        op["SaturateCompositeOp"] = Napi::Number::New(env, SaturateCompositeOp);
        op["ScreenCompositeOp"] = Napi::Number::New(env, ScreenCompositeOp);
        op["SoftLightCompositeOp"] = Napi::Number::New(env, SoftLightCompositeOp);
        op["SrcAtopCompositeOp"] = Napi::Number::New(env, SrcAtopCompositeOp);
        op["SrcCompositeOp"] = Napi::Number::New(env, SrcCompositeOp);
        op["SrcInCompositeOp"] = Napi::Number::New(env, SrcInCompositeOp);
        op["SrcOutCompositeOp"] = Napi::Number::New(env, SrcOutCompositeOp);
        op["SrcOverCompositeOp"] = Napi::Number::New(env, SrcOverCompositeOp);
        op["ThresholdCompositeOp"] = Napi::Number::New(env, ThresholdCompositeOp);
        op["VividLightCompositeOp"] = Napi::Number::New(env, VividLightCompositeOp);
        op["XorCompositeOp"] = Napi::Number::New(env, XorCompositeOp);
        op["StereoCompositeOp"] = Napi::Number::New(env, CompositeOperator::StereoCompositeOp);
        
        exports.Set("Image", func);
        exports.Set("CompositeOperator", op);

        return exports;
    }
};


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return NodeImage::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
