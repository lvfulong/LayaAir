package layaair.game.browser;

import java.util.regex.Pattern;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.core.widget.NestedScrollView;

import layaair.game.R;
import layaair.game.conch.LayaConch5;
import layaair.game.utility.DensityUtils;

import android.animation.LayoutTransition;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.Selection;
import android.text.Spannable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;


import static android.content.res.Configuration.ORIENTATION_LANDSCAPE;

public class LayaEditBoxNew implements KeyboardHeightObserver
{
	private static final String	TAG = "LayaEditBoxNew";
	public EditText	mEditBox = null;
	//public Button	m_pEditBoxTouch = null;
	public String mConfirmType = null;
	public String mInputType = null;
	public boolean mMultiple = false;
	public boolean mConfirmHold = false;
	public String mDefaultValue = null;
	public String mHint = null;
	public String mHintColor = null;
	public int	mMaxLength = 0;
	public static LayaEditBoxNew instance = null;
	public Context mContext = null;
	public KeyboardHeightProvider mKeyboardHeightProvider = null;
	public RelativeLayout mEditboxPanelBg;
	public RelativeLayout mEditboxPanel;
	public LinearLayout mRootLayout = null;
	public int mOrientation;
	private NestedScrollView mScrollView;
	private int mEditboxPanelHeight = 0;
	@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
	public LayaEditBoxNew(final String defaultValue, final int maxLength, final boolean multiple, final boolean confirmHold, final String confirmType, final String hint , final String hintColor, final String inputType) {
		mContext = LayaConch5.ms_layaConche.mCtx;
		LayaEditBoxNew.instance = this;
		mKeyboardHeightProvider = new KeyboardHeightProvider((Activity)mContext);
		mKeyboardHeightProvider.start();
		mKeyboardHeightProvider.setKeyboardHeightObserver(this);
		mConfirmType = confirmType;
		mMultiple = multiple;
		mConfirmHold = confirmHold;
		mDefaultValue = defaultValue;
		mHint = hint;
		mHintColor = hintColor;
		mMaxLength = maxLength;
		mInputType = inputType;
		initView();
		mOrientation = getScreenOrientation();
		InputMethodManager imm = (InputMethodManager)mContext.getSystemService(mContext.INPUT_METHOD_SERVICE);
		//imm.showSoftInput(m_pEditBox, InputMethodManager.SHOW_IMPLICIT);
		imm.showSoftInput(mEditBox, 0);
		/*if (m_context.getResources().getConfiguration().orientation == ORIENTATION_LANDSCAPE) {
			m_rootLayout.setVisibility(View.VISIBLE);
			m_rootLayout.setX(-10000);
			m_rootLayout.setY(-10000);
		}
		else {*/
		mRootLayout.setVisibility(View.VISIBLE);
		//}
	}
	public static boolean show(final String defaultValue, final int maxLength, final boolean multiple, final boolean confirmHold, final String confirmType, final String hint , final String hintColor, final String inputType) {
		Log.d(TAG, "show " + defaultValue + " " + maxLength + " " + multiple + " " + confirmHold + " " + confirmType + " " + hint + " " + hintColor + " " + inputType);
		final ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp == null)
			return false;

