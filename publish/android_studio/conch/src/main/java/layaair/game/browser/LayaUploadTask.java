package layaair.game.browser;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.Log;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class LayaUploadTask {
    public static final String TAG = "UploadTask";
    private static final OkHttpClient client = new OkHttpClient();
    
    private String url;
    private String filePath;
    private Map<String, String> headers;
    private Map<String, String> formData;
    private boolean isAborted;
    private boolean isCompleted;
    private boolean isFailed;
    private boolean isSuccess;
    private long totalBytes;
    private long uploadedBytes;

    private Request request;
    private Call call;
    private Response response;
    private Callback callback;
    public long ptr;

    public static class Description {
        public String url;
        public String filePath;
        public Map<String, String> headers;
        public Map<String, String> formData;
        public int timeout = 5000;
        public void setUrl(String url) {
            this.url = url;
        }
        public void setFilePath(String filePath) {
            this.filePath = filePath;
        }
        public void setHeaders(Map<String, String> headers) {
            this.headers = headers;
        }
        public void setFormData(Map<String, String> formData) {
            this.formData = formData;
        }   
        public void setTimeout(int timeout) {   
            this.timeout = timeout;
        }   
    }

    public static LayaUploadTask upload(long ptr, Description desc) {
        LayaUploadTask task = new LayaUploadTask();
        task.ptr = ptr;
        task.url = desc.url;
        task.filePath = desc.filePath;
        task.headers = desc.headers != null ? desc.headers : new HashMap<>();
        task.formData = desc.formData != null ? desc.formData : new HashMap<>();
        
        // 创建文件对象
        File file = new File(desc.filePath);
        if (!file.exists()) {
            Log.e(TAG, "File not found: " + desc.filePath);
            return null;
        }

        // 创建 MultipartBody.Builder
        MultipartBody.Builder builder = new MultipartBody.Builder()
            .setType(MultipartBody.FORM);

        // 添加表单数据
        for (Map.Entry<String, String> entry : task.formData.entrySet()) {
            builder.addFormDataPart(entry.getKey(), entry.getValue());
        }

        // 添加文件
        MediaType mediaType = MediaType.parse("application/octet-stream");
        builder.addFormDataPart("file", file.getName(),
            RequestBody.create(mediaType, file));

        // 创建请求
        Request.Builder requestBuilder = new Request.Builder()
            .url(desc.url)
            .post(builder.build());

        // 添加请求头
        for (Map.Entry<String, String> entry : task.headers.entrySet()) {
            requestBuilder.addHeader(entry.getKey(), entry.getValue());
        }

        // 设置超时
        if (desc.timeout > 0) {
            client.newBuilder()
                .connectTimeout(desc.timeout, java.util.concurrent.TimeUnit.MILLISECONDS)
                .readTimeout(desc.timeout, java.util.concurrent.TimeUnit.MILLISECONDS)
                .writeTimeout(desc.timeout, java.util.concurrent.TimeUnit.MILLISECONDS)
                .build();
        }

        task.request = requestBuilder.build();
        task.call = client.newCall(task.request);
        task.call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                task.isFailed = true;
                task.isCompleted = true;
                LayaUploadTask.onFailure(task.ptr, 0, e.getMessage());
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                task.isCompleted = true;
                if (response.isSuccessful()) {
                    task.isSuccess = true;
                } else {
                    task.isFailed = true;
                }
                LayaUploadTask.onResponse(task.ptr, response.code(), response.body().string());
            }
        });
        return task;
    }
    static void abort(LayaUploadTask task) {
        Log.d(TAG, "abort ");
        task.abort();
    }
    public void abort() {
        if (call != null) {
            call.cancel();
            isAborted = true;
        }
    }

    public boolean isCompleted() {
        return isCompleted;
    }

    public boolean isFailed() {
        return isFailed;
    }

    public boolean isSuccess() {
        return isSuccess;
    }

    public boolean isAborted() {  
        return isAborted;
    }

    public long getTotalBytes() {
        return totalBytes;
    }

    public long getUploadedBytes() {
        return uploadedBytes;
    }
    public static native void onFailure(long ptr, int statusCode, String errorMsg);
    public static native void onResponse(long ptr, int statusCode, String data);
}   
    