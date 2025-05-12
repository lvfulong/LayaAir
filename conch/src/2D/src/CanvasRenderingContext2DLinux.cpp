#include "CanvasRenderingContext2DLinux.h"
#include <fontconfig/fontconfig.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <utils/Log.h>
#include <X11/Xft/Xft.h>
#include <algorithm>
#include <map>
#include <string>
#include <memory>
#include <iostream>
/*
使用了 cairo库，需要 apt-get install libcairo2-dev
*/
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <cairo/cairo-ft.h>

#include <fstream>
#include <sys/mman.h>

extern Display *g_X11_display;
extern Window g_X11_window;

std::map<std::string, std::string> privateFontMap;
FT_Library  gFTLibrary=nullptr;

//获得本进程的内存占用。单位是kb
long getMemoryUsage() {
    std::ifstream stat_stream("/proc/self/status");
    std::string line;
    while (std::getline(stat_stream, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            return std::stol(line.substr(6));
        }
    }
    return 0;
}

static cairo_user_data_key_t ft_face_key;
static void releaseface(void* data) {
    if(!data)
        return;
    FT_Face face = static_cast<FT_Face>(data);
    printf("delete %s\n", face->family_name);
    FT_Done_Face(face);
}

namespace laya
{

class xftTextRender
{
private:
    int     m_nWidth=0;
    int     m_nHeight=0;
    cairo_font_face_t* m_pCurCairoFont = nullptr;
    double m_FillColor[4]={1,1,1,1};
    double m_BorderColor[4]={1,1,1,1};
    int     m_nFontSize=16;
    cairo_surface_t* m_pCS=nullptr;
    cairo_t*        m_pCR = nullptr;
    std::map<std::string, std::string> font_map_;
public:
    xftTextRender(int width, int height)
    {
        init(width, height);
    }

    ~xftTextRender()
    {
        if(m_pCR)
            cairo_destroy(m_pCR);
        if(m_pCS)
            cairo_surface_destroy(m_pCS);

        // 注意：不要释放 m_pCurFont，因为它是由 X11FontManager 管理的

        if(m_pCurCairoFont){
            cairo_font_face_destroy(m_pCurCairoFont);
        }
    }    

    void init( int width, int height)
    {
        m_nWidth = width;
        m_nHeight = height;
        FcInit();
        //m_pCS = cairo_xlib_surface_create(m_pDisplay, m_pixMap, visual, m_nWidth, m_nHeight);
        m_pCS = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_nWidth, m_nHeight);
        m_pCR = cairo_create(m_pCS);
        cairo_set_line_join(m_pCR,CAIRO_LINE_JOIN_ROUND);
    }

    void InitializeFontMap()
    {
        font_map_["Arial"] = "Liberation Sans";
        font_map_["Times New Roman"] = "Liberation Serif";
        font_map_["Courier New"] = "Liberation Mono";
        font_map_["Verdana"] = "DejaVu Sans";
        font_map_["Georgia"] = "Nimbus Roman No9 L";
        font_map_["Calibri"] = "Carlito";
        font_map_["Cambria"] = "Caladea";
        // 可以继续添加更多映射...
    }
    void list_fonts(Display *dpy)
    {
        int screen = DefaultScreen(dpy);
        XftFontSet *fs = nullptr;
        FcPattern *pat = nullptr;
        FcObjectSet *os = nullptr;

        pat = FcPatternCreate();
        FcPatternAddString(pat, FC_LANG, (const FcChar8 *)"zh");
        os = FcObjectSetBuild(FC_FAMILY, FC_STYLE, NULL); // FC_LANG, NULL);
        fs = FcFontList(NULL, pat, os);

        // fs = XftListFonts(dpy, screen,XFT_FAMILY, XftTypeString, "Arial",(char*)NULL);

        for (int i = 0; i < fs->nfont; i++)
        {
            FcChar8 *family, *style;

            if (FcPatternGetString(fs->fonts[i], FC_FAMILY, 0, &family) == FcResultMatch &&
                FcPatternGetString(fs->fonts[i], FC_STYLE, 0, &style) == FcResultMatch)
            {
                printf("%s - %s\n", family, style);
            }
        }

        FcObjectSetDestroy(os);
        FcPatternDestroy(pat);
        XftFontSetDestroy(fs);
    }

