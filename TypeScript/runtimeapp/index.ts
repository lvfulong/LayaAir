//(window as any).conch.app_start_load_js = Date.now() - (window as any).conchConfig.getAppLaunchStartTime();
window._conchInfo = { version: '2.1.3.1' };
var _inline =!conchConfig.localizable; //是否是单机版
console.log('======================================================  ');
console.log('             LAYA CONCH            ');
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion());
console.log('     appVersion:' + conchConfig.getAppVersion());
console.log('     appLocalVersion:' + conchConfig.getAppLocalVersion());
console.log('          jsversion:' + window._conchInfo.version);
console.log('             isplug:' + conchConfig.getIsPlug());
console.log('======================================================');

function log(m) {
    console.log(m);
}
/*if (conchConfig.getOS() == "Conch-ios"){
    require('promise');
}*/

function loadLib(url:string) {
    var script = document.createElement("script");
    /*if(url.indexOf("laya.physics3D.js") >= 0 )
    {
        url = url.replace("laya.physics3D.js","laya.physics3D.runtime.js");
    }*/
    script.src = url;
    script.onerror=function(){
        if(window["onLayaInitError"]){
            window["onLayaInitError"]("Load script error");
        }
    }
    document.head.appendChild(script);
}
window['loadLib']=loadLib;
window['require']=loadLib;
import asyncs = require('async');

/*async function initFreeType() {
    var sOS = conchConfig.getOS();
    var bRet = false;

    //优先使用包中的字体
    //var sTempFontPath = conch.getCachePath() + "/runtimeFont/";
    //if (!fs_exists(sTempFontPath)) {
    //    fs_mkdir(sTempFontPath);
    //}
    //sTempFontPath += "layabox.ttf";
    //bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
    //if (bRet == false) {
        var assetFontData = conch.readFileFromAsset('font/layabox.ttf', 'raw');
        if (assetFontData) {
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromBuffer(assetFontData);
            //fs_writeFileSync(sTempFontPath, assetFontData);
            //bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
        }
    var assetBoldFontData = conch.readFileFromAsset('font/layabox-Bold.ttf', 'raw');
    if (assetBoldFontData) {
        _conchTextCanvas.initFreeTypeDefaultFontFromBuffer(assetBoldFontData);
    }
    var assetFontDataEmoji = <ArrayBuffer>conch.readFileFromAsset('font/NotoEmoji-Regular.ttf', 'raw');
    if (assetFontDataEmoji) {
        _conchTextCanvas.initFreeTypeDefaultFontFromBuffer(assetFontDataEmoji);
    }
    if(!bRet){
        if (sOS == "Conch-window") {
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("C:/Windows/Fonts/simhei.ttf");
        }
        else if (sOS == "Conch-android") {
            var fSystemVersion = navigator.sv;
            if (fSystemVersion >= 2.0 && fSystemVersion < 5.0) {
                //这个步骤为因为HTC部分手机，作出了一个奇葩的决定：将DroidSansFallback架空，引入DFHEIA5A和DFHEIA7A两个字体文件。
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("/system/fonts/DFHEIA5A.ttf");
                if (bRet == false) {
                    bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("/system/fonts/DroidSansFallback.ttf");
                }
            }
            else if (fSystemVersion >= 5.0 && fSystemVersion < 6.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansHans-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 6.0 && fSystemVersion < 7.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansSC-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 7.0 && fSystemVersion < 8.0) {
                bRet = false;
            }
        }
        else if(sOS=="Conch-ios")
        {
            bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile("");
        }
    }
    if (bRet == false) {
        log('字体初始化失败，从网络下载字体...');
        var sTempFontPath = conch.getCachePath() + "/runtimeFont/";
        if (!fs_exists(sTempFontPath)) {
            fs_mkdir(sTempFontPath);
        }
        sTempFontPath += "layabox.ttf";
        var data = <ArrayBuffer>(await asyncs.downloadSync(location.fullpath+'/font/simhei.ttf', true, null));
        if(!data){
            data=<ArrayBuffer>(await asyncs.downloadSync('http://runtime.layabox.com/font/simhei.ttf', true, null));
        }
        if (!data) {
            alert('下载字体失败。 ');
            return;
        }
        fs_writeFileSync(sTempFontPath, data);
        bRet = _conchTextCanvas.initFreeTypeDefaultFontFromFile(sTempFontPath);
    }
    if(!bRet){
        log('字体初始化失败。');
    }
}*/
//initFreeType();
function registerFont(fontName, fileName)
{
    var assetFontData = conch.readFileFromAsset(fileName, 'raw');
    if (assetFontData) {
        conch.registerFont(fontName, assetFontData);
    }
}
function registerAllFonts()
{
    /*var sOS = conchConfig.getOS();
    if(sOS == "Conch-ios") {
        registerFont('Palatino Linotype-Bold', 'font/Palatino Linotype Bold.ttf');
    }
    registerFont('Palatino Linotype', 'font/Palatino Linotype.ttf');
    registerFont('SimHei', 'font/SimHei.ttf');
    registerFont('MicrosoftYaHei', 'font/Microsoft Yahei.ttf');
    registerFont('SimSun', 'font/SimSun.ttc');*/
}
function setOrientation(s: string) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6,sensorLandscape: 6,
        sensor_portrait: 7,sensorPortrait: 7, reverse_landscape: 8,reverseLandscape: 8, reverse_portrait: 9,reversePortrait: 9, full_sensor: 10,fullSensor: 10,
    }
    var nOri = (function (name: string) {
        try {
            var n = nameToVal[name];
            return n || 0;
        } catch (e) {
            return 0;
        }
    })(s);

    //if (conchConfig.getScreenOrientation() != nOri) {
        conchConfig.setScreenOrientation(nOri);;
    //}
}

