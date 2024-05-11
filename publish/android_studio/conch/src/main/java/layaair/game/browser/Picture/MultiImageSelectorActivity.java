package layaair.game.browser.Picture;

import android.Manifest;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.app.ActivityCompat;
import androidx.core.content.FileProvider;
import android.text.TextUtils;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.GridView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import androidx.loader.app.LoaderManager;
import androidx.loader.content.CursorLoader;
import androidx.loader.content.Loader;
import layaair.game.R;
import layaair.game.browser.Picture.adapter.ImageGridAdapter;
import layaair.game.browser.Picture.bean.Image;
import layaair.game.utility.Constants;
import layaair.game.utility.Utils;

public class MultiImageSelectorActivity extends AppCompatActivity {

    private static final String TAG = "ImageSelector";


    public static final String  CROP_WIDTH   = "crop_width";
    public static final String  CROP_HEIGHT  = "crop_Height";
    public static final String  RATIO_WIDTH  = "ratio_Width";
    public static final String  RATIO_HEIGHT = "ratio_Height";
    public static final String  ENABLE_CROP  = "enable_crop";

    public static final int MODE_SINGLE = 0;
    public static final int MODE_MULTI = 1;

    public static final String EXTRA_SELECT_MODE = "select_count_mode";
    public static final String EXTRA_RESULT = "select_result";
    public static final String EXTRA_RESULT_SIZE = "select_result_size";
    private static final int DEFAULT_IMAGE_SIZE = 9;
    private static final int LOADER_ALL = 1;

    public static final String[] REQUIRED_PERMISSIONS = new String[]{
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"};
    private boolean isToast = true;//是否弹吐司，为了保证for循环只弹一次
    private int mCropWidth;
    private int mCropHeight;
    private int mRatioWidth;
    private int mRatioHeight;
    private boolean mCropEnabled;
    private boolean isCamera;
    private int mSelectImageCount;
    private int mode;
    private ArrayList<String> resultList = new ArrayList<>();
    private ArrayList<Integer> sizeList = new ArrayList<>();
    private Button mSubmitButton;
    private GridView mGridView;
    private ImageGridAdapter mImageAdapter;
    private boolean hasStarted = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_picture_select);


        mSelectImageCount = getIntent().getIntExtra("count", 9);
        mCropEnabled = (getIntent().getStringExtra("sizeType").equals("compressed"));
        isCamera = (getIntent().getStringExtra("sourceType").equals("camera"));
        mode = getIntent().getIntExtra(EXTRA_SELECT_MODE, MODE_MULTI);
        hasStarted = false;

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        if (toolbar != null) {
            setSupportActionBar(toolbar);
        }

