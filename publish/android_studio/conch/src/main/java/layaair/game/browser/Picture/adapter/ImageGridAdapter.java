package layaair.game.browser.Picture.adapter;

import android.content.Context;
import android.graphics.Point;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.BaseAdapter;
import android.widget.ImageView;

import com.bumptech.glide.Glide;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import layaair.game.R;
import layaair.game.browser.Picture.bean.Image;

public class ImageGridAdapter extends BaseAdapter {

    private static final String TAG = "ImageSelector";

    private static final int TYPE_CAMERA = 0;
    private static final int TYPE_NORMAL = 1;

    private Context mContext;
    private LayoutInflater mInflater;
    private boolean showCamera = true;
    private boolean showSelectIndicator = true;
    private List<Image> mImages = new ArrayList<>();
    private List<Image> mSelectedImages = new ArrayList<>();
    final int mGridWidth;

    public ImageGridAdapter(Context context, boolean showCamera, int column){
        mContext = context;
        mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        this.showCamera = showCamera;
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        int width = 0;
        Point size = new Point();
        wm.getDefaultDisplay().getSize(size);
        width = size.x;
        Log.d(TAG, "ImageGridAdapter: width " + width);
        mGridWidth = width / column;
        Log.d(TAG, "ImageGridAdapter: mGridWidth " + mGridWidth);
    }

    public void showSelectIndicator(boolean b) {
        showSelectIndicator = b;
    }

    public void select(Image image) {
        if (mSelectedImages.contains(image)) {
            mSelectedImages.remove(image);
        } else {
            mSelectedImages.add(image);
        }
        notifyDataSetChanged();
    }

    public void setDefaultSelected(ArrayList<String> resultList) {
        for (String path : resultList) {
            Image image = getImageByPath(path);
            if (image != null) {
                mSelectedImages.add(image);
            }
        }
        if (mSelectedImages.size() > 0) {
            notifyDataSetChanged();
        }
    }

    private Image getImageByPath(String path) {
        if (mImages != null && mImages.size()>0) {
            for (Image image : mImages) {
                if (image.getPath().equalsIgnoreCase(path)) {
                    return image;
                }
            }
        }
        return null;
    }

    public void setData(List<Image> images) {
        mSelectedImages.clear();
        if (images != null && images.size()>0) {
            mImages = images;
        } else {
            mImages.clear();
        }
        notifyDataSetChanged();
    }

    @Override
    public int getViewTypeCount() {
        return 2;
    }

    @Override
    public int getItemViewType(int position) {
        if (showCamera) {
            return position==0 ? TYPE_CAMERA : TYPE_NORMAL;
        }
        return TYPE_NORMAL;
    }

    @Override
    public int getCount() {
        return showCamera ? mImages.size()+1 : mImages.size();
    }

    @Override
    public Image getItem(int position) {
        if (showCamera) {
            if (position == 0) {
                return null;
            }
            return mImages.get(position - 1);
        } else {
            return mImages.get(position);
        }
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
//        if (isShowCamera()) {
//            if (position == 0) {
//                convertView = mInflater.inflate(R.layout.mis_list_item_camera, parent, false);
//                return convertView;
//            }
//        }

        ViewHolder holder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.mis_list_item_image, parent, false);
            holder = new ViewHolder(convertView);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        if (holder != null) {
            holder.bindData(getItem(position));
        }

        return convertView;
    }

    class ViewHolder {
        ImageView image;
        ImageView indicator;
        View mask;

        ViewHolder(View view) {
            image = (ImageView) view.findViewById(R.id.image);
            indicator = (ImageView) view.findViewById(R.id.checkmark);
            mask = view.findViewById(R.id.mask);
            view.setTag(this);
        }

        void bindData(final Image data) {
            if (data == null) {
                return;
            }
            // 处理单选和多选状态
            if (showSelectIndicator) {
                indicator.setVisibility(View.VISIBLE);
                if (mSelectedImages.contains(data)) {
                    // 设置选中状态
                    indicator.setImageResource(R.drawable.mis_btn_selected);
                    mask.setVisibility(View.VISIBLE);
                } else {
                    // 未选择
                    indicator.setImageResource(R.drawable.mis_btn_unselected);
                    mask.setVisibility(View.GONE);
                }
            } else {
                indicator.setVisibility(View.GONE);
            }
            File imageFile = new File(data.getPath());
            if (imageFile.exists()) {
                Glide.with(mContext)
                        .load(imageFile)
                        .override(mGridWidth, mGridWidth)
                        .centerCrop()
                        .into(image);
            } else {
//                image.setImageResource(R.drawable.mis_default_error);
                Log.d(TAG, "bindData: image file not exists");
            }
        }
    }
}
