function test1() {
    var xhr = new XMLHttpRequest();
    xhr.responseType = 'text';
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.open('POST', "http://10.10.20.22:5005", true);
    xhr.onerror = function (e) {
        alert('onerr');
    }
    xhr.onload = function (e) {
        alert('onload1 ' + xhr.responseText);
    }
    xhr.send('name=myname&psword=xxx');
}
function test2() {

    let data = {};
    data['preform_id'] = 1;
    data["performance"] = 99;
    data['preform_name'] = 'name';
    var xhr = new XMLHttpRequest();
    xhr.responseType = 'text';
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.open('POST', "http://10.10.20.22:5005", true);
    xhr.onerror = function (e) {
        alert('onerr');
    }
    xhr.onload = function (e) {
        alert('onload2 ' + xhr.responseText);
    }
    xhr.send(JSON.stringify(data));
    
}
test1();
test2();