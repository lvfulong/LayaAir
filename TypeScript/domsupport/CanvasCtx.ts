///<reference path="./LayaConchRuntime.d.ts" />
class CanvasRenderingContext
{
    private canvas:HTMLCanvasElement;
    public gl:LayaGLContext;
    public _width:number;
    public _height:number;
    public _nativeObj:any;
    constructor(c:HTMLCanvasElement) {
        this.canvas = c;
        this.gl = LayaGLContext.instance;
        this._nativeObj = new (window as any).conchCanvasRenderingContext2D(c.width, c.height);
    }
    setSize(w:number,h:number){
        if (this._width != w || this._height != h) {
            this._width = w;
            this._height = h;
            this._nativeObj = new (window as any).conchCanvasRenderingContext2D(w, h);
            if(this.canvas._isFirst)
            {
                this.gl.setMainContextSize(this._width,this._height);
            }
        }
    }
    clear():void{
        this.gl.clearColor(0, 0, 0, 0);
        this.gl.clear(LayaGLContext.COLOR_BUFFER_BIT | LayaGLContext.DEPTH_BUFFER_BIT | LayaGLContext.STENCIL_BUFFER_BIT);
    }
    toBase64(type:string, encoderOptions:number, callback:(data:string)=>void):void{
    }
    drawImage(...args):void
    {

    }
    fillText (text, x, y, maxWidth) {
        this._nativeObj.fillText(text, x, y, maxWidth);
    }
    strokeText (text, x, y, maxWidth) {
        this._nativeObj.strokeText(text, x, y, maxWidth);
    }
    measureText(text)
    {
        return this._nativeObj.measureText(text);
    }
    clearRect(x,y,width,height)
    {
        this._nativeObj.clearRect(x,y,width,height);
    }
    save()
    {
        return this._nativeObj.save();
    }
    restore()
    {
        return this._nativeObj.restore();
    }
    getImageData(x,y,width,height)
    {
        return this._nativeObj.getImageData(x,y,width,height);
    }
    setTransform(a,b,c,d,e,f)
    {
        this._nativeObj.setTransform(a,b,c,d,e,f);
    }
    scale(x,y)
    {
        this._nativeObj.scale(x,y);
    }
    destroy():void {
        this.canvas = null;
        this.gl = null;
    }
    set font(fontName:string) {
        //window["_conchTextCanvas"].font = fontName;
        this._nativeObj.font = fontName;
    }
    get font():string {
        //return window["_conchTextCanvas"].font;
        return this._nativeObj.font;
    }
    set textAlign(textAlign:string) {
        this._nativeObj.textAlign = textAlign;
    }
    get textAlign():string {
        return this._nativeObj.textAlign;
    }
    set textBaseline(textBaseline:string) {
        this._nativeObj.textBaseline = textBaseline;
    }
    get textBaseline():string {
        return this._nativeObj.textBaseline;
    }
    set fillStyle(fillStyle:string) {
        this._nativeObj.fillStyle = fillStyle;
    }
    get fillStyle():string {
        return this._nativeObj.fillStyle;
    }
    set strokeStyle(strokeStyle:string) {
        this._nativeObj.strokeStyle = strokeStyle;
    }
    get strokeStyle():string {
        return this._nativeObj.strokeStyle;
    }
    set lineWidth(lineWidth:number) {
        this._nativeObj.lineWidth = lineWidth;
    }
    get lineWidth():number {
        return this._nativeObj.lineWidth;
    }
    set lineJoin(lineJoin:string) {
        this._nativeObj.lineJoin = lineJoin;
    }
    get lineJoin():string {
        return this._nativeObj.lineJoin;
    }
}
window["CanvasRenderingContext"]=CanvasRenderingContext;