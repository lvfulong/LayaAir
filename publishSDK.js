/*
* node publishSDK.js  inputDir[] outputDir
*/

var crypto = require('crypto');
var fs = require("fs");
var archiver = require("archiver");
const path = require("path");

//递归创建目录 同步方法  
function mkdirsSync(dirname) {  
    if (fs.existsSync(dirname)) {  
      return true;
    } else {  
        if (mkdirsSync(path.dirname(dirname))) {  
            console.log("mkdirsSync = " + dirname);
            fs.mkdirSync(dirname);
            return true;
        }  
    }  
}
function _delete(src) {
  var paths = fs.readdirSync(src)
  paths.forEach(function(p) {
    var _src = path.join(src ,p);
    var stat = fs.statSync(_src)
    if(stat.isFile()) {// 判断是文件还是目录
      fs.unlinkSync(_src);
    } else if(stat.isDirectory()) {
      _delete(_src)// 当是目录是，递归复制
      fs.rmdirSync(_src);
    }
  })
}
function doDelete(src){
    //var src=path.join(curdir,param[1]);
    if(fs.existsSync(src))
    {
      var ext=path.extname(src);
      if(ext)
      {
        fs.unlinkSync(src);
      }
      else
      {
        _delete(src);
        fs.rmdirSync(src);
      }
      return true;
    }
    else
    {
      return false;
    }
}

var publishAll = function(inputDir, outputDir, outputFileName, baseUrl)
{
	//fs.copyFileSync('../dist/index_fenbao.js', '../release/' + dirName + '/index.js');
	
	var outputTempZip = outputDir + "/" + Date.now() + ".zip";
	var zipName = outputFileName + ".zip"
	if (!mkdirsSync(outputDir)) 
	{
		console.log('Error mk outputDir failed');
		return;
	}

	// create a file to stream archive data to.
	var output = fs.createWriteStream(outputTempZip);
	var archive = archiver("zip", {
		zlib: { level: 9 }, // Sets the compression level.
	});

	// pipe archive data to the file
	archive.pipe(output);
	archive.directory(inputDir, outputFileName);
	archive.finalize();

	output.on('close', function() {
		
		var md5 = crypto.createHash('md5').update(fs.readFileSync(outputTempZip)).digest("hex");
		var date = new Date();

		var zipPath = outputDir + "/" + zipName;
		doDelete(zipPath);
	
		mkdirsSync(outputDir)
		fs.copyFileSync(outputTempZip, zipPath);
		console.log('copy ' + outputTempZip + ' to ' + zipPath);
			
		var versionJSONObject = 
		{
			"versionList": 
			[
				{
					"version": outputFileName,
					"url": baseUrl + "/" + zipName,
					"md5": md5
				}
			]
		};
		fs.writeFileSync(outputDir + "/versionconfig.json", JSON.stringify(versionJSONObject));
		console.log("md5 "+ " " + md5);
		fs.unlinkSync(outputTempZip);
	})
}
//publish('binrealse','C:/Users/Administrator/Desktop/testLayaMe', 'http://10.10.30.2:7777/'， 'D:/lvfulong/LayaMeApp/LayaMeApp/Android/template/app/src/main/assets/cache')
publishAll(process.argv[2], process.argv[3], process.argv[4], process.argv[5])