		((Activity)exp.m_pEngine.mCtx).runOnUiThread(new Runnable() {
			@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
			@Override
			public void run() {
				if (LayaEditBoxNew.instance != null)
				{
					LayaEditBoxNew.instance.close();
				}
				new LayaEditBoxNew(defaultValue, maxLength, multiple, confirmHold, confirmType, hint , hintColor, inputType);
			}
		});
		return true;
	}
	public void close() {
		Log.d(TAG, "close ");
		InputMethodManager imm = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(mEditBox.getWindowToken(), 0);
		LayaEditBox.hideSoftKeyBorad();
		ConchJNI.handleKeyboardConfirm(mEditBox.getText().toString());
		ConchJNI.handleKeyboardComplete(mEditBox.getText().toString());
		mRootLayout.setVisibility(View.INVISIBLE);
		LayaConch5.ms_layaConche.getAbsLayout().removeViewInLayout(mRootLayout);
		mKeyboardHeightProvider.close();
		LayaEditBoxNew.instance = null;
	}
	public static boolean hide() {
		Log.d(TAG, "hide ");
		final ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp == null)
			return false;

		((Activity)exp.m_pEngine.mCtx).runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (LayaEditBoxNew.instance != null && !LayaEditBoxNew.instance.mConfirmHold) {
					LayaEditBoxNew.instance.close();
				}
			}
		});
		return true;
	}
	void initView() {
		setLayout();
		setProperties();
		/*m_pEditBoxTouch.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				LayaEditBoxNew.this.close();
			}
		});*/
		/*m_rootLayout.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				LayaEditBoxNew.this.close();
			}
		});*/
		mEditBox.setOnEditorActionListener(new TextView.OnEditorActionListener() {
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				if (LayaEditBoxNew.this.mMultiple) {
					return false;
				}
				else {
					ConchJNI.handleKeyboardConfirm(LayaEditBoxNew.this.mEditBox.getText().toString());
					if (!LayaEditBoxNew.this.mConfirmHold) {
						LayaEditBoxNew.this.close();
					}
					return false;
				}
			}
		});
		mEditBox.addTextChangedListener(new TextWatcher() {
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after) {

			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {

			}

			@Override
			public void afterTextChanged(Editable s) {
				ConchJNI.handleKeyboardInput(s.toString());
			}
		});
		mEditBox.setFilters(new InputFilter[]{new InputFilter.LengthFilter(mMaxLength) });
		mEditBox.requestFocus();
	}
	@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
	public void setLayout() {
		if (mRootLayout != null) {
			LayaConch5.ms_layaConche.getAbsLayout().removeViewInLayout(mRootLayout);
			mRootLayout = null;
		}
		if (true) {
			LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			mRootLayout = (LinearLayout) inflater.inflate(R.layout.editbox_layout, null);
			mEditboxPanel = mRootLayout.findViewById(R.id.editbox_panel);
			mEditboxPanelBg = mRootLayout.findViewById(R.id.editbox_panel_bg);

			RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) mEditboxPanel.getLayoutParams();
			int left = params.leftMargin;
			int top = params.topMargin;
			int right = params.rightMargin;
			int bottom = params.bottomMargin;
			params.setMargins(left,99999, right, bottom);
			mEditboxPanel.setLayoutParams(params);

			mScrollView = mRootLayout.findViewById(R.id.scroll_view);
			mEditBox = mRootLayout.findViewById(R.id.editbox_text);
			//m_pEditBoxTouch = m_rootLayout.findViewById(R.id.editbox_touch);
		}
		LayaConch5.ms_layaConche.getAbsLayout().addView(mRootLayout);

		setScroll();
	}
	private int getScreenOrientation() {
		return ((Activity)mContext).getResources().getConfiguration().orientation;
	}
	public void setProperties() {
		if (!TextUtils.isEmpty(mHint)) {
			mEditBox.setHint(mHint);
		}
		if (!TextUtils.isEmpty(mHintColor)) {
			try {
				mEditBox.setHintTextColor(Color.parseColor(mHintColor));
			}
			catch(Exception e) {

			}

		}
		mEditBox.setText(mDefaultValue);
		switch(mInputType) {
			case "email":
				mEditBox.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
				break;
			case "number":
				mEditBox.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL | InputType.TYPE_NUMBER_FLAG_SIGNED);
				break;
			case "phone":
				mEditBox.setInputType(InputType.TYPE_CLASS_PHONE);
				break;
			case "password":
				mEditBox.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
				break;
			default:
				if (mMultiple) {
					mEditBox.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_MULTI_LINE);
				} else {
					mEditBox.setInputType(InputType.TYPE_CLASS_TEXT);
				}
				break;
		}
		switch(mConfirmType) {
			case "done":
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_DONE | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
			case "next":
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_NEXT | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
			case "search":
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_SEARCH | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
			case "go":
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_GO | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
			case "send":
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_SEND | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
			default:
				mConfirmType = null;
				mEditBox.setImeOptions(EditorInfo.IME_ACTION_UNSPECIFIED | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				break;
		}
		if (mMultiple) {
			mEditBox.setImeOptions(EditorInfo.IME_ACTION_NONE | EditorInfo.IME_FLAG_NO_EXTRACT_UI | EditorInfo.IME_FLAG_NO_ENTER_ACTION);
		}
	}

	//键盘不遮挡按钮
	private void setScroll() {
		mEditboxPanelBg.setOnTouchListener(new View.OnTouchListener() {                 //parent为Editext外面那层布局
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (!LayaEditBoxNew.this.mConfirmHold) {
					LayaEditBoxNew.this.close();
				}
				return false;
			}
		});

		mScrollView.setOnScrollChangeListener(new NestedScrollView.OnScrollChangeListener() {         //scroll为parent外面那层布局（）最好用NestedScrollView，ScrollView会有版本问题
			@Override
			public void onScrollChange(NestedScrollView v, int scrollX, int scrollY, int oldScrollX, int oldScrollY) {
				v.scrollTo(0,450);     //这个是滑动距离，随便大一点就好
			}
		});
	}
	@Override
	public void onSoftKeyboardOpened(int keyboardHeight) {
		if (keyboardHeight >= 0) {
			Point screenSize = new Point();
			((Activity)mContext).getWindowManager().getDefaultDisplay().getSize(screenSize);
			RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) mEditboxPanel.getLayoutParams();
			int left = params.leftMargin;
			int top = params.topMargin;
			int right = params.rightMargin;
			int bottom = params.bottomMargin;
			if (mEditboxPanelHeight == 0) {
				mEditboxPanelHeight = mEditboxPanel.getHeight();
			}
			//int height = m_editbox_panel.getHeight();
			//int orientation = getScreenOrientation();
			//if (orientation != m_orientation) {
			initView();
			//}
			//m_orientation = orientation;
			//if (orientation == Configuration.ORIENTATION_PORTRAIT) {
				int screenHeight = DensityUtils.getScreenHeight(this.mContext);
				params.setMargins(left, screenSize.y - keyboardHeight - mEditboxPanelHeight, right, bottom);
			//}
			//else  {
			//	params.setMargins(left, screenSize.y - m_editbox_panel_height, right, bottom);
			//}
			mEditboxPanel.setLayoutParams(params);
		}
	}
	@Override
	public void onSoftKeyboardClosed() {
		//m_rootLayout.setVisibility(View.INVISIBLE);
		close();
	}
}
