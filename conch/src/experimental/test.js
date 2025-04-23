
var TEST_FUNCTION_ID;
(function (TEST_FUNCTION_ID) {
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testInt32"] = 0] = "testInt32";  
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testFloat32"] = 1] = "testFloat32";
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testString"] = 2] = "testString";
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testBoolean"] = 3] = "testBoolean";
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testArrayBuffer"] = 4] = "testArrayBuffer";
    TEST_FUNCTION_ID[TEST_FUNCTION_ID["testArrayBufferView"] = 5] = "testArrayBufferView";  
})(TEST_FUNCTION_ID || (TEST_FUNCTION_ID = {}));







//先不考虑超过512字节的情况   
window["commandStreamBuffer"] = new ArrayBuffer(512);
window["commandStreamInt32Array"] = new Int32Array(window["commandStreamBuffer"]);  
window["commandStreamFloat32Array"] = new Float32Array(window["commandStreamBuffer"]);      
window["commandStreamUint8Array"] = new Uint8Array(window["commandStreamBuffer"]);  
//window["commandStreamFloat64Array"] = new Float64Array(window["commandStreamBuffer"]);
window["commandStreamInt32Array"][0] = 1;//int32的个数
test.setBuffer(window["commandStreamBuffer"]);

function test_add_i(i) {
    window["commandStreamInt32Array"][window["commandStreamInt32Array"][0]++] = i;
}
function test_add_f(a) { 
    window["commandStreamFloat32Array"][window["commandStreamInt32Array"][0]++] = a;
}
function test_add_String(str) {
    var ab = conch.strTobufer(str);
    var len = ab.byteLength;
    //this._need(len + 4);
    window["commandStreamInt32Array"][window["commandStreamInt32Array"][0]++] = len;
    if (len == 0)
        return;
    var uint8array = new Uint8Array(ab);
    window["commandStreamUint8Array"].set(uint8array,this.commandStreamInt32Array[0] * 4);
    this.commandStreamInt32Array[0] += len / 4;
}
function test_wab(arraybuffer, length, nAlignLength, offset) {   
    offset = offset ? offset : 0;
    //this._need(nAlignLength + 4);
    window["commandStreamInt32Array"][window["commandStreamInt32Array"][0]++] = length;
    var uint8array = null;
    if (arraybuffer instanceof Float32Array && offset == 0) {
        this.commandStreamFloat32Array.set(arraybuffer, this.commandStreamInt32Array[0]);
    }
    else {
        if (arraybuffer instanceof ArrayBuffer) {
            uint8array = new Uint8Array(arraybuffer, offset, length);
        }
        else if (arraybuffer.buffer) {
            uint8array = new Uint8Array(arraybuffer.buffer, offset + arraybuffer.byteOffset, length);
        }
        else {
            console.log("not arraybuffer/dataview");
            return;
        }
        this.commandStreamUint8Array.set(uint8array, this.commandStreamInt32Array[0] * 4);
    }
    this.commandStreamInt32Array[0] += nAlignLength / 4;
}
// 测试命令缓冲区
//缺点
//不支持返回值
//维护性差
class TestCommandBuffer {
    objectID;
    constructor() {
        this.objectID = test.createObject("TestCommandBuffer");
    }
    destroy() {
        test.destroyObject("TestCommandBuffer", this.objectID);
    }
    getAlignLength(data) {
        var byteLength = data.byteLength;
        return (byteLength + 3) & 0xfffffffc;
    }
    testInt32(value) {
        test_add_i(TEST_FUNCTION_ID.testInt32);
        test_add_i(this.objectID);
        test_add_i(value);
    }   
    testFloat32 (value) {
        test_add_i(TEST_FUNCTION_ID.testFloat32);
        test_add_i(this.objectID);
        test_add_f(value);
    }
    testString(value) {
        test_add_i(TEST_FUNCTION_ID.testString); 
        test_add_i(this.objectID);
        test_add_String(value);   
    }   
    testBoolean(value) {
        test_add_i(TEST_FUNCTION_ID.testBoolean);
        test_add_i(this.objectID);
        test_add_i(value ? 1 : 0);
    }
    testArrayBuffer(value) {
        var nAlignLength = this.getAlignLength(value);
        test_add_i(TEST_FUNCTION_ID.testArrayBuffer);
        test_add_i(this.objectID);
        test_wab(value, value.byteLength, nAlignLength);
    }
    testArrayBufferView(value) {
        var nAlignLength = this.getAlignLength(value);
        test_add_i(TEST_FUNCTION_ID.testArrayBufferView);
        test_add_i(this.objectID);
        test_wab(value, value.byteLength, nAlignLength);
    }
    flush() {
        test.flush();
    }   
}
let maxNum = 1000000;
let startTime = Date.now();

let commandBuffer = new TestCommandBuffer();
for (let i = 0; i < maxNum; i++) {        
    commandBuffer.testInt32(i);
    commandBuffer.testFloat32(i);
    commandBuffer.testString("Hello, world!");
    commandBuffer.testBoolean(true);
    commandBuffer.testArrayBuffer(new ArrayBuffer(10));
    commandBuffer.testArrayBufferView(new Uint8Array(10));
    commandBuffer.flush();
}
commandBuffer.destroy();
let endTime = Date.now();
console.log(`TestCommandBuffer Time taken: ${endTime - startTime}ms`);

startTime = Date.now();
let normal = new TestNormal();
for (let i = 0; i < maxNum; i++) {
    normal.testInt32(i);
    normal.testFloat(i);
    normal.testString("Hello, world!");
    normal.testBoolean(true);
    normal.testArrayBuffer(new ArrayBuffer(10));
    normal.testArrayBufferView(new Uint8Array(10));
    //normal.testFlush();
}       
normal.destroy();
endTime = Date.now();
console.log(`TestNormal Time taken: ${endTime - startTime}ms`); 





