function test() {
    var xhr = new XMLHttpRequest();
    xhr
    xhr.open('GET', "http://10.10.20.22:5005?name=myname&psword=xxx", true);
    xhr.onerror = function (e) {
        alert('onerr');
    }
    xhr.onload = function (e) {
        alert('onload' + xhr.responseText);
        aaa();
    }
    xhr.send('');
}

test();