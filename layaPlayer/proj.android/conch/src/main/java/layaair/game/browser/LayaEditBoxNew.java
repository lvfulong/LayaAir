package layaair.game.browser;

import java.util.regex.Pattern;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.core.widget.NestedScrollView;

import layaair.game.R;
import layaair.game.conch.LayaConch5;

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

	public EditText	m_pEditBox = null;

	//public Button	m_pEditBoxTouch = null;

	public Button	m_pEditBoxButton = null;

	public String m_confirmType = null;

	public String m_inputType = null;

	public boolean m_multiple = false;

	public boolean m_confirmHold = false;

	public String m_defaultValue = null;

	public String m_hint = null;

	public String m_hintColor = null;
	public int	m_maxLength = 0;
	public static final int EditBoxTouchId = 0;

	public static final int EditBoxPanelId = 1;

	public static final int EditBoxButtonId = 2;

	public static final int EditBoxTextId = 3;

	public RelativeLayout.LayoutParams m_editBoxButtonLayoutParams;

	public static LayaEditBoxNew instance = null;

	public Context m_context = null;

	public KeyboardHeightProvider m_keyboardHeightProvider = null;
	public RelativeLayout m_editbox_panel_bg;
	public RelativeLayout m_editbox_panel;
	public KeyboardLayout m_rootLayout = null;
	public int m_orientation;
	private NestedScrollView mScrollView;
	private int m_editbox_panel_height = 0;
	@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
	public LayaEditBoxNew(final String defaultValue, final int maxLength, final boolean multiple, final boolean confirmHold, final String confirmType, final String hint , final String hintColor, final String inputType) {
		m_context = LayaConch5.ms_layaConche.mCtx;
		LayaEditBoxNew.instance = this;
		m_keyboardHeightProvider = new KeyboardHeightProvider((Activity) m_context);
		m_keyboardHeightProvider.start();
		m_keyboardHeightProvider.setKeyboardHeightObserver(this);
		m_confirmType = confirmType;
		m_multiple = multiple;
		m_confirmHold = confirmHold;
		m_defaultValue = defaultValue;
		m_hint = hint;
		m_hintColor = hintColor;
		m_maxLength = maxLength;
		m_inputType = inputType;
		initView();
		m_orientation = getScreenOrientation();
		InputMethodManager imm = (InputMethodManager)m_context.getSystemService(m_context.INPUT_METHOD_SERVICE );
		//imm.showSoftInput(m_pEditBox, InputMethodManager.SHOW_IMPLICIT);
		imm.showSoftInput(m_pEditBox, 0);
		/*if (m_context.getResources().getConfiguration().orientation == ORIENTATION_LANDSCAPE) {
			m_rootLayout.setVisibility(View.VISIBLE);
			m_rootLayout.setX(-10000);
			m_rootLayout.setY(-10000);
		}
		else {*/
		m_rootLayout.setVisibility(View.VISIBLE);
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
		InputMethodManager imm = (InputMethodManager) m_context.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(m_pEditBox.getWindowToken(), 0);
		LayaEditBox.hideSoftKeyBorad();
		ConchJNI.handleKeyboardComplete(m_pEditBox.getText().toString());
		m_rootLayout.setVisibility(View.INVISIBLE);
		LayaConch5.ms_layaConche.getAbsLayout().removeViewInLayout(m_rootLayout);
		m_keyboardHeightProvider.close();
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
				if (LayaEditBoxNew.instance != null) {
					LayaEditBoxNew.instance.close();
				}
			}
		});
		return true;
	}
	void initView() {
		setLayout();
		setProperties();
		//((Activity)m_context).getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);

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
		m_pEditBoxButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				ConchJNI.handleKeyboardConfirm(LayaEditBoxNew.this.m_pEditBox.getText().toString());
				if (!LayaEditBoxNew.this.m_confirmHold) {
					LayaEditBoxNew.this.close();
				}
				LayaEditBoxNew.this.m_pEditBox.setText("");
			}
		});
		m_pEditBox.setOnEditorActionListener(new TextView.OnEditorActionListener() {
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				ConchJNI.handleKeyboardConfirm(LayaEditBoxNew.this.m_pEditBox.getText().toString());
				if (!LayaEditBoxNew.this.m_multiple) {
					LayaEditBoxNew.this.close();
				}
				return false;
			}
		});
		m_pEditBox.addTextChangedListener(new TextWatcher() {
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
		m_pEditBox.setFilters(new InputFilter[]{new InputFilter.LengthFilter(m_maxLength) });
		m_pEditBox.requestFocus();
	}
	@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
	public void setLayout() {
		if (m_rootLayout != null) {
			LayaConch5.ms_layaConche.getAbsLayout().removeViewInLayout(m_rootLayout);
			m_rootLayout.setKeyboardListener(null);
			m_rootLayout = null;
		}
		if (true) {
			LayoutInflater inflater = (LayoutInflater) m_context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			int orientation = getScreenOrientation();
			if (orientation == Configuration.ORIENTATION_PORTRAIT) {
				m_rootLayout = (KeyboardLayout) inflater.inflate(R.layout.editbox_layout, null);
			}
			else {
				m_rootLayout = (KeyboardLayout) inflater.inflate(R.layout.editbox_layout_landscape, null);
			}
			m_editbox_panel = m_rootLayout.findViewById(R.id.editbox_panel);
			m_editbox_panel_bg = m_rootLayout.findViewById(R.id.editbox_panel_bg);

			RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) m_editbox_panel.getLayoutParams();
			int left = params.leftMargin;
			int top = params.topMargin;
			int right = params.rightMargin;
			int bottom = params.bottomMargin;
			params.setMargins(left,99999, right, bottom);
			m_editbox_panel.setLayoutParams(params);

			mScrollView = m_rootLayout.findViewById(R.id.scroll_view);
			m_pEditBox = m_rootLayout.findViewById(R.id.editbox_text);
			//m_pEditBoxTouch = m_rootLayout.findViewById(R.id.editbox_touch);
			m_pEditBoxButton = m_rootLayout.findViewById(R.id.editbox_button);
			/*m_rootLayout.setKeyboardListener(new KeyboardLayout.KeyboardLayoutListener() {
				@Override
				public void onKeyboardStateChanged(boolean isActive, int keyboardHeight) {
					if (isActive && LayaEditBoxNew.instance != null) {
						Point screenSize = new Point();
						((Activity)m_context).getWindowManager().getDefaultDisplay().getSize(screenSize);
						int visiableHeight = screenSize.y - keyboardHeight;
						Log.e(TAG, isActive + " " + keyboardHeight);
						RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) m_editbox_panel.getLayoutParams();
						int left = params.leftMargin;
						int top = params.topMargin;
						int right = params.rightMargin;
						int bottom = params.bottomMargin;
						int orientation = getScreenOrientation();
						if (orientation != m_orientation) {
							Log.e(TAG, "!!!!!!!!!!!!!!!!!!!!!!!");
							initView();
						}
						m_orientation = orientation;
						if (orientation == Configuration.ORIENTATION_PORTRAIT) {
							Log.e(TAG, "!!!!!!!!!!!!!!!!!!!!!!! PORTRAIT");
							params.setMargins(left, visiableHeight - m_editbox_panel.getHeight(), right, bottom);
						}
						else  {
							Log.e(TAG, "!!!!!!!!!!!!!!!!!!!!!!! landscape");
							params.setMargins(left, screenSize.y - m_editbox_panel.getHeight(), right, bottom);
						}
						m_editbox_panel.setLayoutParams(params);
					}
				}
			});*/
		}
		LayaConch5.ms_layaConche.getAbsLayout().addView(m_rootLayout);

		setScroll();
	}
	private int getScreenOrientation() {
		return ((Activity)m_context).getResources().getConfiguration().orientation;
	}
	public void setProperties() {
		if (!TextUtils.isEmpty(m_hint)) {
			m_pEditBox.setHint(m_hint);
		}
		if (!TextUtils.isEmpty(m_hintColor)) {
			try {
				m_pEditBox.setHintTextColor(Color.parseColor(m_hintColor));
			}
			catch(Exception e){

			}

		}
		m_pEditBox.setText(m_defaultValue);

		switch(m_inputType) {
			case "email":
				m_pEditBox.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
				break;
			case "number":
				m_pEditBox.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL | InputType.TYPE_NUMBER_FLAG_SIGNED);
				break;
			case "phone":
				m_pEditBox.setInputType(InputType.TYPE_CLASS_PHONE);
				break;
			case "password":
				m_pEditBox.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
				break;
			default:
				if (m_multiple) {
					m_pEditBox.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_MULTI_LINE);
				} else {
					m_pEditBox.setInputType(InputType.TYPE_CLASS_TEXT);
				}
				break;
		}
		switch(m_confirmType) {
			case "done":
				m_pEditBoxButton.setText(m_context.getResources().getString(R.string.done));
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_DONE);
				break;
			case "next":
				m_pEditBoxButton.setText(m_context.getResources().getString(R.string.next));
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_NEXT);
				break;
			case "search":
				m_pEditBoxButton.setText(m_context.getResources().getString(R.string.search));
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
				break;
			case "go":
				m_pEditBoxButton.setText(m_context.getResources().getString(R.string.go));
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_GO);
				break;
			case "send":
				m_pEditBoxButton.setText(m_context.getResources().getString(R.string.send));
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_SEND);
				break;
			default:
				m_confirmType = null;
				m_pEditBoxButton.setText("");
				m_pEditBox.setImeOptions(EditorInfo.IME_ACTION_UNSPECIFIED);
				break;
		}
		if (TextUtils.isEmpty(m_confirmType)) {
			m_pEditBoxButton.setText("");
			m_pEditBoxButton.setPadding(0,0,0,0);
			m_editBoxButtonLayoutParams = (RelativeLayout.LayoutParams)m_pEditBoxButton.getLayoutParams();
			m_editBoxButtonLayoutParams.setMargins(0,0,0,0);
			m_pEditBoxButton.setLayoutParams(m_editBoxButtonLayoutParams);
			m_pEditBoxButton.setVisibility(View.INVISIBLE);
		}
		else {
			m_pEditBoxButton.setVisibility(View.VISIBLE);
		}
	}
	//键盘不遮挡按钮
	private void setScroll() {
		KeyboardUtil.assistActivity(((Activity)m_context), R.id.scroll_view);       //这个是别人给我的工具类，只用这个会有

		m_editbox_panel_bg.setOnTouchListener(new View.OnTouchListener() {                 //parent为Editext外面那层布局
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				LayaEditBoxNew.this.close();
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
	public void onKeyboardHeightChanged(int visibleHeight, int keyboardHeight, int orientation) {
		Log.d(TAG, "visibleHeight " + visibleHeight + " keyboardHeight " + keyboardHeight);
		if (keyboardHeight >= 0) {
			Point screenSize = new Point();
			((Activity)m_context).getWindowManager().getDefaultDisplay().getSize(screenSize);
			RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) m_editbox_panel.getLayoutParams();
			int left = params.leftMargin;
			int top = params.topMargin;
			int right = params.rightMargin;
			int bottom = params.bottomMargin;
			if (m_editbox_panel_height == 0) {
				m_editbox_panel_height = m_editbox_panel.getHeight();
			}
			//int height = m_editbox_panel.getHeight();
			//int orientation = getScreenOrientation();
			//if (orientation != m_orientation) {
				initView();
			//}
			//m_orientation = orientation;
			if (orientation == Configuration.ORIENTATION_PORTRAIT) {
				params.setMargins(left, visibleHeight - m_editbox_panel_height, right, bottom);
			}
			else  {
				params.setMargins(left, screenSize.y - m_editbox_panel_height, right, bottom);
			}
			m_editbox_panel.setLayoutParams(params);
		}
		else {
			RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) m_editbox_panel.getLayoutParams();
			int left = params.leftMargin;
			int top = params.topMargin;
			int right = params.rightMargin;
			int bottom = params.bottomMargin;
			params.setMargins(left,-99999, right, bottom);
			m_editbox_panel.setLayoutParams(params);
		}
	}
}
