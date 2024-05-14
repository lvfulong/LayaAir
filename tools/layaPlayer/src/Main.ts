const { regClass, property } = Laya;

@regClass()
export class Main extends Laya.Script {

    @property(Laya.Button)
    recentlyButton: Laya.Button;

    @property(Laya.Button)
    debugButton: Laya.Button;

    @property(Laya.Button)
    helpButton: Laya.Button;

    @property(Laya.Button)
    logButton: Laya.Button;

    @property(Laya.Sprite)
    page1: Laya.Sprite;

    @property(Laya.Sprite)
    page2: Laya.Sprite;

    @property(Laya.Text)
    mesg: Laya.Text;

    onStart() {
        this.openScene('recently')
        this.helpButton.on(Laya.Event.CLICK, this, () => {
            console.log('helpButton');
            this.openScene('help')
        });
        this.recentlyButton.on(Laya.Event.CLICK, this, () => {
            console.log('recentlyButton');
            this.openScene('recently')
        });
        this.debugButton.on(Laya.Event.CLICK, this, () => {
            // this.openScene('Wait');
            this.openScene('debug')
            this.debugButton.skin = 'sprites/btn_nomal.png';
        });

        this.logButton.on(Laya.Event.CLICK, this, () => {
            console.log('logButton');
        });

        this.mesg.text = 'Hello World!';
    }

    private openScene(sceneName: string) {
        // Laya.Scene.open(sceneName);
        if(sceneName == "help"){
            this.page1.visible = false;
            this.page2.visible = true;
        }else{
            this.page1.visible = true;
            this.page2.visible = false;
        }
    }
}