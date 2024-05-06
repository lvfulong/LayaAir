#include "CanvasRenderingContext2DCG.h"
#include "utils/ColorParser.h"
#import "Utils.h"
#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIFont.h>
#import <UIKit/UIKit.h>
#include <utils/JCCommonMethod.h>
#include <utils/JCCrypto.h>
#include <utils/Log.h>
#include "FontManager.h"

namespace laya
{

class CanvasRenderingContext2DCGImpl
{
  public:
    CanvasRenderingContext2DCGImpl(int width, int height) : m_width(width), m_height(height)
    {
        if (width > 0 && height > 0)
        {
            create(width, height);
        }
    }
    ~CanvasRenderingContext2DCGImpl()
    {
        if (m_context)
        {
            CGContextRelease(m_context);
            m_context = nullptr;
        }
        if (m_bitmapData)
        {
            delete m_bitmapData;
            m_bitmapData = nullptr;
        }
    }
    void create(int width, int height)
    {
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        m_bitmapData = new BitmapData(new char[width * height * 4], width, height);
        m_context = CGBitmapContextCreate(m_bitmapData->m_pImageData, width, height, 8, width * 4, colorSpace,
                                          kCGImageAlphaPremultipliedLast);

        CGColorSpaceRelease(colorSpace);

        // Font smoothing
        CGContextSetShouldAntialias(m_context, YES);
        CGContextSetShouldSmoothFonts(m_context, YES);
        CGContextSetShouldSubpixelPositionFonts(m_context, YES);
        CGContextSetShouldSubpixelQuantizeFonts(m_context, YES);

        CGContextSetTextMatrix(m_context, CGAffineTransformIdentity);
        CGContextTranslateCTM(m_context, 0.0f, height);
        CGContextScaleCTM(m_context, 1.0f, -1.0f);
    }
    CGContextRef m_context = nullptr;
    BitmapData *m_bitmapData = nullptr;
    UIFont *m_UIFont;
    NSMutableDictionary *_attributesDict;
    uint32_t m_width;
    uint32_t m_height;
};
CanvasRenderingContext2DCG::CanvasRenderingContext2DCG(int width, int height)
    : CanvasRenderingContext2D(width, height), m_impl(new CanvasRenderingContext2DCGImpl(width, height))
{
    setDefault();
}
CanvasRenderingContext2DCG::~CanvasRenderingContext2DCG()
{
    if (m_impl != nullptr)
    {
        delete m_impl;
    }
}
void CanvasRenderingContext2DCG::fillText(const std::string &text, double x, double y, std::optional<double> maxWidth)
{
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);

    NSString *nsText = [NSString stringWithUTF8String:text.c_str()];

    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;
    [m_impl->_attributesDict setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    [m_impl->_attributesDict setObject:[UIColor colorWithRed:m_fillColorR / 255.0f
                                                       green:m_fillColorG / 255.0f
                                                        blue:m_fillColorB / 255.0f
                                                       alpha:m_fillColorA / 255.0f]
                                forKey:NSForegroundColorAttributeName];

    save();
    CGContextSetTextDrawingMode(m_impl->m_context, kCGTextFill);
    CGContextSetRGBFillColor(m_impl->m_context, m_fillColorR / 255.0f, m_fillColorG / 255.0f, m_fillColorB / 255.0f, m_fillColorA / 255.0f);
    CGContextSetLineWidth(m_impl->m_context, m_lineWidth);
    if (nsText != nil)
    {
        NSAttributedString *attributedString =
        [[NSAttributedString alloc] initWithString:nsText attributes:m_impl->_attributesDict];
        
        [attributedString drawAtPoint:CGPointMake(outX, outY)];
    }
    restore();
   
}

void CanvasRenderingContext2DCG::strokeText(const std::string &text, double x, double y, std::optional<double> maxWidth)
{
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);