    void setFont(const std::string &fontName, int size, bool bold)
    {
        // printf("set font :%s %d\n", fontName.c_str(),size);
        m_nFontSize = size;
        if (!gFTLibrary)
            FT_Init_FreeType(&gFTLibrary);
        FT_Face ft_face = nullptr;

        FT_Int32 load_flags = FT_LOAD_DEFAULT;

        // 先看是不是自定义字体
        auto it = privateFontMap.find(fontName);
        if (it != privateFontMap.end())
        {
            // 加载自定义字体
            int fid = 0;
            if (bold)
                fid = 1;
            FT_New_Face(gFTLibrary, it->second.c_str(), fid, &ft_face);
            if(!ft_face && bold){
                //加载粗体失败，回退
                FT_New_Face(gFTLibrary, it->second.c_str(), 0, &ft_face);
            }
        }

        // 如果没有找到自定义字体，尝试从系统加载
        if (ft_face == nullptr)
        {
            FcPattern *pattern = FcNameParse((const FcChar8 *)fontName.c_str());//"Arial:Bold:size=12"
            if(bold)
                FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            FcConfigSubstitute(NULL, pattern, FcMatchPattern);
            FcDefaultSubstitute(pattern);

            FcResult result;
            FcPattern *match = FcFontMatch(NULL, pattern, &result);

            if (match)
            {
                FcChar8 *file = NULL;
                if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch)
                {
                    FT_New_Face(gFTLibrary, (const char *)file, 0, &ft_face);
                }
                FcPatternDestroy(match);
            }
        }
        // 如果还是没有找到字体，加载默认字体
        if (ft_face == nullptr)
        {
            // 这里使用一个系统默认字体，比如 "Sans"
            FcPattern *pattern = FcNameParse((const FcChar8 *)"Sans");
            if(bold)
                FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            FcConfigSubstitute(NULL, pattern, FcMatchPattern);
            FcDefaultSubstitute(pattern);

            FcResult result;
            FcPattern *match = FcFontMatch(NULL, pattern, &result);

            if (match)
            {
                FcChar8 *file = NULL;
                if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch)
                {
                    FT_New_Face(gFTLibrary, (const char *)file, 0, &ft_face);
                }
                FcPatternDestroy(match);
            }

