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
function registerFont() {
    var assetFontData = conch.readFileFromAsset('font/layabox.ttf', 'raw');
    if (assetFontData) {
        if (conch.registerFont("layabox", assetFontData)) {
            log('字体注册成功');
        }
        else {
            log('字体注册失败');
        }
    }
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
function startSmallGame(localfile) {
    var zip = new ZipFile();
    if (!zip.setSrc(localfile)) {
        console.log("Error : load local package error! \n");
        return;
    }
    conch.setZipPackage(zip);
    function loadLib(url) {
        var qpos = url.indexOf('?');
        if (qpos < 0) {
            qpos = url.length;
        }
        url = url.substr(0, qpos);
        if (url.indexOf('./') == 0) {
            url = url.replace("./", "");
        }
        var script = document.createElement("script");
        var text = zip.readAsTextByName(url);
        if (text) {
            console.log("local file: " + url + " loaded success");
            script["text"] = text;
            script.onerror = function () {
                console.log("loadLib: " + url + " failed");
                if (window["loadingView"]) {
                    window["loadingView"].setFontColor("#FF0000");
                    window["loadingView"].showTips("InternalError");
                }
            };
            document.head.appendChild(script);
        }
        else {
            console.log("local file: " + url + " loaded failed");
            if (window["loadingView"]) {
                window["loadingView"].setFontColor("#FF0000");
                window["loadingView"].showTips("InternalError");
            }
        }
        document.head.appendChild(script);
    }
    window['loadLib'] = loadLib;
    var data = zip.readAsTextByName('index.js');
    if (!data) {
        console.log("Error : load index.js error! \n ");
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].showTips("InternalError");
        }
        return;
    }
    window.eval(data + `
    //@ sourceURL=index.js
    `);
    document.createElement("script").text = "window.onload&&window.onload()";
}
var lastPercent = -1;
function download(url, cachePkgPath, md5) {
    console.log("start download " + url);
    downloadBigFile(url, cachePkgPath, (total, now, speed) => {
        let percent = Math.floor((now / total) * 100);
        if (percent > lastPercent) {
            console.log('downloading' + percent, null);
            if (window["loadingView"]) {
                window["loadingView"].loading(percent);
            }
        }
        lastPercent = percent;
        return false;
    }, (curlret, httpret) => {
        if (curlret != 0 || httpret < 200 || httpret >= 300) {
            console.log('downloadError ' + httpret + " " + url);
            if (window["loadingView"]) {
                window["loadingView"].setFontColor("#FF0000");
                window["loadingView"].showTips("NetworkError");
            }
            return;
        }
        else {
            console.log('downloaded ' + url);
            let thisMd5 = calcmd5(fs_readFileSync(cachePkgPath));
            console.log('check downloaded md5 ' + md5);
            if (thisMd5 !== md5) {
                if (window["loadingView"]) {
                    window["loadingView"].setFontColor("#FF0000");
                    window["loadingView"].showTips("DownloadError");
                }
            }
            else {
                startSmallGame(cachePkgPath);
            }
        }
    }, 10, 100000000);
}
async function loadSmallGame(url) {
    window["conchUseWXAdapter"] = true;
    let urllen = url.length;
    if (urllen < 2)
        return;
    url = url.trim();
    console.log("loadSmallGame:" + url);
    if (history.length <= 0) {
        history._push(url);
    }
    let tempLocation = new Location;
    tempLocation.setHref(url);
    location._search = tempLocation._search;
    window.localStorage.create(url);
    try {
        require("config");
    }
    catch (e) {
    }
    let data = await asyncs.loadText(url + "&" + Date.now());
    for (let n = 0; n < 3 && !data; n++) {
        data = await asyncs.loadText(url + "&" + Date.now());
    }
    if (!data) {
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].showTips("NetworkError");
        }
        if (!data || data.length <= 0) {
            return;
        }
    }
    class versionItem {
    }
    let jsonobj = null;
    try {
        jsonobj = JSON.parse(data);
    }
    catch (e) {
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].showTips("ParseJsonError");
        }
        return;
    }
    let item = jsonobj.versionList[0];
    let cachePath = conch.getCachePath();
    if (!fs_exists(cachePath)) {
        fs_mkdir(cachePath);
    }
    let localCachePkg = cachePath + "/package.zip";
    if (!fs_exists(localCachePkg)) {
        var assetPkgData = conch.readFileFromAsset('cache/package.zip', 'raw');
        if (!assetPkgData) {
            console.log('start to download new package');
            download(item.url + "?" + Date.now(), localCachePkg, item.md5);
        }
        else {
            let md5 = calcmd5(assetPkgData);
            console.log('asset package md5 ' + md5 + ' ' + item.md5);
            if (md5 !== item.md5) {
                console.log('start to download new package');
                download(item.url + "?" + Date.now(), localCachePkg, item.md5);
            }
            else {
                console.log('start with asset package');
                fs_writeFileSync(localCachePkg, assetPkgData);
                startSmallGame(localCachePkg);
            }
        }
    }
    else {
        let md5 = calcmd5(fs_readFileSync(localCachePkg));
        console.log('md5 ' + md5 + ' ' + item.md5);
        if (md5 !== item.md5) {
            console.log('start to download new package');
            fs_rm(localCachePkg);
            download(item.url + "?" + Date.now(), localCachePkg, item.md5);
        }
        else {
            startSmallGame(localCachePkg);
            console.log('start with cached package');
        }
    }
}
async function loadSmallGameLocal(url) {
    window["conchUseWXAdapter"] = true;
    let urllen = url.length;
    if (urllen < 2)
        return;
    url = url.trim();
    console.log("loadSmallGame:" + url);
    if (history.length <= 0) {
        history._push(url);
    }
    let tempLocation = new Location;
    tempLocation.setHref(url);
    location._search = tempLocation._search;
    window.localStorage.create(url);
    try {
        require("config");
    }
    catch (e) {
    }
    let cachePath = conch.getCachePath();
    let localCachePkg = cachePath + "/package.zip";
    if (!fs_exists(cachePath)) {
        fs_mkdir(cachePath);
    }
    var assetPkgData = conch.readFileFromAsset('cache/package.zip', 'raw');
    if (!assetPkgData) {
        console.log('error find no package');
    }
    else {
        console.log('start with asset package');
        fs_writeFileSync(localCachePkg, assetPkgData);
        startSmallGame(localCachePkg);
    }
    let md5 = calcmd5(fs_readFileSync(localCachePkg));
    console.log('md5 ' + md5);
}
loadApp("https://lv.layabox.com/test/lfl/bin/index.js");