    NSString *nsText = [NSString stringWithUTF8String:text.c_str()];

    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;
    //[m_impl->_attributesDict setObject:[NSNumber numberWithDouble:m_lineWidth] forKey:NSStrokeWidthAttributeName];
    [m_impl->_attributesDict setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    [m_impl->_attributesDict setObject:[UIColor colorWithRed:m_strokeColorR / 255.0f
                                                       green:m_strokeColorG / 255.0f
                                                        blue:m_strokeColorB / 255.0f
                                                       alpha:m_strokeColorA / 255.0f]
                                forKey:NSStrokeColorAttributeName];
    [m_impl->_attributesDict removeObjectForKey:NSForegroundColorAttributeName];

    save();
    CGContextSetTextDrawingMode(m_impl->m_context, kCGTextStroke);
    CGContextSetRGBFillColor(m_impl->m_context, m_fillColorR / 255.0f, m_fillColorG / 255.0f, m_fillColorB / 255.0f, m_fillColorA / 255.0f);
    CGContextSetRGBStrokeColor(m_impl->m_context, m_strokeColorR / 255.0f, m_strokeColorG / 255.0f, m_strokeColorB / 255.0f, m_strokeColorA / 255.0f);
    CGContextSetLineWidth(m_impl->m_context, m_lineWidth);
    switch (m_lineJoin) {
        case LineJoin::Round:
            CGContextSetLineJoin(m_impl->m_context, kCGLineJoinRound);
            break;
        case LineJoin::Bevel:
            CGContextSetLineJoin(m_impl->m_context, kCGLineJoinBevel);
            break;
        case LineJoin::Miter:
            CGContextSetLineJoin(m_impl->m_context, kCGLineJoinMiter);
            break;
        default:
            break;
    }
    if (nsText != nil)
    {
        NSAttributedString *attributedString =
        [[NSAttributedString alloc] initWithString:nsText attributes:m_impl->_attributesDict];
        
        [attributedString drawAtPoint:CGPointMake(outX, outY)];
    }
    restore();
}
TextMetrics CanvasRenderingContext2DCG::measureText(const std::string &text)
{

    NSString *nsText = [NSString stringWithUTF8String:text.c_str()];
    CGSize textRect = CGSizeZero;
    textRect.width = CGFLOAT_MAX;
    textRect.height = CGFLOAT_MAX;

    CGSize dim = [nsText boundingRectWithSize:textRect
                                      options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                   attributes:m_impl->_attributesDict
                                      context:nil]
                     .size;
    TextMetrics metrics;

    metrics.m_width = ceilf(dim.width);
    //metrics.m_height = ceilf(dim.height);
    //NSLog(@"measureText [ascender %f] [descender %f] [leading %f]",m_impl->m_UIFont.ascender,m_impl->m_UIFont.descender,m_impl->m_UIFont.leading);
    //NSLog(@"measureText [dim.height %f] [a-d+l%f] [lineHeight %f]",ceilf(dim.height),m_impl->m_UIFont.ascender - m_impl->m_UIFont.descender + m_impl->m_UIFont.leading, m_impl->m_UIFont.lineHeight);
    metrics.m_height = m_impl->m_UIFont.lineHeight;// lineHeight == m_impl->m_UIFont.ascender - m_impl->m_UIFont.descender
    metrics.m_ascender = m_impl->m_UIFont.ascender;
    metrics.m_descender = -m_impl->m_UIFont.descender;
    return metrics;
}
void CanvasRenderingContext2DCG::clearRect(double x, double y, double width, double height)
{
    CGRect rect = CGRectMake(x, y, width, height);
    // save();
    //  CGContextSetBlendMode(x->m_context, kCGBlendModeClear);
    //  CGContextFillRect(m_impl->m_context, rect);

    // CGContextFillRect(m_impl->m_context, CGRectMake(0, 0, m_impl->m_width, m_impl->m_height));
    memset(m_impl->m_bitmapData->m_pImageData, 0, m_impl->m_bitmapData->m_nWidth * m_impl->m_bitmapData->m_nHeight * 4);
    // restore();
}
void CanvasRenderingContext2DCG::save()
{
    UIGraphicsPushContext(m_impl->m_context);
    CGContextSaveGState(m_impl->m_context);
}
void CanvasRenderingContext2DCG::restore()
{
    CGContextRestoreGState(m_impl->m_context);
    UIGraphicsPopContext();
}
const BitmapData &CanvasRenderingContext2DCG::getBitmapData() const
{
    return *m_impl->m_bitmapData;
}
ImageData CanvasRenderingContext2DCG::getImageData(double x, double y, double width, double height)
{
    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedW = std::clamp(width, 0.0, static_cast<double>(m_width));
    int clampedH = std::clamp(height, 0.0, static_cast<double>(m_height));

    if (clampedW > 0 && clampedH > 0)
    {
        ImageData data;
        data.m_width = clampedW;
        data.m_height = clampedH;
        data.m_data.resize(clampedW * clampedH * 4);
        int start = 0;
        int bytePerLineDes = clampedW * 4;
        int bytePerLineSrc = m_width * 4;
        int startI = clampedH + clampedY - 1;
        // for (int i = startI; i >= clampedY; i--)
        for (int i = clampedY; i < clampedH + clampedY; i++)
        {
            std::memcpy(static_cast<void *>(&data.m_data[start]),
                        &m_impl->m_bitmapData->m_pImageData[clampedX * 4 + i * bytePerLineSrc], bytePerLineDes);
            start += bytePerLineDes;
        }

        return data;
    }
    return ImageData();
}
void CanvasRenderingContext2DCG::setTransform(double a, double b, double c, double d, double tx, double ty)
{
    CGAffineTransform currentMatrix = CGContextGetCTM(m_impl->m_context);
    CGContextConcatCTM(m_impl->m_context, CGAffineTransformInvert(currentMatrix));
    CGContextSetTextMatrix(m_impl->m_context, CGAffineTransformIdentity);
    CGContextTranslateCTM(m_impl->m_context, 0.0f, m_impl->m_height);
    CGContextScaleCTM(m_impl->m_context, 1.0f, -1.0f);
    CGContextConcatCTM(m_impl->m_context, CGAffineTransformMake(a, b, c, d, tx, ty));
}
void CanvasRenderingContext2DCG::scale(double x, double y)
{
    CGContextScaleCTM(m_impl->m_context, x, y);
}
void CanvasRenderingContext2DCG::chooseFont(const std::string& strFontName)
{
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();
    NSString *fontName = [NSString stringWithUTF8String:strFontName.c_str()];
    /*if (isBold && isItalic)
    {
        m_impl->m_UIFont =
        [UIFont fontWithName:[fontName stringByAppendingString:@"-BoldItalic"] size:m_fontDescription.m_size];
    }
    else if (isItalic)
    {
        m_impl->m_UIFont =
        [UIFont fontWithName:[fontName stringByAppendingString:@"-Italic"] size:m_fontDescription.m_size];
    }
    else*/ if (isBold)
    {
        m_impl->m_UIFont =
        [UIFont fontWithName:[fontName stringByAppendingString:@"-Bold"] size:m_fontDescription.m_size];
    }
    else
    {
        m_impl->m_UIFont = [UIFont fontWithName:fontName size:m_fontDescription.m_size];
    }
}


void CanvasRenderingContext2DCG::setFont(const char *font)
{
    CanvasRenderingContext2D::setFont(font);
    m_impl->m_UIFont = nil;
    bool isBold = m_fontDescription.isBold();
    bool isItalic = m_fontDescription.isItalic();
    
    auto pair = FontManager::getInstance()->getRealFontName(m_fontDescription.m_family);
    
    if (pair.first) {
        chooseFont(pair.second);
    }
    if (m_impl->m_UIFont == nil)
    {
        chooseFont(m_fontDescription.m_family);
    }

    if (m_impl->m_UIFont == nil)
    {
        /*if (isBold && isBold)
        {
            
        }
        else */if (isBold)
        {
            m_impl->m_UIFont = [UIFont boldSystemFontOfSize:m_fontDescription.m_size];
        }
        /*else if (isItalic)
        {
            m_impl->m_UIFont = [UIFont italicSystemFontOfSize:m_fontDescription.m_size];
        }*/
        else
        {
            m_impl->m_UIFont = [UIFont systemFontOfSize:m_fontDescription.m_size];
        }
    }
    assert (m_impl->m_UIFont != nil);
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineBreakMode = NSLineBreakByTruncatingTail;
    [paragraphStyle setAlignment:NSTextAlignmentCenter];
    
    m_impl->_attributesDict = [[NSMutableDictionary alloc]  init];
    [m_impl->_attributesDict setObject:m_impl->m_UIFont forKey: NSFontAttributeName];
    [m_impl->_attributesDict setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    
    if (isItalic)
    {
        [m_impl->_attributesDict setObject:@(0.3f) forKey: NSObliquenessAttributeName];
    }
}
void CanvasRenderingContext2DCG::getTextPosition(const std::string &text, double x, double y, double &outX, double &outY)
{
    TextMetrics textMetrics = measureText(text);
    outX = x;
    outY = y;
    if (m_textAlign == TextAlign::Center)
    {
        outX = x - textMetrics.m_width / 2.0f;
    }
    else if (m_textAlign == TextAlign::Left)
    {
    }
    else if (m_textAlign == TextAlign::Right)
    {
        outX = x - textMetrics.m_width;
    }

    if (m_textBaseline == TextBaseline::Top)
    {
        LOGI("");
    }
    else if (m_textBaseline == TextBaseline::Middle)
    {
        outY = y - (textMetrics.m_descender + textMetrics.m_ascender) * 0.5f;
        //outY = y - textMetrics.m_height / 2.0f;
    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {
        //outY = y - textMetrics.m_height;
        outY = y - (textMetrics.m_descender + textMetrics.m_ascender);
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        outY = y - textMetrics.m_ascender;
    }
}
std::vector<std::string> getAllSystemFontsIOS()
{
    std::vector<std::string> vec;
    NSArray* _fontFamilyArray=[UIFont familyNames];
    NSMutableArray* _fontArray=[NSMutableArray array];
    for(NSString* familyName in _fontFamilyArray) {
        vec.emplace_back(std::string([familyName UTF8String]));
    }   
}
} // namespace laya
