"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
window._conchInfo = { version: '2.1.3.1' };
var _inline = !conchConfig.localizable;
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
function loadLib(url) {
    var script = document.createElement("script");
    script.src = url;
    script.onerror = function () {
        if (window["onLayaInitError"]) {
            window["onLayaInitError"]("Load script error");
        }
    };
    document.head.appendChild(script);
}
window['loadLib'] = loadLib;
window['require'] = loadLib;
const asyncs = require("async");
function registerFont(fontName, fileName) {
    var assetFontData = conch.readFileFromAsset(fileName, 'raw');
    if (assetFontData) {
        conch.registerFont(fontName, assetFontData);
    }
}
function registerAllFonts() {
}
function setOrientation(s) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6, sensorLandscape: 6,
        sensor_portrait: 7, sensorPortrait: 7, reverse_landscape: 8, reverseLandscape: 8, reverse_portrait: 9, reversePortrait: 9, full_sensor: 10, fullSensor: 10,
    };
    var nOri = (function (name) {
        try {
            var n = nameToVal[name];
            return n || 0;
        }
        catch (e) {
            return 0;
        }
    })(s);
    conchConfig.setScreenOrientation(nOri);
    ;
}
Object.defineProperty(window, 'screenOrientation', {
    get: function () {
        return window.___screenOri;
    },
    set: function (v) {
        window.___screenOri = v;
        setOrientation(v);
    }
});
async function loadApp(url) {
    url = url.trim();
    if (!url || url.length < 2 || url.indexOf('http://stand.alone.version') == 0)
        _inline = false;
    console.log("loadApp:" + url || "单机版");
    location.setHref(url);
    var urlpath = location.fullpath + '/';
    try {
        require("config");
    }
    catch (e) { }
    document.loadCookie();
    registerAllFonts();
    if (_inline) {
    }
    var data = await asyncs.loadText(url);
    for (var n = 0; n < 3 && !data; n++) {
        data = await asyncs.loadText(url);
    }
    if (!data) {
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].showTips("NetworkError");
        }
    }
    console.log("");
    var qpos = url.indexOf('?');
    if (qpos < 0)
        qpos = url.length;
    if (url.substr(qpos - 3, 3) === '.js' || data.indexOf("loadLib(") >= 0) {
        window.eval(data + `
        //@ sourceURL=${url}
        `);
        document.createElement("script").text = "window.onload&&window.onload()";
    }
    else {
        console.log('url must be a js file');
    }
    if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
        window["loadingView"].hideLoadingView();
    }
}
window.document.addEventListener('keydown', function (e) {
    switch (e.keyCode) {
        case 116:
            reloadJS(true);
            break;
        case 117:
            break;
        case 118:
            break;
        case 119:
            break;
        case 120:
            gc();
            break;
    }
});
function getBaseUrl(url) {
    let qidx = url.indexOf('?');
    if (qidx > 0) {
        url = url.substring(0, qidx);
    }
    url = url.substring(0, url.lastIndexOf('/') + 1);
    return url;
}
window.loadConchUrl = loadApp;
var enableDcc2 = true;
var appUrl = 'http://layabox.com/layanative3.0/demo/index.js';
var dccHead = '';
var dccUrl = null;
var mapToDCC = null;
if (enableDcc2) {
    let layadcc = require('layadcc.js').layadcc;
    let dcc = new layadcc.LayaDCCClient(dccUrl || getBaseUrl(dccHead));
    dcc.pathMapToDCC = mapToDCC || getBaseUrl(appUrl);
    dcc.init(dccHead, null).then((ok) => {
        if (ok) {
            dcc.injectToNative3();
        }
        else {
            console.log('init dcc error!');
        }
        window.layadcc = layadcc;
        window.dcc = dcc;
        loadApp(conch.presetUrl || appUrl);
    });
}
else {
    loadApp(conch.presetUrl || appUrl);
}
