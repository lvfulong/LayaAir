class loadingView
{
    constructor() 
	{
		this.sOS = conchConfig.getOS();
		if (this.sOS == "Conch-ios")
		{
			this.bridge = PlatformClass.createClass("JSBridge");
		}
		else if (this.sOS == "Conch-android")
		{
			this.bridge = PlatformClass.createClass("demo.JSBridge");
		}
    }
    set loadingAutoClose(value) 
	{
        this._loadingAutoClose = value;
    }
	get loadingAutoClose()
	{
		return this._loadingAutoClose;
	}
	set showTextInfo(value) 
	{
        this._showTextInfo = value;
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("showTextInfo:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("showTextInfo",value);
			}
		}
    }
	get showTextInfo()
	{
		return this._showTextInfo;
	}
	bgColor(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("bgColor:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("bgColor",value);
			}
		}
	}
	setFontColor(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("setFontColor:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("setFontColor",value);
			}
		}
	}
	showTips(value)
	{
	    if(this.bridge)
     	{
     		if (this.sOS == "Conch-ios")
     		{
     			this.bridge.call("showTips:",value);
     		}
     		else if(this.sOS == "Conch-android")
     		{
     			this.bridge.call("showTips",value);
     		}
     	}
	}
	loading(value)
	{
		if(this.bridge)
		{
			if (this.sOS == "Conch-ios")
			{
				this.bridge.call("loading:",value);
			}
			else if(this.sOS == "Conch-android")
			{
				this.bridge.call("loading",value);
			}
		}
	}
	hideLoadingView()
	{
		this.bridge.call("hideSplash");
	}
}
window.loadingView = new loadingView();
if(window.loadingView)
{
    window.loadingView.loadingAutoClose=true;//true代表当动画播放完毕，自动进入游戏。false为开发者手动控制
    window.loadingView.bgColor("#ffffff");//设置背景颜色
    window.loadingView.setFontColor("#000000");//设置字体颜色
    window.loadingView.loading(-1);
}
window.onLayaInitError=function(e)
{
	console.log("onLayaInitError error=" + e);
	alert("加载游戏失败，可能由于您的网络不稳定，请退出重进");
}

var setting_language = window.localStorage.getItem("setting_language");
if (setting_language) {
    conchConfig.setLanguage(setting_language);
}
else {
    	let getQueryStringConchConfig = function(name) {
            if (!window.location || !window.location.search)
                return null;
            var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)");
            var r = window.location.search.substring(1).match(reg);
            if (r != null)
                return unescape(r[2]);
            return null;
        }
    	let amazon = getQueryStringConchConfig("amazon")
    	if (amazon) {
    	    conchConfig.setLanguage("en");
    	}
    	else {
    	    conchConfig.setLanguage("zh");
    	}
}
window.hideSplashScreen = function() {
    window.loadingView.hideLoadingView();
}
