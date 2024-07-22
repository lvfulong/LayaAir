function download(url, cachePkgPath) {
    console.log("start download " + url);
    var lastPercent = -1;
    downloadBigFile(url, cachePkgPath, (total, now, speed) => {
        let percent = Math.floor((now / total) * 100);
        if (percent > lastPercent) {
            console.log('downloading ' + percent, null);
        }
        lastPercent = percent;
        return false;
    }, (curlret, httpret) => {
        if (curlret != 0 || httpret < 200 || httpret >= 300) {
            console.log('download Error ' + httpret + " " + url);
        }
        else {
            console.log('downloaded ccc ' + url);
        }
    }, 10, 100000000);
}


let cachePath = conch.getCachePath();
if (!fs_exists(cachePath)) {
    fs_mkdir(cachePath);
}
let localCachePkg = cachePath + "/package.zip";


let url = "https://lv.layabox.com/test/layame-yanshi/momoyo/0ef1008fbf1866a9d672a68d1dab3e03-202431219224.zip";
download(url + "?" + Date.now(), localCachePkg);