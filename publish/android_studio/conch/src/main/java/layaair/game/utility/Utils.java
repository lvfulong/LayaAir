package layaair.game.utility;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import androidx.core.app.ActivityCompat;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by hugao on 2016/11/5.
 */

public class Utils {
    public static byte[] screenShot(GL10 gl, int width, int height) {
        long begin=System.currentTimeMillis();
        int screenshotSize = width * height*4;
        ByteBuffer bb = ByteBuffer.allocateDirect(screenshotSize);
        bb.order(ByteOrder.nativeOrder());
        gl.glReadPixels(0, 0, width, height, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE, bb);
        /*int pixelsBuffer[] = new int[screenshotSize];
        bb.asIntBuffer().get(pixelsBuffer);
        bb = null;
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap.setPixels(pixelsBuffer, screenshotSize - width, -width, 0, 0, width, height);
        byte sBuffer[] = new byte[screenshotSize*4];
        ByteBuffer sb = ByteBuffer.wrap(sBuffer);
        bitmap.copyPixelsToBuffer(sb);
        int size=screenshotSize*4;
        byte temp;
        for(int i=0;i<size;i+=4)
        {
            temp=sBuffer[i];
            sBuffer[i]=sBuffer[i+2];
            sBuffer[i+2]=temp;
        }
        long useTime=System.currentTimeMillis()-begin;
        Log.e("temp",">>>>>>>>>>>>>>>>>>>>>>>useTimenew"+  useTime);
        return sb.array();*/
        byte [] a;
        try {
            a = bb.array();
        }
        catch (UnsupportedOperationException e)
        {
            a=new byte[screenshotSize];
            bb.get(a);
        }

        verticalMirror(a,width,height);
        long useTime=System.currentTimeMillis()-begin;
        Log.e("temp",">>>>>>>>>>>>>>>>>>>>>>>useTimenew"+  useTime);
        return a;
    }

    public static byte[] verticalMirror(byte[]a,int w,int h) {
        byte tR,tG,tB,tA;
        int  tempF,tempT;
        for (int i = 0; i < w; i++)//n列
        {
            for (int j = 0; j < h / 2; j++) //每一列转换（n/2）次
            {
                //将上下两个对称的元素进行交
                tempF=(w * j + i)*4;
                tempT=((h - j - 1) * w + i)*4;
                tR=a[tempF];
                tG=a[tempF+1];
                tB=a[tempF+2];
                tA=a[tempF+3];
                a[tempF]=a[tempT];
                a[tempF+1]=a[tempT+1];
                a[tempF+2]=a[tempT+2];
                a[tempF+3]=a[tempT+3];
                a[tempT]=tR;
                a[tempT+1]=tG;
                a[tempT+2]=tB;
                a[tempT+3]=tA;
            }
        }
        return  a;
    }
    public static int getResIdByName(Context context, String className, String resName) {
        String packageName = context.getPackageName();
        int id = 0;
        try {
            Class r = Class.forName(packageName + ".R");
            Class[] classes = r.getClasses();
            Class desireClass = null;
            for (Class cls : classes) {
                if (cls.getName().split("\\$")[1].equals(className)) {
                    desireClass = cls;
                    break;
                }
            }
            if (desireClass != null) {
                id = desireClass.getField(resName).getInt(desireClass);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return id;
    }

    public static String md5(String input) {
        try {
            byte[] bytes = MessageDigest.getInstance("MD5").digest(input.getBytes());
            return printHexBinary(bytes);
        } catch (NoSuchAlgorithmException exception) {
            Log.d("LayaConch5", "md5: NoSuchAlgorithmException");
            exception.printStackTrace();
            return "";
        }
    }

    public static String printHexBinary(byte[] data) {
        StringBuilder r = new StringBuilder(data.length * 2);
        for (byte b : data) {
            r.append(String.format("%02X", b & 0xFF));
        }
        return r.toString();
    }

    public static boolean copyFile(String oldPath$Name, String newPath$Name) {
        try {
            File oldFile = new File(oldPath$Name);
            if (!oldFile.exists() || !oldFile.isFile() || !oldFile.canRead()) {
                Log.d("LayaConch5", "copyFile: oldFile exist " + oldFile.exists());
                Log.d("LayaConch5", "copyFile: oldFile isFile " + oldFile.isFile());
                Log.d("LayaConch5", "copyFile: oldFile canRead " + oldFile.canRead());
                return false;
            }
            FileInputStream fileInputStream = new FileInputStream(oldPath$Name);
            FileOutputStream fileOutputStream = new FileOutputStream(newPath$Name);
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = fileInputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            fileInputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
    public static String getExtension(String filename) {
        if ((filename != null) && (filename.length() > 0)) {
            int dot = filename.lastIndexOf('.');
            if ((dot > -1) && (dot < (filename.length() - 1))) {
                return filename.substring(dot);
            }
        }
        return "";
    }
    public static String getCacheDir(Context context) {
        String path = "";
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED) || !Environment.isExternalStorageRemovable()) {
            try {
                path = context.getExternalCacheDir().getAbsolutePath();
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (TextUtils.isEmpty(path)) {
                //path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
                path = context.getCacheDir().getAbsolutePath();
            }
        } else {
            path = context.getCacheDir().getAbsolutePath();
        }
        return path;
    }
    public static String getFileDir(Context context) {
        String path = "";
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED) || !Environment.isExternalStorageRemovable()) {
            try {
                path = context.getExternalFilesDir(null).getAbsolutePath();
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (TextUtils.isEmpty(path)) {
                //path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath();
                path = context.getFilesDir().getAbsolutePath();
            }
        } else {
            path = context.getFilesDir().getAbsolutePath();
        }
        return path;
    }
    public static boolean checkPermission(Context context, String[] permission, int requestCode) {
        List<String> deniedPermissions = new ArrayList<String>();
        for (String per : permission) {
            int permissionCode = ActivityCompat.checkSelfPermission(context, per);
            Log.d("LayaConch5", "checkPermission: " + permissionCode);
            if (permissionCode != PackageManager.PERMISSION_GRANTED) {
                deniedPermissions.add(per);
            }
        }
        if (deniedPermissions.isEmpty()) {
            return true;
        }
        else {
            ActivityCompat.requestPermissions((Activity) context, permission, requestCode);
            return false;
        }
    }
}
