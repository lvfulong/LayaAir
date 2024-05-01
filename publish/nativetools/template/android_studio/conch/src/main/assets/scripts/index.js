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
        if (conch.registerFont(fontName, assetFontData)) {
            log('字体注册成功');
        }
        else {
            log('字体注册失败');
        }
    }
}
function registerAllFonts() {
    registerFont('Arial', 'font/Arial.ttf');
    registerFont('SimHei', 'font/SimHei.ttf');
    registerFont('MicrosoftYaHei', 'font/Microsoft Yahei.ttf');
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
function startApp(data) {
    var jsonobj = null;
    try {
        jsonobj = JSON.parse(data);
    }
    catch (e) {
        console.log("Error:start page parse error! \n " + data);
        return;
    }
    jsonobj.scripts.forEach((v) => {
        var t = document.createElement("script");
        t["src"] = v;
        t.onerror = function () {
            if (window["onLayaInitError"]) {
                window["onLayaInitError"]("Load script error");
            }
        };
        document.head.appendChild(t);
    });
    if (jsonobj.screenOrientation)
        setOrientation(jsonobj.screenOrientation);
    else if (jsonobj.screenorientation)
        setOrientation(jsonobj.screenorientation);
    else
        setOrientation("sensor_landscape");
    document.createElement("script").text = "window.onload&&window.onload()";
}
async function loadApp(url) {
    var urllen = url.length;
    if (urllen < 2)
        return;
    url = url.trim();
    if (url.substring(urllen - 1) === '/')
        url = url + 'runtime.json';
    url = url.replace(/.html$/i, '.json');
    if (url.indexOf('http://stand.alone.version') == 0)
        _inline = false;
    if (!_inline) {
        url = 'http://stand.alone.version/index.js';
    }
    console.log("loadApp:" + url);
    if (history.length <= 0) {
        history._push(url);
    }
    if (url.length < 2)
        return;
    location.setHref(url);
    var urlpath = location.fullpath + '/';
    var cache = null;
    try {
        require("config");
    }
    catch (e) {
    }
    cache = window.appcache = new AppCache(urlpath);
    document.loadCookie();
    registerAllFonts();
    var isDccOk = true;
    async function updateDcc() {
        cache.setResourceID('appurl', urlpath);
        var curassets = cache.getResourceID('netassetsid');
        var assetsidStr = (await asyncs.downloadSync(urlpath + 'update/assetsid.txt?rand=' + Math.random() * Date.now(), false, null));
        console.log("assetsid old:" + curassets + "  new:" + assetsidStr);
        if (!assetsidStr) {
            if (curassets && curassets != "") {
                if (window["onLayaInitError"]) {
                    isDccOk = false;
                    window["onLayaInitError"]("Update DCC get assetsid error");
                }
            }
        }
        else {
            if (curassets != assetsidStr) {
                log('need update;');
                var txtdcc = '';
                var bindcc = await asyncs.downloadSync(urlpath + 'update/filetable.bin?' + assetsidStr, true, null);
                if (!bindcc || !(bindcc instanceof ArrayBuffer)) {
                    txtdcc = (await asyncs.downloadSync(urlpath + 'update/filetable.txt?' + assetsidStr, false, null));
                }
                else {
                    if (bindcc.byteLength % 8 != 0) {
                        log('下载的的filetable.bin的长度不对。是不是错了。');
                    }
                    else {
                        var v = new Uint32Array(bindcc);
                        if (v[0] != 0xffeeddcc || v[1] != 1) {
                            log('dcc.bin file err!');
                        }
                        else {
                            if (v[2] == 0x00ffffff) {
                                var stp = (4 + 8) / 2;
                                var md5int = v.slice(4, 12);
                                var md5char = new Uint8Array(md5int.buffer);
                                var so = String.fromCharCode.apply(null, md5char);
                                console.log('--------------------------------------------');
                                console.log('so=' + so);
                                console.log('netid=' + assetsidStr);
                                if (so == assetsidStr) {
                                    for (var ii = stp, isz = v.length / 2; ii < isz; ii++)
                                        txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                                }
                            }
                            else {
                                console.log('----------------old format');
                                for (var ii = 1, isz = v.length / 2; ii < isz; ii++)
                                    txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                            }
                        }
                    }
                }
                if (txtdcc && txtdcc.length > 0) {
                    cache.saveFileTable(txtdcc);
                    window.appcache = cache = new AppCache(urlpath);
                    cache.setResourceID('netassetsid', assetsidStr);
                }
                else {
                    if (window["onLayaInitError"]) {
                        isDccOk = false;
                        window["onLayaInitError"]("Update DCC get filetable error");
                    }
                }
            }
        }
    }
    if (_inline) {
        await updateDcc();
        if (!isDccOk) {
            console.log("init dcc fail");
            return;
        }
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
        data = cache.loadCachedURL(url);
        if (!data || data.length <= 0) {
            if (window["onLayaInitError"]) {
                window["onLayaInitError"]("Load start url error");
            }
            return;
        }
    }
    console.log("");
    var qpos = url.indexOf('?');
    if (qpos < 0)
        qpos = url.length;
    if (url.substr(qpos - 3, 3) === '.js') {
        window.eval(data + `
        //@ sourceURL=${url}
        `);
        document.createElement("script").text = "window.onload&&window.onload()";
    }
    else if (data.indexOf("<html>") >= 0) {
        startAppHTML(data);
    }
    else {
        startApp(data);
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
            history.back();
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
window.loadConchUrl = loadApp;
window['updateByZip'] = function (url, onEvent, onEnd) {
    let cachePath = conch.getCachePath();
    let localfile = cachePath + url.substr(url.lastIndexOf('/'));
    downloadBigFile(url, localfile, (total, now, speed) => {
        onEvent('downloading', Math.floor((now / total) * 100), null);
        return false;
    }, (curlret, httpret) => {
        if (curlret != 0 || httpret < 200 || httpret >= 300) {
            onEvent('downloadError');
        }
        else {
            onEvent('downloadOK');
            let zip = new ZipFile();
            if (zip.setSrc(localfile)) {
                zip.forEach((id, name, dir, sz) => {
                    if (!dir) {
                        let buf = zip.readFile(id);
                        let fid = window.appcache.hashstr('/' + name);
                        if (window.appcache.updateFile(fid, 0, buf, false)) {
                            onEvent('updating', null, name);
                        }
                        else {
                            onEvent("updateError", null, name);
                        }
                    }
                });
                zip.close();
                if (onEnd)
                    onEnd(localfile);
            }
            else {
                console.log("set zip src error!");
                onEvent('unknownError');
            }
        }
    }, 10, 100000000);
};
function startAppHTML(data) {
    const parser = new DOMParser();
    const doc = parser.parseFromString(data, "text/html");
    const scriptElements = doc.querySelectorAll("script");
    scriptElements.forEach(element => {
        element.attributes.forEach(attri => {
            if (attri.nodeName == "src") {
                var t = document.createElement("script");
                t["src"] = attri.nodeValue;
                t.onerror = function () {
                    if (window["onLayaInitError"]) {
                        window["onLayaInitError"]("Load script error");
                    }
                };
                document.head.appendChild(t);
            }
        });
    });
    document.createElement("script").text = "window.onload&&window.onload()";
}
loadApp(conch.presetUrl || "http://layabox.com/layanative3.0/demo/index.js");