Object.defineProperty(window,'screenOrientation',{
    get:function(){
        return (window as any).___screenOri;
    },
    set:function(v:string){
        (window as any).___screenOri = v;
        setOrientation(v);
    }
});


/**
 * 
 * @param url 一个js文件，通常是 http://path/to/server/index.js
 * @returns 
 */
async function loadApp(url: string) {
    url = url.trim();

    if(!url||url.length < 2 || url.indexOf('http://stand.alone.version')==0)
        _inline=false;

    console.log("loadApp:" + url||"单机版");

    location.setHref(url);
    var urlpath = location.fullpath + '/';
    try{
        require("config");
    }catch(e){    }

    document.loadCookie();
    registerAllFonts();

    if(_inline){
        //await updateDcc();
    }

    var data = await asyncs.loadText(url);
    for(var n=0; n<3 && !data; n++){
        data = await asyncs.loadText(url);
    }
    if(!data){
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].showTips("NetworkError");
        }
    }    

    console.log("");//此操作在ios下 能起很大的作用  如果注释掉就会非法 不信你试试 但是最后还是得看看是鸡巴毛原因 TODO
    var qpos = url.indexOf('?');
    if(qpos<0) qpos = url.length;

    if (url.substr(qpos - 3,3) === '.js') {
        window.eval(data + `
        //@ sourceURL=${url}
        `);
        document.createElement("script").text="window.onload&&window.onload()";
    }else{
        console.log('url must be a js file');
    }
    if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
        window["loadingView"].hideLoadingView();
    }
}

window.document.addEventListener('keydown', function (e: KeyboardEvent) {
    switch (e.keyCode) {
        case 116://F5
            reloadJS(true);
            break;
        case 117://F6
            break;
        case 118://F7
            //conch.printAllMemorySurvey("","http://10.10.20.114:8889/upload","layabox");
            break;
        case 119://F8
            break;
        case 120:
            gc();
            break;
    }
});

//取url所在目录
function getBaseUrl(url:string){
    let qidx = url.indexOf('?');
    if(qidx>0){
        url = url.substring(0,qidx);
    }
    url = url.substring(0,url.lastIndexOf('/')+1);//保留最后的/
    return url;
}

window.loadConchUrl = loadApp;
var enableDcc2=true;
var appUrl = 'http://layabox.com/layanative3.0/demo/index.js';
var dccHead = '';
//null表示根据规则自己计算
var dccUrl = null;
//null表示根据规则自己计算
var mapToDCC=null;

if(enableDcc2){
    //@ts-ignore
    let layadcc = require('layadcc.js').layadcc;
    let dcc = new layadcc.LayaDCCClient(dccUrl||getBaseUrl(dccHead));
    //设置这个地址下的资源加载走DCC模式
    dcc.pathMapToDCC= mapToDCC||getBaseUrl(appUrl);
    //通过DCC的根文件初始化dcc客户端
    dcc.init(dccHead,null).then((ok:boolean)=>{
        if(ok){
            dcc.injectToNative3();
        }else{
            console.log('init dcc error!');
        }
        //导出以便用户使用
        (window as any).layadcc2 = layadcc;
        (window as any).dcc2 = dcc;
        loadApp(conch.presetUrl||appUrl);
    })
}else{
    loadApp(conch.presetUrl||appUrl);
}

