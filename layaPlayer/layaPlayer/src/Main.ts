const { regClass, property } = Laya;

@regClass()
export class Main extends Laya.Script {

    @property(Laya.Button)
    recentlyButton: Laya.Button;

    @property(Laya.Button)
    debugButton: Laya.Button;

    @property(Laya.Button)
    helpButton: Laya.Button;

    @property(Laya.Button)
    logButton: Laya.Button;

    @property(Laya.Sprite)
    page1: Laya.Sprite;

    @property(Laya.Sprite)
    page2: Laya.Sprite;

    @property(Laya.Text)
    mesg: Laya.Text;
    strDBPath: string;
    db: any;
	bridge: any;
	sOS: string;
    onStart() {
		this.sOS = (window as any).conchConfig.getOS();
        if (this.sOS == "Conch-ios") 
        {
            this.bridge = (window as any).PlatformClass.createClass("JSBridge");
        }
        else if (this.sOS == "Conch-android")
        {
            this.bridge = (window as any).PlatformClass.createClass("demo.layaPlayer.JSBridge");
        }
        this.strDBPath = (window as any).conchConfig.getStoragePath() + "/db.txt";
		this.mesg.text = "RuntimeVersion: " + (window as any).conchConfig.getRuntimeVersion();
		var me = this;
		if (this.sOS == "Conch-ios") {
			this.mesg.text += "\n " + "AppVersion: " + (window as any).conchConfig.getAppVersion();
		}
		else if (this.sOS == "Conch-android") {
			this.bridge.callWithBack(function(ip: any){
				me.mesg.text += "\n " + ip + " Port: " + (window as any).conchConfig.JSDebugPort;
			},"getIP");
			this.mesg.text += "\n " + "AppVersion: " + (window as any).conchConfig.getAppVersion();
		}

		if (this.sOS == "Conch-ios") {
			this.bridge.call("showFloatPanel:", false);
		}
		else if (this.sOS=="Conch-android"){
			this.bridge.call("showFloatPanel", false);
		}

		this.fetchDB();

		if (this.db['debugMode']) {
			this.refreshDebugModeUI(this.db['debugMode']);
		}
		else {
			this.refreshDebugModeUI((window as any).conchConfig.JSDebugMode);
		}

        (window as any).layaPlayerOnScanResult = function(url: any) {
            if (me.sOS == "Conch-ios"){
                me.bridge.call('showFloatPanel:', true);
            }
            else if (me.sOS == "Conch-android") {
                me.bridge.call('showFloatPanel', true);
            }
            (window as any).location.href = url;
            me.saveURLToDB(url);
            me.waitModeAlert();
        }

        

        this.openScene('recently')
        this.helpButton.on(Laya.Event.CLICK, this, () => {
            console.log('helpButton');
            this.openScene('help')
        });
        this.recentlyButton.on(Laya.Event.CLICK, this, () => {
            console.log('recentlyButton');
            this.openScene('recently')
            this.fetchDB();
		    if (this.db['url']) {
			    var url = this.db['url'];
			    window.location.href = url;
			    if (this.sOS == "Conch-ios"){
				    this.bridge.call('showFloatPanel:',true);
			    }
			    else if (this.sOS == "Conch-android"){
				    this.bridge.call('showFloatPanel',true);
			    }
			    this.waitModeAlert();
		    }
		    else {
			    alert("Rencently record is empty,Please click the scan button!");
		    }
        });
        this.debugButton.on(Laya.Event.CLICK, this, () => {
            // this.openScene('Wait');
            this.openScene('debug')
            var sOS = (window as any).conchConfig.getOS();
		    if (sOS == "Conch-ios"){
			    alert('JS Debug Not Supported!');
		    }
            this.page2.visible = false;
            this.saveDebugModeToDB();
        });

        this.logButton.on(Laya.Event.CLICK, this, () => {
            console.log('logButton');
		    var sOS = (window as any).conchConfig.getOS();
		    if (sOS == "Conch-ios") {
			    this.bridge.call("showScanner:", true);
		    }
		    else if (sOS == "Conch-android"){
			    this.bridge.call("showScanner", true);
		    }
        });
    }
    private waitModeAlert() {
		if ((window as any).conchConfig.JSDebugMode == 2) {
			if (this.sOS == "Conch-android"){
				alert("Please Connect Chrome Debugger");
			}
		}
	}
    private saveURLToDB(url: string){
		this.db['url']=url;
		this.pushDB();
	}
    private fetchDB() {
		var strdb = (window as any).readFileSync(this.strDBPath,'utf8') || '{}';
		console.log(strdb);
		this.db = JSON.parse(strdb);
	}

	private pushDB() {
		var strdb = JSON.stringify(this.db);
		console.log(strdb);
		(window as any).writeStrFileSync(this.strDBPath, strdb);
	}
    private saveDebugModeToDB() {
		if (this.debugButton.skin == 'sprites/btn_nomal.png') {
			this.db['debugMode'] = 2;
			this.refreshDebugModeUI(2);
		}
		else if (this.debugButton.skin == 'sprites/btn_wait.png') {
			this.db['debugMode'] = 1;
			this.refreshDebugModeUI(1);
		}
		this.pushDB();
	}

    private refreshDebugModeUI(mode: number) {
        /*if (mode == 0) {
			this.debugButton.skin = 'sprites/btn_off.png';
            this.debugButton.disabled = false;
			(window as any).conchConfig.JSDebugMode = 0;
		}
		else */if(mode == 1) {
			this.debugButton.skin = 'sprites/btn_nomal.png';
            this.debugButton.disabled = false;
			(window as any).conchConfig.JSDebugMode = 1;
		}
		else if (mode == 2) {
			this.debugButton.skin = 'sprites/btn_wait.png';
            this.debugButton.disabled = false;
			(window as any).conchConfig.JSDebugMode = 2;
		}
		else {
            this.debugButton.skin = 'sprites/btn_nomal.png';
            this.debugButton.disabled = false;
			(window as any).conchConfig.JSDebugMode = 1;
		}
    }
    private openScene(sceneName: string) {
        // Laya.Scene.open(sceneName);
        if(sceneName == "help"){
            if (this.page2.visible) {
                this.page1.visible = true;
                this.page2.visible = false;
                this.mesg.visible = true;
            }
            else {
                this.page1.visible = false;
                this.page2.visible = true;
                this.mesg.visible = false;
            }
        }
        else {
            this.page1.visible = true;
            this.page2.visible = false;
            this.mesg.visible = true;
        }
    }
}