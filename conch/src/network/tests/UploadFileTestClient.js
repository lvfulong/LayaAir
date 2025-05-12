var tasks = [];

var data = conch.readFileFromAsset('scripts/async.js', 'raw');
var path = conch.getCachePath() + '/async.js';
fs_writeFileSync(path, data);
tasks.push(path);

data = conch.readFileFromAsset('scripts/runtimeInit.js', 'raw');
path = conch.getCachePath() + '/runtimeInit.js';
fs_writeFileSync(path, data);
tasks.push(path);

for (var i = 0; i < tasks.length; i++) {
  var task = conch.uploadFile({
    url: 'http://10.1.127.98:3000/upload',
    filePath: tasks[i],
    name: 'file',
    formData: {
      'user': 'test'
    },
    success(res) {
      console.log("js success " + res.data + " " + res.statusCode);
    },
    fail(code, errorMessage) {
      console.log("js fail " + code + " " + errorMessage);
    },
    complete() {
      console.log("js complete");
    }
  })
}