            FcPatternDestroy(pattern);
        }

        //检查加载粗体是否成功
        if(bold){
            if (ft_face->style_flags & FT_STYLE_FLAG_BOLD) {
                //加载粗体成功
            }else{
                //如果不成功
                if (ft_face->face_flags & FT_FACE_FLAG_SCALABLE) {
                    // 对于可缩放字体,使用 FreeType 的合成粗体
                    //TODO 
                    // load_flags |= FT_LOAD_FORCE_AUTOHINT;
                    // FT_Set_Char_Size(ft_face, 0, m_nFontSize * 64, 0, 0);
                    // FT_Set_Transform(ft_face, NULL, NULL);
                    // FT_Set_Charmap(ft_face, ft_face->charmaps[0]);
                    // FT_Set_Pixel_Sizes(ft_face, 0, m_nFontSize);
                }                
            }
        }

        cairo_font_face_t* font = nullptr;
        if(ft_face){
            font = cairo_ft_font_face_create_for_ft_face(ft_face, load_flags);
            cairo_font_face_set_user_data(font, &ft_face_key, ft_face, releaseface);
        }

        if(m_pCurCairoFont){
            cairo_font_face_destroy(m_pCurCairoFont);
        }

        m_pCurCairoFont = font;
        cairo_set_font_face(m_pCR, m_pCurCairoFont);
        cairo_set_font_size(m_pCR,m_nFontSize);

        //printf("cur font = %s %d\n", ft_face->family_name, (int)ft_face->height);

        //cairo_set_font_size(m_pCR,m_nFontSize);
        //cairo_font_extents_t font_extents;        
        //cairo_font_extents(m_pCR, &font_extents);
    }

    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
    {
        m_FillColor[2]=r/255.0;
        m_FillColor[1]=g/255.0;
        m_FillColor[0]=b/255.0;
    }

    void setStrokeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255){
        m_BorderColor[2]=r/255.0;
        m_BorderColor[1]=g/255.0;
        m_BorderColor[0]=b/255.0;
    }

    void scale(double x, double y){
        cairo_scale(m_pCR, x, y);
    }

    void setTransform(double a, double b, double c, double d, double e, double f)
    {
        cairo_matrix_t matrix;
        cairo_matrix_init(&matrix, a, b, c, d, e, f);
        cairo_set_matrix(m_pCR, &matrix);
    }

    void measure(const char* ptext,int& width, int& height,int& ascent, int& descent, int& advance){
        //cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_text_extents_t extents;
        cairo_text_extents(m_pCR, ptext, &extents);        

        cairo_font_extents_t font_extents;        
        cairo_font_extents(m_pCR, &font_extents);
        ascent = font_extents.ascent;
        descent = font_extents.descent;

        width  = extents.width+extents.x_bearing;
        height = extents.height;
        advance = extents.x_advance;

    }

    void clearRect(double x, double y, double width, double height){
        if(width<=0||height<=0)
            return;

        // 设置背景
        //cairo_set_source_rgba(m_pCR, 0,0,0,1);
        cairo_set_operator(m_pCR, CAIRO_OPERATOR_CLEAR);
        cairo_paint(m_pCR);
        // 恢复到默认操作符
        cairo_set_operator(m_pCR, CAIRO_OPERATOR_OVER);
    }

    //在xy位置画文字。
    //y是根据对齐方式希望的位置
    //cairo使用的是baseline，所以需要转换到baseline上
    void drawString(const std::string& str, int x, int y)
    {
        //strokeString(str,x,y,1,0);
        //return;
        cairo_t *cr = m_pCR;

    // cairo_set_source_rgb(cr, 1, 0, 1);
    // cairo_set_line_width(cr, 1);
    // cairo_move_to(cr,x,y);
    // cairo_line_to(cr,x+1500,y);
    // cairo_stroke(cr);
    // cairo_font_extents_t font_extents;        
    // cairo_font_extents(m_pCR, &font_extents);
    // auto ascent = font_extents.ascent;
    // auto descent = font_extents.descent;
    // cairo_text_extents_t extents;
    // cairo_text_extents(m_pCR, str.c_str(), &extents);       

    // cairo_set_line_width(cr, 2);  // 设置线宽
    // cairo_set_source_rgb(cr, 0, 0, 0);  // 设置颜色（黑色）
    // auto stx = x+extents.x_bearing;
    // auto sty = y+extents.y_bearing;
    // cairo_rectangle(cr, stx, sty, extents.width, extents.height); 
    // cairo_stroke(cr);
        //debug

        //cairo_font_face_destroy(font_face);
        //XftUnlockFace(m_pCurFont);

        // 设置字体和颜色
        //FT_Face face = XftLockFace(m_pCurFont);
        //cairo_font_face_t *font_face = cairo_ft_font_face_create_for_ft_face(face, 0);
        //cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_source_rgb(cr, m_FillColor[0], m_FillColor[1], m_FillColor[2]);

        cairo_move_to(cr, x, y);
        cairo_show_text(cr,str.c_str());

        return;
    }

    void strokeString(const std::string &str, int sx, int sy, int lineWidth, float r, float g, float b)
    {
        cairo_t *cr = m_pCR;

        // 设置背景
        //cairo_set_source_rgb(cr, r, g, b);
        //cairo_paint(cr);

        // 设置字体和颜色
        //FT_Face face = XftLockFace(m_pCurFont);
        //cairo_font_face_t *font_face = cairo_ft_font_face_create_for_ft_face(face, 0);  //会把大小信息给cairo font
        //cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_source_rgb(cr, m_BorderColor[0], m_BorderColor[1], m_BorderColor[2]);

        cairo_move_to(cr, sx, sy);
        cairo_text_path(cr, str.c_str());
        cairo_set_line_width(cr, lineWidth);
        cairo_stroke(cr);

        //cairo_font_face_destroy(font_face);
        //XftUnlockFace(m_pCurFont);
    }

    ImageData getBmp(int x, int y, int width, int height){
        //debug
        // auto cr = m_pCR;
        // cairo_set_line_width(cr, 2);  // 设置线宽
        // cairo_set_source_rgb(cr, 0, 0, 0);  // 设置颜色（黑色）
        // cairo_rectangle(cr, x, y, x+width, y+height); 
        // cairo_stroke(cr);
        //debug

        int stride = cairo_image_surface_get_stride(m_pCS);
        unsigned char *data = cairo_image_surface_get_data(m_pCS)+y*stride+x*4;
        if(width<=0||height<=0)
            return ImageData();

        ImageData imgdata;
        imgdata.m_width = width;
        imgdata.m_height = height;
        imgdata.m_data.resize(width * height * 4);
        unsigned int *glImageData = (unsigned int *)&imgdata.m_data[0];

        unsigned int *cline = (unsigned int*)data;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned int cc = cline[x];
                float alpha = (cc>>24)/255.0f;
                if(alpha<0.001f)alpha=1.0f;
                //边缘发黑，反向预乘一下
                float fc0 = cc&0xff;
                float fc1 = (cc>>8)&0xff;
                float fc2 = (cc>>16)&0xff;
                fc0/=alpha;
                fc1/=alpha;
                fc2/=alpha;
                int c0 = fc0; if(c0>255)c0=255;
                int c1 = fc1; if(c1>255)c1=255;
                int c2 = fc2; if(c2>255)c2=255;

                cc = (cc&0xff000000)|(c0)|(c1<<8)|(c2<<16);

                //unsigned int nAlpha = cc>>24;
                //cc = 0xff000000|(nAlpha)|(nAlpha<<8)|(nAlpha<<16);
                *(glImageData + y * width + x) = cc;
                //*((int *)glImageData + y * width + x) = (*(((int*)cline)+x)) | (0xff << 24);
            }
            cline += stride/4;
        }

        return imgdata;
    }
};

