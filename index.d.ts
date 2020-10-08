/// <reference types="node" />
export = nimg;

declare namespace nimg {
    
    class Image {
        constructor()
        constructor(w : number, h : number)
        constructor(b : Buffer)
        constructor(img : Image)

        width(): number;
        height(): number;
        toBuffer(magic:string = "PNG") : Buffer;
        fill(color:string) : Image;
        fill(r : number, g : number, b : number, a : number) : Image;
        crop(x : number, y : number, w : number, h : number) : Image;
        draw(img : Image, x : number, y : number, op : CompositeOperator = CompositeOperator.OverCompositeOp) : Image;
        resize(w : number, h : number) : Image;
        rotate(degree: number) : Image;
    }

    const enum CompositeOperator {
        UndefinedCompositeOp,
        AlphaCompositeOp,
        AtopCompositeOp,
        BlendCompositeOp,
        BlurCompositeOp,
        BumpmapCompositeOp,
        ChangeMaskCompositeOp,
        ClearCompositeOp,
        ColorBurnCompositeOp,
        ColorDodgeCompositeOp,
        ColorizeCompositeOp,
        CopyBlackCompositeOp,
        CopyBlueCompositeOp,
        CopyCompositeOp,
        CopyCyanCompositeOp,
        CopyGreenCompositeOp,
        CopyMagentaCompositeOp,
        CopyAlphaCompositeOp,
        CopyRedCompositeOp,
        CopyYellowCompositeOp,
        DarkenCompositeOp,
        DarkenIntensityCompositeOp,
        DifferenceCompositeOp,
        DisplaceCompositeOp,
        DissolveCompositeOp,
        DistortCompositeOp,
        DivideDstCompositeOp,
        DivideSrcCompositeOp,
        DstAtopCompositeOp,
        DstCompositeOp,
        DstInCompositeOp,
        DstOutCompositeOp,
        DstOverCompositeOp,
        ExclusionCompositeOp,
        HardLightCompositeOp,
        HardMixCompositeOp,
        HueCompositeOp,
        InCompositeOp,
        IntensityCompositeOp,
        LightenCompositeOp,
        LightenIntensityCompositeOp,
        LinearBurnCompositeOp,
        LinearDodgeCompositeOp,
        LinearLightCompositeOp,
        LuminizeCompositeOp,
        MathematicsCompositeOp,
        MinusDstCompositeOp,
        MinusSrcCompositeOp,
        ModulateCompositeOp,
        ModulusAddCompositeOp,
        ModulusSubtractCompositeOp,
        MultiplyCompositeOp,
        NoCompositeOp,
        OutCompositeOp,
        OverCompositeOp,
        OverlayCompositeOp,
        PegtopLightCompositeOp,
        PinLightCompositeOp,
        PlusCompositeOp,
        ReplaceCompositeOp,
        SaturateCompositeOp,
        ScreenCompositeOp,
        SoftLightCompositeOp,
        SrcAtopCompositeOp,
        SrcCompositeOp,
        SrcInCompositeOp,
        SrcOutCompositeOp,
        SrcOverCompositeOp,
        ThresholdCompositeOp,
        VividLightCompositeOp,
        XorCompositeOp,
        StereoCompositeOp
    }

}