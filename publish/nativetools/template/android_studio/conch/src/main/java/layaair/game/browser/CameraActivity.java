package layaair.game.browser;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import com.open.camera.CameraView;
import com.open.camera.CustomCameraView;
import com.open.camera.listener.ClickListener;
import com.open.camera.listener.FlowCameraListener;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import layaair.game.R;

import static com.open.camera.CameraView.BUTTON_STATE_ONLY_CAPTURE;
import static layaair.game.browser.Picture.MultiImageSelectorActivity.EXTRA_RESULT;
import static layaair.game.browser.Picture.MultiImageSelectorActivity.EXTRA_RESULT_SIZE;

public class CameraActivity extends AppCompatActivity {

    private static final String TAG = "CameraActivity";
    private static final int REQUEST_PERMISSION_CODE = 1;
    public static final String[] REQUIRED_PERMISSIONS = new String[]{
            "android.permission.CAMERA",
            "android.permission.WRITE_EXTERNAL_STORAGE"};
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.camera_layout);

        CustomCameraView camera = findViewById(R.id.camera);
        camera.setBindToLifecycle(this);
        camera.setCaptureMode(BUTTON_STATE_ONLY_CAPTURE);
        camera.setFlowCameraListener(new FlowCameraListener() {
            @Override
            public void captureSuccess(@NonNull File file) {
                Intent data = new Intent();
                ArrayList<String> resultList = new ArrayList<>();
                ArrayList<Integer> sizeList = new ArrayList<>();
                resultList.add(file.getAbsolutePath());
                sizeList.add((int) file.length());
                data.putStringArrayListExtra(EXTRA_RESULT, resultList);
                data.putIntegerArrayListExtra(EXTRA_RESULT_SIZE, sizeList);
                setResult(RESULT_OK, data);
                finish();
            }

            @Override
            public void recordSuccess(@NonNull File file) {

            }

            @Override
            public void onError(int videoCaptureError, @NonNull String message, @Nullable Throwable cause) {
                setResult(RESULT_CANCELED);
                finish();
            }
        });
        camera.setLeftClickListener(new ClickListener() {
            @Override
            public void onClick() {
                setResult(RESULT_CANCELED);
                finish();
            }
        });
    }
    private boolean checkPermission() {
        Log.d(TAG, "checkPermission: ");
        List<String> deniedPermissions = new ArrayList<>();
        for (String per : REQUIRED_PERMISSIONS) {
            int permissionCode = ActivityCompat.checkSelfPermission(this, per);
            Log.d(TAG, "checkPermissionFirst: " + permissionCode);
            if (permissionCode != PackageManager.PERMISSION_GRANTED) {
                deniedPermissions.add(per);
            }
        }
        if (!deniedPermissions.isEmpty()) {
            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS, REQUEST_PERMISSION_CODE);
        }
        return deniedPermissions.isEmpty();
    }
}

