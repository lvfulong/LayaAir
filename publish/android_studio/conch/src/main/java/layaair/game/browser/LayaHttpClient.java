package layaair.game.browser;
import android.util.Log;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.concurrent.TimeUnit;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Headers;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class LayaHttpClient {

    public static final String TAG = "LayaHttpClient";

    public static OkHttpClient.Builder builder = new OkHttpClient.Builder();
    public static OkHttpClient client = null;//shared  client
    public OkHttpClient eagerClient = null;

    public Response response;

    public Request.Builder requestBuilder = new Request.Builder();

    public OkHttpClient.Builder eagerClientBuilder;// = client.newBuilder();

    public String contentType = "";

    public String url = null;

    public String localFilePath = "";

    public long ptr;

    public Call call = null;
    public static void test() {

    }

    public LayaHttpClient(){
        eagerClientBuilder = client.newBuilder();
        //.retryOnConnectionFailure(false); //禁止重试，否则断网会导致3分钟的卡死
    }

    public static LayaHttpClient create(long ptr, String url, String localFilePath) {
        if (client == null) {
            Log.d(TAG, "cache path: " + ExportJavaFunction.GetInstance().m_pEngine.mCtx.getCacheDir());
            //if (config.GetInstance().m_bUseDcc) {
                client = builder.build();//DCC需关闭缓存
            //}
            //else {
            //    client = builder.cache(new Cache(new File(ExportJavaFunction.GetInstance().m_pEngine.mCtx.getCacheDir(), "okhttpcache"), 100 * 1024 * 1024)).build();
            //}
            client.dispatcher().setMaxRequests(128);
            client.dispatcher().setMaxRequestsPerHost(10);
        }
        //Log.d(TAG, "create: " + strUrl);
        LayaHttpClient connection = new LayaHttpClient();
        connection.requestBuilder.url(url);
        connection.url = url;
        connection.localFilePath = localFilePath;
        connection.ptr = ptr;
        return connection;
    }
    static void cancel(LayaHttpClient connection) {
        Log.d(TAG, "cancel ");
        if (connection.call != null) {
            connection.call.cancel();
        }
    }
    /*public static boolean connect(LayaHttpClient connection) {
        //Log.d(TAG, "connect ");
        return true;
    }*/
    static void postData(LayaHttpClient connection, byte[] byteArray) {
        Log.d(TAG, "postData ");
        connection.requestBuilder.post(RequestBody.create(MediaType.parse(connection.contentType), byteArray));
    }
    static void addHeader(LayaHttpClient connection, String key, String value) {
        Log.d(TAG, "addHeader " + key + " -- " + value);
        if (key.equalsIgnoreCase("Content-Type")) {
            connection.contentType = value;
        }
        connection.requestBuilder.header(key, value);
    }
    static void setMethod(LayaHttpClient connection, String method) {
        //Log.d(TAG, "setMethod ");
    }
    static String getResponseHeaders(Response response) {
        Headers headers = response.headers();
        String strHeader = "";
        for (int i = 0; i < headers.size(); i++) {
            String key = headers.name(i);
            strHeader += key + ":" + listToString(headers.values(key), ",") + "\n";
        }
        //Log.d(TAG, "getResponseHeaders " + strHeader);
        return strHeader;
    }
    public static String listToString(List<String> list, String strInterVal) {
        if (list == null) {
            return null;
        }
        StringBuilder result = new StringBuilder();
        boolean flag = false;
        for (String str : list) {
            if (flag) {
                result.append(strInterVal);
            }
            if (null == str) {
                str = "";
            }
            result.append(str);
            flag = true;
        }
        return result.toString();
    }
    static void doRequest(LayaHttpClient connection) {
        try{
        connection.eagerClient = connection
                .eagerClientBuilder
                .build();
            connection.call = connection.eagerClient.newCall(connection.requestBuilder.build());
        //Log.e(TAG,"下载："+connection.call.request().url().toString());
        connection.call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                connection.call = null;
                Log.e(TAG, "onFailure " + e.getMessage());
                if(call.isCanceled()) {
                    LayaHttpClient.onFailure(connection.ptr, -999);//
                }
                else {
                    LayaHttpClient.onFailure(connection.ptr, 0);
                }
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                connection.call = null;
                //Log.e(TAG, "onResponse:" + connection.url);
                InputStream in = response.body().byteStream();
                String header = getResponseHeaders(response);
                int code = response.code();
                //Log.e(TAG, "onResponse:code " + code);
                if (connection.localFilePath.length() > 0) {
                    File file = new File(connection.localFilePath);
                    byte[] buffer = new byte[1024];
                    FileOutputStream fos = null;
                    try {
                        fos = new FileOutputStream(file, false);
                        int len;
                        int now = 0;
                        int total = (int)response.body().contentLength();
                        while ((len = in.read(buffer)) != -1) {
                            now += len;
                            fos.write(buffer, 0, len);
                            LayaHttpClient.onProgress(connection.ptr, total, now,0);
                        }
                        fos.flush();
                        LayaHttpClient.onResponse(connection.ptr, null, code, header);
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "getResponseContent:" + e.toString());
                        LayaHttpClient.onFailure(connection.ptr, -1);
                    }
                }
                else {
                    try {
                        byte[] buffer = new byte[1024];
                        int size = 0;
                        ByteArrayOutputStream bytestream = new ByteArrayOutputStream();
                        while ((size = in.read(buffer, 0, 1024)) != -1) {
                            bytestream.write(buffer, 0, size);
                        }
                        byte retbuffer[] = bytestream.toByteArray();
                        bytestream.close();
                        LayaHttpClient.onResponse(connection.ptr, retbuffer, code, header);
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "getResponseContent:" + e.toString());
                        LayaHttpClient.onFailure(connection.ptr, -1);
                    }
                }
                try {
                    if (in != null) {
                        in.close();
                    }
                } catch (IOException e) {
                }
                response.close();
            }
        });
        } catch (Exception e) {
            Log.e(TAG, "Error in doRequest: " + e.toString());
            LayaHttpClient.onFailure(connection.ptr, -1);
        }
    }
    /*static void disconnect(LayaHttpClient http) {
        //Log.d(TAG, "HttpURLConnection disconnect ");
        http = null;
    }*/
    static void setReadTimeout(LayaHttpClient connection, int miliseconds) {
        //Log.d(TAG, "HttpURLConnection setReadTimeout " + miliseconds);
        connection.eagerClientBuilder.readTimeout(miliseconds, TimeUnit.MILLISECONDS);
    }
    static void setConnectTimeout(LayaHttpClient connection, int miliseconds) {
        //Log.d(TAG, "HttpURLConnection setConnectTimeout " + miliseconds);
        connection.eagerClientBuilder.connectTimeout(miliseconds, TimeUnit.MILLISECONDS);
    }
    public static native void onProgress(long ptr, int total, int now, float speed);
    public static native void onResponse(long ptr, byte[] data, int code, String header);
    public static native void onFailure(long ptr, int code);
}