CanvasRenderingContext2DLinux::CanvasRenderingContext2DLinux(int width, int height)
    : CanvasRenderingContext2D(width, height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    m_bitmapData.m_nWidth = width;
    m_bitmapData.m_nHeight = height;
    m_bitmapData.m_pImageData = new char[width * height * 4];

    m_pTextRender = new xftTextRender( width, height);
    setDefault();
}
CanvasRenderingContext2DLinux::~CanvasRenderingContext2DLinux()
{
    if(m_pTextRender){
        delete m_pTextRender;
    }
}

void CanvasRenderingContext2DLinux::setLineWidth(double lineWidth)
{
    CanvasRenderingContext2D::setLineWidth(lineWidth);
}
void CanvasRenderingContext2DLinux::setLineJoin(const char *lineJoin)
{
}
void CanvasRenderingContext2DLinux::fillText(const std::string &text, double x, double y,
                                             std::optional<double> maxWidth)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }

    // 使用 Xft 在 pixmap 上绘制文本
    //XftDrawString8(xftDraw, &color, font, x, y, (XftChar8 *)text, strlen(text));    

    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    m_pTextRender->setColor(m_fillColorR, m_fillColorG, m_fillColorB, m_fillColorA);
    m_pTextRender->drawString(text,outX,outY);
}

void CanvasRenderingContext2DLinux::strokeText(const std::string &text, double x, double y,
                                               std::optional<double> maxWidth)
{
    if(m_lineWidth<=0)
        return;
    double outX;
    double outY;
    getTextPosition(text, x, y, outX, outY);
    //m_pTextRender->setColor(m_fillColorR, m_fillColorG, m_fillColorB, m_fillColorA);
    m_pTextRender->setStrokeColor(m_strokeColorR, m_strokeColorG, m_strokeColorB, 255);
    m_pTextRender->strokeString(text,outX,outY,m_lineWidth,0.0,1.0,0.0);
    //m_pTextRender->drawString(text,outX,outY);
}

