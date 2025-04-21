
class WindowSessionStorage  {
    getItem(i:string)
    {
        return this[i]||null;
    }
    setItem(i:string,b:Object)
    {
        this[i]=b;
    }
    
    //todo  sessionStorage 
}



class _Cookie{
    key:string;
    value:string;
    domain:string;
    expires:Date;
    static cookies:Array<_Cookie>=[];
    static sreg:RegExp=new RegExp("([^=]*)\\s*=\\s*([^;]*)\\s*;\\s*(expires\\s*=\\s*(.{23,26}GMT)|)");
    
    constructor(){
        this.domain="";
    }

    static addCookie(s:string):_Cookie{
       var result= _Cookie.sreg.exec(s);
       if(result==null){
           console.warn("设置cookie无效");
           return null;
       }
       else{
           var temp:_Cookie=new _Cookie();
           temp.key=result[1];
           temp.value=result[2];
           if(result[4]){
             temp.expires=new Date(result[4]);
           }
           return temp;
       }
    }
      
    static pushCookie(c:_Cookie):boolean{
        //TO-DO
        if(!c.isValid())return false;
        for(var i:number=0,len:number=_Cookie.cookies.length;i<len;i++)
        {
            var temp:_Cookie=_Cookie.cookies[i];
            if(temp.key==c.key){
                _Cookie.cookies[i]=c;
                return true;
            }
        }
        _Cookie.cookies.push(c);
        return true;
    } 
    
    static flush():void
    {
        writeStrFileSync(document._cookiePath,_Cookie.getString("in","\""));
    }
    
    static getString(type:string,joinstr:string):string{
        var a:Array<string>=[];
        for(var i:number=0,len:number=_Cookie.cookies.length;i<len;i++)
        {
            var temp:_Cookie=_Cookie.cookies[i];
            if(temp.isValid()){
                a.push(temp.toLocalString(type));
            }
            else{
                _Cookie.cookies.slice(i,1);
                i--;
                len--;
            }
        }
        return a.join(joinstr);
    }
    
    static toLocalString():string{
       return _Cookie.getString("out","; ");
    }

  
    isValid():boolean{
       if(this.expires&&(this.expires.getTime()<Date.now())){
           return false;
       }
       return true;
    }
    
    static init(s:string){
        if(s)
        {
            var temp:Array<string>=s.split("\"");
            for(var i:number=0,len:number=temp.length;i<len;i++){
                 var t:_Cookie= _Cookie.addCookie(temp[i]);
                 console.warn(">>>>>>>>>>addCookie"+temp[i]);
                 if(t)
                 {
                     t.value=decodeURIComponent(t.value);
                     t.domain=decodeURIComponent(t.domain);
                 }
                 _Cookie.pushCookie(t);   
            }
        }
    }
    
    toLocalString(type:string):string{
        switch (type) {
            case "in":
                if (this.expires) {
                    return this.key+"="+encodeURIComponent(this.value)+"; expires="+this.expires.toGMTString()+"; domain="+encodeURIComponent(this.domain);
                }
                else {
                    return this.key+"="+encodeURIComponent(this.value) + "; domain="+encodeURIComponent(this.domain);
                }
            case "out":
                return this.key+"="+this.value;
            default:
                return null;
        }
    }
}