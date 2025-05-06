package layaair.game.browser;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.Base64;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;


public class LayaCanvasRenderingContext2D {

    public static final String TAG = "LayaCanvasRenderingContext2D";
    public Paint paint = new Paint();
    public Bitmap bitmap;
    public Canvas canvas;
    public int fillColorR = 0;
    public int fillColorG = 0;
    public int fillColorB = 0;
    public int fillColorA = 0;;

    public int strokeColorR = 0;
    public int strokeColorG = 0;
    public int strokeColorB = 0;
    public int strokeColorA = 0;
    enum TextBaseline
    {
        Alphabetic,
        Top,
        Hanging,
        Middle,
        Ideographic,
        Bottom
    };
    public int width = 0;
    public int height = 0;
    public TextBaseline textBaseline = TextBaseline.Alphabetic;
    public int[] transparentPixels = null;
    float[] valuesTemp = new float[9];
    public static HashMap<String, Typeface> typefaceMap = new HashMap<String, Typeface>();
    public static LayaCanvasRenderingContext2D create(long ptr, int width, int height) {
        LayaCanvasRenderingContext2D context = new LayaCanvasRenderingContext2D(width, height);
        return context;
    }
    public LayaCanvasRenderingContext2D(int width, int height)
    {
        this.width = width;
        this.height = height;
        this.bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        this.canvas = new Canvas(this.bitmap);
        this.paint.setAntiAlias(true);
        this.paint.setSubpixelText(true);
        int pixelCount = width * height;
        this.transparentPixels = new int [pixelCount];
        Arrays.fill(this.transparentPixels, Color.TRANSPARENT);
    }
    @SuppressLint("LongLogTag")
    static float getTextY(TextBaseline textBaseline, float y, Paint paint) {
        switch (textBaseline)
        {
            case Alphabetic:
                break;
            case Top:
                return y - paint.getFontMetrics().ascent;
            case Hanging:
                break;
            case Middle:
                return y + (paint.getFontMetrics().descent - paint.getFontMetrics().ascent) * 0.5f - paint.getFontMetrics().descent;
            case Ideographic:
                break;
            case Bottom:
                return y - paint.getFontMetrics().descent;
            default:
                Log.e(TAG, "getTextY invalid textBaseline " + textBaseline);
                break;
        }
        return y;
    }
    static float[] measureText(LayaCanvasRenderingContext2D context, String text) {
        float width = context.paint.measureText(text);
        Paint.FontMetrics fontMetrics = context.paint.getFontMetrics();
        //float height = fontMetrics.bottom - fontMetrics.top;
        float height = fontMetrics.descent-fontMetrics.ascent;
        //Log.d("measureText ", (fontMetrics.bottom - fontMetrics.top) + " " + (fontMetrics.descent-fontMetrics.ascent)  + " " + fontMetrics.leading);
        float[] metrics = { width, height, fontMetrics.ascent};
        return metrics;
    }
    static void setLineWidth(LayaCanvasRenderingContext2D context, float lineWidth) {
        context.paint.setStrokeWidth(lineWidth);
    }
    static void setLineJoin(LayaCanvasRenderingContext2D context, int lineJoin) {
        switch (lineJoin)
        {
            case 0:
                context.paint.setStrokeJoin(Paint.Join.ROUND);
                break;
            case 1:
                context.paint.setStrokeJoin(Paint.Join.BEVEL);
                break;
            case 2:
                context.paint.setStrokeJoin(Paint.Join.MITER);
                break;
            default:
                Log.e(TAG, "setLineJoin invalid lineJoin " + lineJoin);
                break;
        }
    }
    static void fillText(LayaCanvasRenderingContext2D context, String text, float x, float y) {
        context.paint.setStyle(Paint.Style.FILL);
        context.paint.setARGB(context.fillColorA, context.fillColorR, context.fillColorG, context.fillColorB);
        context.canvas.drawText(text, x, getTextY(context.textBaseline, y, context.paint), context.paint);

    }
    static void strokeText(LayaCanvasRenderingContext2D context, String text, float x, float y) {
        context.paint.setStyle(Paint.Style.STROKE);
        context.paint.setARGB(context.strokeColorA, context.strokeColorR, context.strokeColorG, context.strokeColorB);
        context.canvas.drawText(text, x, getTextY(context.textBaseline, y, context.paint), context.paint);
    }
    static  byte[] getImageData(LayaCanvasRenderingContext2D context, int x, int y, int width, int height) {
        final byte[] pixels = new byte[width * height * 4];
        final ByteBuffer buf = ByteBuffer.wrap(pixels);
        buf.order(ByteOrder.nativeOrder());
        Bitmap destBitmap = Bitmap.createBitmap(context.bitmap, x, y, width, height);
        destBitmap.copyPixelsToBuffer(buf);
        return pixels;
    }
    static void save(LayaCanvasRenderingContext2D context) {
        context.canvas.save();
    }
    static void restore(LayaCanvasRenderingContext2D context) {
        context.canvas.restore();
    }
    static void setTransform(LayaCanvasRenderingContext2D context, float a, float b, float c, float d, float tx, float ty) {

        // [a  , c  , tx]
        // [b  , d  , ty]
        // [0.0, 0.0, 1.0]
        Matrix matrix = new Matrix();
        matrix.setValues(new float[]{a, c, tx,
                b, d, ty,
                0.0f,0.0f,1.0f});
        context.canvas.setMatrix(matrix);
    }
    static void scale(LayaCanvasRenderingContext2D context, float x, float y) {
        context.canvas.scale(x, y);
    }
    static void clearRect(LayaCanvasRenderingContext2D context, float x, float y, float width, float height) {
        Matrix mat = context.canvas.getMatrix();//todo deprecated
        mat.getValues(context.valuesTemp);
        float scaleX = context.valuesTemp[Matrix.MSCALE_X];
        float scaleY = context.valuesTemp[Matrix.MSCALE_Y];
        int w = (int)Math.ceil(width * scaleX);
        int h = (int)Math.ceil(height * scaleY);

        int ix = (int)Math.floor(x * scaleX);
        int iy = (int)Math.floor(y * scaleY);
        int maxW = Math.min(ix + w, context.width);
        int maxH = Math.min(iy + h, context.height);
        w = maxW - ix;
        h = maxH - iy;
        if (w <= 0 || h <= 0) {
            return;
        }
        context.bitmap.setPixels(context.transparentPixels, 0, context.width, ix, iy, w, h);
    }
    static void setStrokeColor(LayaCanvasRenderingContext2D context, int r, int g, int b, int a) {
        context.strokeColorR = r;
        context.strokeColorG = g;
        context.strokeColorB = b;
        context.strokeColorA = a;
    }
    static void setFillColor(LayaCanvasRenderingContext2D context, int r, int g, int b, int a) {
        context.fillColorR = r;
        context.fillColorG = g;
        context.fillColorB = b;
        context.fillColorA = a;
    }
    static void setTextSize(LayaCanvasRenderingContext2D context, float size) {
        context.paint.setTextSize(size);
    }
    static void setTypeface(LayaCanvasRenderingContext2D context, String fontName, int style) {
        if (LayaCanvasRenderingContext2D.typefaceMap.containsKey(fontName)) {
            Typeface typeface = LayaCanvasRenderingContext2D.typefaceMap.get(fontName);
            context.paint.setTypeface(Typeface.create(typeface, style));
        }
        else {
            context.paint.setTypeface(Typeface.create(fontName, style));
        }
    }
    static boolean registerFontFromPath(String fontName, String path) {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.DONUT) {
            Typeface newTypeFace = Typeface.createFromFile(path);
            typefaceMap.put(fontName, newTypeFace);
            return true;
        }
        return false;
    }
    @SuppressLint("LongLogTag")
    static void setTextAlign(LayaCanvasRenderingContext2D context, int textAlign) {
        switch (textAlign)
        {
            case 0:
                context.paint.setTextAlign(Paint.Align.LEFT);
                break;
            case 1:
                context.paint.setTextAlign(Paint.Align.CENTER);
                break;
            case 2:
                context.paint.setTextAlign(Paint.Align.RIGHT);
                break;
            default:
                Log.e(TAG, "setTextAlign invalid textAlign " + textAlign);
                break;
        }
    }
    @SuppressLint("LongLogTag")
    static void setTextBaseline(LayaCanvasRenderingContext2D context, int textBaseline) {
        switch (textBaseline)
        {
            case 0:
                context.textBaseline = TextBaseline.Alphabetic;
                break;
            case 1:
                context.textBaseline = TextBaseline.Top;
                break;
            case 2:
                context.textBaseline = TextBaseline.Hanging;
                break;
            case 3:
                context.textBaseline = TextBaseline.Middle;
                break;
            case 4:
                context.textBaseline = TextBaseline.Ideographic;
                break;
            case 5:
                context.textBaseline = TextBaseline.Bottom;
                break;
            default:
                Log.e(TAG, "setTextBaseline invalid textBaseline " + textBaseline);
                break;
        }
    }

}