TextMetrics CanvasRenderingContext2DLinux::measureText(const std::string &text)
{
    TextMetrics metrics;
    int ascent = 0;
    int descent = 0;
    int direction = 0;
    int width = 0;
    int height = 0;
    int advance = 0;
    m_pTextRender->measure(text.c_str(),width,height,ascent,descent,advance);//TODO
    width+=m_lineWidth;
    metrics.m_ascender = ascent;
    metrics.m_descender = descent;
    metrics.m_width = width;
    metrics.m_height = height;

    return metrics;
}
void CanvasRenderingContext2DLinux::clearRect(double x, double y, double width, double height)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    m_pTextRender->clearRect(x,y,width,height);
}
void CanvasRenderingContext2DLinux::save()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
void CanvasRenderingContext2DLinux::restore()
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
}
ImageData CanvasRenderingContext2DLinux::getImageData(double x, double y, double width, double height)
{
    //不能直接显示宽高，要限制终点，否则xy为负的就不对了
    int ex = (int)(x+width);
    int ey = (int)(y+height);

    int clampedX = std::clamp(x, 0.0, static_cast<double>(m_width));
    int clampedY = std::clamp(y, 0.0, static_cast<double>(m_height));
    int clampedEX = std::clamp(ex, 0, m_width);
    int clampedEY = std::clamp(ey, 0, m_height);
    
    return m_pTextRender->getBmp(clampedX, clampedY, clampedEX-clampedX, clampedEY-clampedY);
}
const BitmapData &CanvasRenderingContext2DLinux::getBitmapData() const
{
    return m_bitmapData;
}
void CanvasRenderingContext2DLinux::setTransform(double a, double b, double c, double d, double e, double f)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    m_pTextRender->setTransform(a,b,c,d,e,f);
}
void CanvasRenderingContext2DLinux::scale(double x, double y)
{
    if (m_width <= 0 || m_height <= 0)
    {
        return;
    }
    m_pTextRender->scale(x,y);
}
void CanvasRenderingContext2DLinux::setFont(const char *font)
{
    CanvasRenderingContext2D::setFont(font);
    m_pTextRender->setFont(m_fontProperties.fontFamily[0].c_str(), (int)m_fontProperties.fontSize, m_fontProperties.isBold()); 
}
void CanvasRenderingContext2DLinux::getTextPosition(const std::string &text, double x, double y, double &outX,
                                                    double &outY)
{
    outX = x;
    outY = y;

    int ascent = 0;
    int descent = 0;
    int width = 0;
    int height = 0;
    int advance = 0;
    m_pTextRender->measure(text.c_str(),width,height,ascent,descent,advance);//TODO

    if (m_textAlign == TextAlign::Center)
    {
        outX = x - width / 2.0f;
    }
    else if (m_textAlign == TextAlign::Left)
    {
    }
    else if (m_textAlign == TextAlign::Right)
    {
        outX = x - width;
    }

    if (m_textBaseline == TextBaseline::Top)
    {
        outY = y + ascent;
    }
    else if (m_textBaseline == TextBaseline::Middle)
    {
        outY = y + (ascent - (ascent+descent)/2);
    }
    else if (m_textBaseline == TextBaseline::Bottom)
    {
        outY = y - descent;
    }
    else if (m_textBaseline == TextBaseline::Alphabetic)
    {
        outY = y;
    }
}
bool CanvasRenderingContext2DLinux::registerFontFromPath(const std::string &fontName, const std::string &path)
{
    if(!gFTLibrary)
        FT_Init_FreeType(&gFTLibrary);

    // 加载字体文件。检查文件是否存在
    FT_Face face;
    if (FT_New_Face(gFTLibrary, path.c_str(), 0, &face) != 0) {
        LOGE("Failed to load font: %s", path.c_str());
        return false;
    }
    FT_Done_Face(face);
    privateFontMap[fontName] = path;
    return true;

    // 将字体添加到FontConfig
    //FcConfig* config = FcInitLoadConfigAndFonts();
    // FcConfig* config = FcConfigGetCurrent();
    // FcPattern* pattern = FcPatternCreate();
    // if(!pattern)
    //     return false;
    // FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)fontName.c_str());
    // FcPatternAddString(pattern, FC_FILE, (const FcChar8*)absPath.c_str());
    // FcConfigSubstitute(config, pattern, FcMatchPattern);
    // FcDefaultSubstitute(pattern);

    // // 将 pattern 添加到配置中
    // // Bind the pattern to font set
    // FcFontSet *fontset = FcFontSetCreate();
    // FcFontSetAdd(fontset, pattern);

    // //销毁本地的pattern（不会影响对fontconfig的配置）
    // FcPatternDestroy(pattern);
    // FcFontSetDestroy(fontset);

    // // 注意：FcConfigAddPattern 会创建 pattern 的一个副本
    // if (!FcConfigAddFont(config, pattern)) {
    //     FcPatternDestroy(pattern);
    //     return false;
    // }    

    // //添加字体文件到fontconfig中
    // if(!FcConfigAppFontAddFile(config, (const FcChar8*)absPath.c_str())){
    //     FcPatternDestroy(pattern);
    //     return false;
    // }

    // //FcConfigDestroy(config);
    // return true;
}

bool CanvasRenderingContext2DLinux::registerFontFromBuffer(const std::string& fontName, const uint8_t* buff, int len){
    if(!gFTLibrary)
        FT_Init_FreeType(&gFTLibrary);

    //FT_Face face;
    //FT_New_Memory_Face(gFTLibrary, buff, len, 0, &face);
    //privateFontMap[fontName] = face;
    // 创建内存文件
    int fd = memfd_create(fontName.c_str(), 0);
    if (fd == -1) {
        std::cerr << "Error creating memfd" << std::endl;
        return false;
    }    

    // 写入字体数据
    if (write(fd, buff, len) != len) {
        std::cerr << "Error writing to memfd" << std::endl;
        close(fd);
        return false;
    }    

    // 创建文件描述符的路径
    char fd_path[64];
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    
    return CanvasRenderingContext2DLinux::registerFontFromPath(fontName, std::string(fd_path));
}

//TODO 在哪里调用这个释放
void CanvasRenderingContext2DLinux::destroy(){
    // auto it = privateFontMap.begin();
    // for(;it!=privateFontMap.end(); it++){
    //     FT_Done_Face(it->second);
    // }
    FT_Done_FreeType(gFTLibrary);
    gFTLibrary=nullptr;
}

} // namespace laya
