const http = require('http');
const querystring = require('querystring'); //导入querystring模块
const server = http.createServer((req, res) => {
    const method = req.method; //请求方式
    const url = req.url; //url
     const path = url.split('?')[0] //路径
    console.log('mothod = ', method,',url = ',url);
    req.query = querystring.parse(url.split('?')[1]); //参数
    console.log('query = ', req.query);
    res.end(
        JSON.stringify(req.query) //显示传的参数以json串的形式
    );
});
server.listen(5005, () => {
    console.log('server running at port 5005');
});
