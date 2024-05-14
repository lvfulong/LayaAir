
class IDCCInterface{
    virtual void download(const char* pszUrl)=0;
    virtual void loadSync()=0;
    virtual void hasLocal()=0;
};