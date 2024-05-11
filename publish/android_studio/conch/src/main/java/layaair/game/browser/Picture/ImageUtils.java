package layaair.game.browser.Picture;

import android.Manifest;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import androidx.core.app.ActivityCompat;
import android.util.Log;
import android.widget.Toast;

import com.google.gson.Gson;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import androidx.exifinterface.media.ExifInterface;
import layaair.game.browser.ConchJNI;
import layaair.game.browser.Picture.bean.CallbackRes;
import layaair.game.conch.LayaConch5;
import layaair.game.utility.Constants;
import layaair.game.utility.Utils;

public class ImageUtils {

    private static final String TAG = "ImageUtils";
    private static Uri takePictureUri;


    public static Uri createImagePathUri(Activity activity) {
        String displayName = System.currentTimeMillis() + ".jpg";
        ContentValues values = new ContentValues();
        values.put(MediaStore.Images.Media.DISPLAY_NAME, displayName);
        String type = getMimeType(displayName);
        if (type != null && !type.equals("")) {
            values.put(MediaStore.Images.Media.MIME_TYPE, type);
        }
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) { //SD 卡是否可用，可用则用 SD 卡，否则用内部存储
            takePictureUri = activity.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
        } else {
            takePictureUri = activity.getContentResolver().insert(MediaStore.Images.Media.INTERNAL_CONTENT_URI, values);
        }

        return takePictureUri;
    }

    public static String getMimeType(String fileName) {
        String suffix = Utils.getExtension(new File(fileName).getPath());
        if (suffix.contains("jpg")) {
            return "image/jpg";
        } else if (suffix.contains("jpeg")) {
            return "image/jpeg";
        } else if (suffix.contains("png")) {
            return "image/png";
        } else {
            return "image/jpg";
        }
    }

    private static String getImagePath(Context context, Uri uri) {
        if (uri == null || context == null) {
            return null;
        }
        Cursor cursor = null;
        try {
            String[] proj = {MediaStore.Images.Media.DATA};
            cursor = context.getContentResolver().query(uri, proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            if (cursor != null) {
                cursor.close();
            }
        }
    }

    private static int getSampleSize(int width, int height) {
        int longestSide = Math.max(width, height);
        int result = 1;
        while (longestSide >= 2048) {
            longestSide = longestSide / 2;
            result = result * 2;
        }
        return result;
    }

    public static void compressImage(String inputImagePath, String outputImagePath) {
        BitmapFactory.Options options = new BitmapFactory.Options();
        //设置此参数是仅仅读取图片的宽高到options中，不会将整张图片读到内存中
        options.inJustDecodeBounds = true;
        Bitmap emptyBitmap = BitmapFactory.decodeFile(inputImagePath, options);
        options.inJustDecodeBounds = false;
        int picWidth = options.outWidth;
        int picHeight = options.outHeight;
        String mimeType = options.outMimeType;
        Log.d(TAG, "onActivityResult: picWidth " + picWidth);
        Log.d(TAG, "onActivityResult: picHeight " + picHeight);
        Log.d(TAG, "onActivityResult: mimeType " + mimeType);

        Bitmap photoBitmap = null;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        int orientation = getImageOrientation(inputImagePath);
        Log.d(TAG, "onActivityResult: orientation 90 " + (orientation == ExifInterface.ORIENTATION_ROTATE_90));
        Log.d(TAG, "onActivityResult: orientation 180 " + (orientation == ExifInterface.ORIENTATION_ROTATE_180));
        Log.d(TAG, "onActivityResult: orientation 270 " + (orientation == ExifInterface.ORIENTATION_ROTATE_270));
        options.inSampleSize = getSampleSize(picWidth, picHeight);
        Log.d(TAG, "onActivityResult: inSampleSize " + options.inSampleSize);
        photoBitmap = BitmapFactory.decodeFile(inputImagePath, options);
        if (orientation == ExifInterface.ORIENTATION_ROTATE_90 ) {
            photoBitmap = rotaingImageView(90, photoBitmap);
        } else if (orientation == ExifInterface.ORIENTATION_ROTATE_180) {
            photoBitmap = rotaingImageView(180, photoBitmap);
        } else if (orientation == ExifInterface.ORIENTATION_ROTATE_270) {
            photoBitmap = rotaingImageView(270, photoBitmap);
        }
        photoBitmap.compress(getCompressFormat(mimeType), 50, bos);
        try {
            FileOutputStream fos = new FileOutputStream(new File(outputImagePath));
            fos.write(bos.toByteArray());
            fos.flush();
            fos.close();
            bos.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static Bitmap.CompressFormat getCompressFormat(String mimeType) {
        if (mimeType == null) {
            return Bitmap.CompressFormat.JPEG;
        }
        if (mimeType.contains("jpg")) {
            return Bitmap.CompressFormat.JPEG;
        } else if (mimeType.contains("jpeg")) {
            return Bitmap.CompressFormat.JPEG;
        } else if (mimeType.contains("png")) {
            return Bitmap.CompressFormat.PNG;
        } else if (mimeType.contains("webp")) {
            //return Bitmap.CompressFormat.WEBP;
            return Bitmap.CompressFormat.JPEG;
        } else {
            return Bitmap.CompressFormat.JPEG;
        }
    }

    public static int getImageOrientation(String imageLocalPath) {
        try {
            ExifInterface exifInterface = new ExifInterface(imageLocalPath);
            int orientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
            return orientation;
        } catch (IOException e) {
            e.printStackTrace();
            return ExifInterface.ORIENTATION_NORMAL;
        }
    }

    public static Bitmap rotaingImageView(int angle, Bitmap bitmap) {
        Matrix matrix = new Matrix();
        matrix.postRotate(angle);
        Bitmap resizedBitmap = Bitmap.createBitmap(bitmap, 0, 0,
                bitmap.getWidth(), bitmap.getHeight(), matrix, true);
        return resizedBitmap;
    }

}