        final ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setHomeButtonEnabled(true);
            actionBar.setDisplayShowTitleEnabled(false);
        }

        mSubmitButton = (Button) findViewById(R.id.commit);
        if (mode == MODE_MULTI) {
            updateDoneText(resultList);
            mSubmitButton.setVisibility(View.VISIBLE);
            mSubmitButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (resultList != null && resultList.size() > 0) {
                        Intent data = new Intent();
                        data.putStringArrayListExtra(EXTRA_RESULT, resultList);
                        data.putIntegerArrayListExtra(EXTRA_RESULT_SIZE, sizeList);
                        setResult(RESULT_OK, data);
                    } else {
                        setResult(RESULT_CANCELED);
                    }
                    finish();
                }
            });
        } else {
            mSubmitButton.setVisibility(View.GONE);
        }

        //请求应用需要的所有权限
        boolean permission = Utils.checkPermission(this, REQUIRED_PERMISSIONS, Constants.REQUEST_PERMISSION_CODE_CHOOSE_IMAGE);
        if (permission) {
            initView();
            load();
        }

    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume: ");
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy: ");
        super.onDestroy();
    }


    private void initView() {
        mImageAdapter = new ImageGridAdapter(this, false, 4);
        mGridView = (GridView) findViewById(R.id.grid);
        mGridView.setAdapter(mImageAdapter);
        mGridView.setNumColumns(4);
        mGridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Image image = (Image) adapterView.getAdapter().getItem(i);
                selectImageFromGrid(image, mode);
            }
        });
    }

    private void load() {
        getSupportLoaderManager().initLoader(LOADER_ALL, null, mLoaderCallback);
    }

    private void updateDoneText(ArrayList<String> resultList) {
        int size = 0;
        if (resultList == null || resultList.size() <= 0) {
            mSubmitButton.setText("完成");
            mSubmitButton.setEnabled(false);
        } else {
            size = resultList.size();
            mSubmitButton.setEnabled(true);
        }
        mSubmitButton.setText(getString(R.string.mis_action_button_string, "完成", size, mSelectImageCount));
    }


    private void onSingleImageSelected(Image image) {
        Intent data = new Intent();
        resultList.add(image.getPath());
        sizeList.add((int) image.getSize());
        data.putStringArrayListExtra(EXTRA_RESULT, resultList);
        data.putIntegerArrayListExtra(EXTRA_RESULT_SIZE, sizeList);
        setResult(RESULT_OK, data);
        finish();
    }


    private void onImageSelected(Image image) {
        Log.d(TAG, "onImageSelected: ");
        String pathStr = image.getPath();
        if(!resultList.contains(pathStr)) {
            resultList.add(pathStr);
            sizeList.add((int) image.getSize());
        }
        updateDoneText(resultList);
    }


    private void onImageUnselected(Image image) {
        Log.d(TAG, "onImageUnselected: ");
        String pathStr = image.getPath();
        if(resultList.contains(pathStr)) {
            resultList.remove(pathStr);
            sizeList.remove((Object)((int) image.getSize()));
        }
        updateDoneText(resultList);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            Log.d(TAG, "onOptionsItemSelected: home");
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    private void selectImageFromGrid(Image image, int mode) {
        Log.d(TAG, "selectImageFromGrid: ");
        if (image != null) {
            if (mode == MODE_MULTI) {
                String pathStr = image.getPath();
                if (resultList.contains(pathStr)) {

                    onImageUnselected(image);
                } else {
                    if (mSelectImageCount == resultList.size()) {
                        Log.d(TAG, "selectImageFromGrid: count limit");
                        Toast.makeText(MultiImageSelectorActivity.this, getString(R.string.mis_count_limit_string, mSelectImageCount), Toast.LENGTH_SHORT).show();
                        return;
                    }

                    onImageSelected(image);
                }
                mImageAdapter.select(image);
            } else if (mode == MODE_SINGLE) {
                onSingleImageSelected(image);
            }
        }
    }

    private LoaderManager.LoaderCallbacks<Cursor> mLoaderCallback = new LoaderManager.LoaderCallbacks<Cursor>() {

        private final String[] IMAGE_PROJECTION = {
                MediaStore.Images.Media.DATA,
                MediaStore.Images.Media.DISPLAY_NAME,
                MediaStore.Images.Media.DATE_ADDED,
                MediaStore.Images.Media.MIME_TYPE,
                MediaStore.Images.Media.SIZE,
                MediaStore.Images.Media._ID };


        @Override
        public Loader<Cursor> onCreateLoader(int id, Bundle args) {
            CursorLoader cursorLoader = null;
            if (id == LOADER_ALL) {
                cursorLoader = new CursorLoader(MultiImageSelectorActivity.this,
                        MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_PROJECTION,
                        IMAGE_PROJECTION[4] + ">0 AND " + IMAGE_PROJECTION[3] + "=? OR " + IMAGE_PROJECTION[3] + "=? OR " + IMAGE_PROJECTION[3] + "=? ",
                        new String[]{"image/jpeg", "image/png", "image/jpg"}, IMAGE_PROJECTION[2] + " DESC");
            }
            return cursorLoader;
        }

        private boolean fileExist(String path){
            if(!TextUtils.isEmpty(path)){
                return new File(path).exists();
            }
            return false;
        }

        @Override
        public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
            if (data != null) {
                if (data.getCount() > 0) {
                    List<Image> images = new ArrayList<>();
                    data.moveToFirst();
                    do{
                        String path = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[0]));
                        String name = data.getString(data.getColumnIndexOrThrow(IMAGE_PROJECTION[1]));
                        long dateTime = data.getLong(data.getColumnIndexOrThrow(IMAGE_PROJECTION[2]));
                        long size = data.getLong(data.getColumnIndexOrThrow(IMAGE_PROJECTION[4]));
                        if (!fileExist(path) ) {
                            continue;
                        }
                        Image image = null;
                        if (!TextUtils.isEmpty(name)) {
                            image = new Image(path);
                            image.setSize(size);
                            images.add(image);
                        }
                    } while (data.moveToNext());

                    mImageAdapter.setData(images);
                    hasStarted = true;
                    if (resultList != null && resultList.size() > 0) {
                        mImageAdapter.setDefaultSelected(resultList);
                    }
                }
            }
        }

        @Override
        public void onLoaderReset(Loader<Cursor> loader) {
            Log.d(TAG, "onLoaderReset: ");
        }
    };

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        boolean isPermissions = true;
        for (int i = 0; i < permissions.length; i++) {
            if (grantResults[i] == PackageManager.PERMISSION_DENIED) {
                isPermissions = false;
                if (!ActivityCompat.shouldShowRequestPermissionRationale(this, permissions[i])) { //用户选择了"不再询问"
                    if (isToast) {
                        Toast.makeText(this, "请手动打开该应用需要的权限", Toast.LENGTH_SHORT).show();
                        isToast = false;
                    }

                    /*跳转到应用详情，让用户去打开权限*/
                    Intent localIntent = new Intent();
                    localIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    localIntent.setAction("android.settings.APPLICATION_DETAILS_SETTINGS");
                    localIntent.setData(Uri.fromParts("package", getPackageName(), null));
                    startActivity(localIntent);
                }


                setResult(RESULT_CANCELED);
                finish();
            }
        }
        isToast = true;
        if (isPermissions) {
            Log.d(TAG, "onRequestPermissionsResult: " + "允许所有权限");
            initView();
            load();
        } else {
            Log.d(TAG, "onRequestPermissionsResult: " + "有权限不允许");
            setResult(RESULT_CANCELED);
            finish();
        }
    }
}
