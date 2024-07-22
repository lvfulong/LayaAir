const http = require('http');
const querystring = require('querystring'); //导入querystring模块
const server = http.createServer((req, res) => {
    if (req.method === "POST") {
        let postData = '';
        //流string的方式
        req.on('data', chunk => {
            postData += chunk.toString();
         })
        req.on('end', () => { 
            console.log('postData = ', postData);
            res.end("收到，over");
        })
        console.log("post data header", req.headers);
    }
});
server.listen(5005, () => {
    console.log('server running at port 5005');
});
