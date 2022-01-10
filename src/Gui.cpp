#include "Vga.h"
#include "Kbc.h"
#include "Gui.h"
#include "Font.h"
#include "Mouse.h"

using namespace std;

const int MOVE_SCALE           = 5;
const int WIDTH_DISPLAY_SCALE  = 1;
const int HEIGHT_DISPLAY_SCALE = 2;

const int MAX_WIDTH            = 1280;
const int MAX_HEIGHT           = 1024;

class Gui::Impl{
    public:
        Pixel* image = NULL;
        bool grab;
        void Update();
        void Update(const int x, const int y, const int w, const int h);
        bool quit = false;
        int mouse_x, mouse_y;
        Vga* vga = NULL;
        Kbc* kbc = NULL;
        Mouse* mouse = NULL;
        Pixel* vram_mem = NULL;
        int screen_width;
        int screen_height;
        SDL_Window* window = NULL;
        SDL_Surface* screenSurface = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Texture *texture   = NULL;
        SDL_DisplayMode display_mode;
        Mix_Music *music;
        void HandleKeyDown(SDL_Event& e);
        void HandleKeyUp(SDL_Event& e);
        uint8_t SdlScancode2KeyCode(SDL_Event& e);
        void HandleMouseMotion(SDL_Event& e);
        void HandleMouseButton(SDL_Event& e);
        void HideCursor();
        void ShowCursor();
        int  GetModState();//左ctrl、左altの状態を得る。
        void Resize();
};

Gui::Gui(Vga& vga, Kbc& kbc, Mouse& mouse){
    //IMPLイディオム
    this->impl = new Gui::Impl();

    //TODO:音出しはフロッピーディスクが動作している時のみにする。
    int result = 0;
    int flags = MIX_INIT_MP3;
    char *MP3_FILE_PATH = "Floppy_Disk_Drive02-1(Operating_Noise).mp3";
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }
    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }
    if(Mix_OpenAudio(22050, AUDIO_F32SYS, 2, 640)==-1){
        fprintf(stderr, "error : Mix_OpenAudio\n");
    }
    this->impl->music = Mix_LoadMUS(MP3_FILE_PATH);
    /***
    if(Mix_PlayMusic(this->music, -1)==-1){
        this->Error("Error : Mix_PlayMusic");
    }
    ***/
    this->impl->vga = &vga;
    this->impl->kbc = &kbc;
    this->impl->mouse = &mouse;

    this->impl->screen_height = DEFAULT_HEIGHT;
    this->impl->screen_width  = DEFAULT_WIDTH;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        cerr << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->impl->window = SDL_CreateWindow("EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->impl->screen_width*WIDTH_DISPLAY_SCALE, this->impl->screen_height*HEIGHT_DISPLAY_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(this->impl->window==NULL){
        cout << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->impl->renderer      = SDL_CreateRenderer(this->impl->window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    this->impl->texture       = SDL_CreateTexture(this->impl->renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, this->impl->screen_width, this->impl->screen_height); 
    if(SDL_RenderSetLogicalSize(this->impl->renderer, this->impl->screen_width, this->impl->screen_height*HEIGHT_DISPLAY_SCALE)<0){
        cout << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->impl->image = new Pixel[MAX_WIDTH*MAX_HEIGHT*sizeof(Pixel)];//最大領域の場合のサイズで確保しておく。
    this->impl->grab  = false;
}

Gui::~Gui(){
    delete[] this->impl->image;
    SDL_DestroyTexture(this->impl->texture);
    SDL_DestroyRenderer(this->impl->renderer);
    SDL_DestroyWindow(this->impl->window);
    SDL_Quit();
    delete this->impl;
}

bool Gui::IsQuit(){
    return this->impl->quit;
}

void Gui::Finish(){
    this->impl->quit = true;
}

void Gui::SoundFdc(){
    if(Mix_PlayMusic(this->impl->music, 1)==-1){
        this->Error("Error : Mix_PlayMusic");
    }
}

inline void Gui::Impl::Resize(){
    SDL_SetWindowSize(this->window, this->screen_width, this->screen_height);
    SDL_RenderSetLogicalSize(this->renderer,this->screen_width,this->screen_height);
    SDL_DestroyTexture(this->texture);
    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, this->screen_width, this->screen_height); 
}

inline void Gui::Impl::Update(){
    SDL_UpdateTexture(this->texture, NULL, this->image, this->screen_width * sizeof(Pixel));
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
}

inline void Gui::Impl::Update(const int x, const int y, const int w, const int h){
    SDL_Rect rect;
    rect.x = x;//左上の座標
    rect.y = y;//左上の座標
    rect.w = this->screen_width;//長方形の幅
    rect.h = this->screen_height;//長方形の高さ
    SDL_UpdateTexture(this->texture, &rect, this->image+x+y*this->screen_width, this->screen_width * sizeof(Pixel));
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
}

//SDLのキーボードキーコードのヘッダーファイル:https://github.com/davidsiaw/SDL2/blob/6ecaa6b61372e5b2f9bd01201814d07e34bb4186/include/SDL_keycode.h
//定数値が分かるURL : http://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html
inline uint8_t Gui::Impl::SdlScancode2KeyCode(SDL_Event& e){
    uint8_t key_code;
    switch (e.key.keysym.sym){
        case SDLK_0:
            key_code = KEY_CODE_0;
            break;
        case SDLK_1:
            key_code = KEY_CODE_1;
            break;
        case SDLK_DOWN:
        case SDLK_2:
            key_code = KEY_CODE_2;
            break;
        case SDLK_3:
            key_code = KEY_CODE_3;
            break;
        case SDLK_LEFT:
        case SDLK_4:
            key_code = KEY_CODE_4;
            break;
        case SDLK_5:
            key_code = KEY_CODE_5;
            break;
        case SDLK_RIGHT:
        case SDLK_6:
            key_code = KEY_CODE_6;
            break;
        case SDLK_7:
            key_code = KEY_CODE_7;
            break;
        case SDLK_UP:
        case SDLK_8:
            key_code = KEY_CODE_8;
            break;
        case SDLK_9:
            key_code = KEY_CODE_9;
            break;
        case SDLK_a:
            key_code = KEY_CODE_A;
            break;
        case SDLK_b:
            key_code = KEY_CODE_B;
            break;
        case SDLK_c:
            key_code = KEY_CODE_C;
            break;
        case SDLK_d:
            key_code = KEY_CODE_D;
            break;
        case SDLK_e:
            key_code = KEY_CODE_E;
            break;
        case SDLK_f:
            key_code = KEY_CODE_F;
            break;
        case SDLK_s:
            key_code = KEY_CODE_S;
            break;
        case SDLK_g:
            key_code = KEY_CODE_G;
            if(this->grab&&this->GetModState()&&(!e.key.repeat)){//ctrlを押しているならば、画面外に出す。
                this->ShowCursor();
            }
            break;
        case SDLK_q:
            key_code = KEY_CODE_Q;
            break;
        case SDLK_h:
            key_code = KEY_CODE_H;
            break;
        case SDLK_j:
            key_code = KEY_CODE_J;
            break;
        case SDLK_k:
            key_code = KEY_CODE_K;
            break;
        case SDLK_l:
            key_code = KEY_CODE_L;
            break;
        case SDLK_m:
            key_code = KEY_CODE_M;
            break;
        case SDLK_n:
            key_code = KEY_CODE_N;
            break;
        case SDLK_o:
            key_code = KEY_CODE_O;
            break;
        case SDLK_p:
            key_code = KEY_CODE_P;
            break;
        case SDLK_r:
            key_code = KEY_CODE_R;
            break;
        case SDLK_t:
            key_code = KEY_CODE_T;
            break;
        case SDLK_w:
            key_code = KEY_CODE_W;
            break;
        case SDLK_x:
            key_code = KEY_CODE_X;
            break;
        case SDLK_z:
            key_code = KEY_CODE_Z;
            break;
        case SDLK_y:
            key_code = KEY_CODE_Y;
            break;
        case SDLK_u:
            key_code = KEY_CODE_U;
            break;
        case SDLK_i:
            key_code = KEY_CODE_I;
            break;
        case SDLK_v:
            key_code = KEY_CODE_V;
            break;
        case SDLK_SEMICOLON:
            key_code = KEY_CODE_SEMICOLON;
            break;
        case SDLK_COLON:
            key_code = KEY_CODE_COLON;
            break;
        case SDLK_SPACE:
            key_code = KEY_CODE_SPACE;
            break;
        case SDLK_RETURN:
            key_code = KEY_CODE_ENTER;
            break;
        case SDLK_PERIOD:
            key_code = KEY_CODE_PERIOD;
            break;
        case SDLK_COMMA:
            key_code = KEY_CODE_COMMA;
            break;
        case SDLK_LEFTBRACKET:
            key_code = KEY_CODE_LEFTBRACKET;
            break;
        case SDLK_RIGHTBRACKET:
            key_code = KEY_CODE_RIGHTBRACKET;
            break;
        case SDLK_BACKSPACE:
            key_code = KEY_CODE_BACKSPACE;
            break;
        case SDLK_LSHIFT:
            key_code = KEY_CODE_LSHIFT;
            break;
        case SDLK_RSHIFT:
            key_code = KEY_CODE_RSHIFT;
            break;
        case SDLK_TAB:
            key_code = KEY_CODE_TAB;
            break;
        case SDLK_LCTRL:
            key_code = KEY_CODE_LCTRL;
            break;
        case SDLK_MINUS:
            key_code = KEY_CODE_MINUS;
            break;
        case SDLK_F1:
            key_code = KEY_CODE_F1;
            break;
        case SDLK_F2:
            key_code = KEY_CODE_F2;
            break;
        case SDLK_F3:
            key_code = KEY_CODE_F3;
            break;
        case SDLK_F4:
            key_code = KEY_CODE_F4;
            break;
        case SDLK_F5:
            key_code = KEY_CODE_F5;
            break;
        case SDLK_F6:
            key_code = KEY_CODE_F6;
            break;
        case SDLK_F7:
            key_code = KEY_CODE_F7;
            break;
        case SDLK_F8:
            key_code = KEY_CODE_F8;
            break;
        case SDLK_F9:
            key_code = KEY_CODE_F9;
            break;
        case SDLK_F10:
            key_code = KEY_CODE_F10;
            break;
        case SDLK_F11:
            key_code = KEY_CODE_F11;
            break;
        case SDLK_F12:
            key_code = KEY_CODE_F12;
            break;
        case SDLK_LALT:
            key_code = KEY_CODE_LALT;
            break;
        case SDLK_CAPSLOCK:
            key_code = KEY_CODE_CAPSLOCK;
            break;
        case SDLK_AT:
            key_code = KEY_CODE_AT;
            break;
        case SDLK_SLASH:
            key_code = KEY_CODE_SLASH;
            break;
        case SDLK_UNDERSCORE:
            key_code = KEY_CODE_UNDERSCORE;
            break;
        case 0xA5://TODO:SDLK_BACKSLASHは日本語では0xA5, このマジックナンバーを後で修正
            key_code = KEY_CODE_BACKSLASH;
            break;
        default:
            fprintf(stderr, "Not implemented: SDL_Keycode = %08X(http://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html)", e.key.keysym.sym);
            throw "at Gui::HandleKeyDown\n";
    }
    return key_code;
}

inline void Gui::Impl::HandleKeyDown(SDL_Event& e){
    uint8_t key_code;
    switch (e.key.keysym.sym){//使うことのないキーコードはここでスルーする
        case SDLK_LGUI: 
            return;
        case SDLK_RGUI:
            return;
    }
    key_code = this->SdlScancode2KeyCode(e);
    this->kbc->Push(key_code);
}

inline void Gui::Impl::HandleKeyUp(SDL_Event& e){
    uint8_t key_code;
    switch (e.key.keysym.sym){//使うことのないキーコードはここでスルーする
        case SDLK_LGUI: 
            return;
        case SDLK_RGUI:
            return;
    }
    key_code = KEY_CODE_BREAK | this->SdlScancode2KeyCode(e);
    this->kbc->Push(key_code);
}

inline void Gui::Impl::HideCursor(){
    this->grab = true;
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);//http://sdl2referencejp.osdn.jp/SDL_SetRelativeMouseMode.html
}

inline void Gui::Impl::ShowCursor(){
    this->grab = false;
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

inline int Gui::Impl::GetModState(){//左ctrl、左altの状態を得る。
    int mod_state = SDL_GetModState();
    int ctrl_alt_state = KMOD_LALT|KMOD_LCTRL;
    return (mod_state&ctrl_alt_state)==ctrl_alt_state;
}

inline void Gui::Impl::HandleMouseMotion(SDL_Event& e){

    uint8_t data0, data1, data2;
    int rel_x, rel_y;
   
    rel_x = e.motion.xrel / MOVE_SCALE;
    rel_y = e.motion.yrel / MOVE_SCALE;
    if(rel_x>127){
        rel_x = 127;
    }else if(rel_x < -127){
        rel_x = -127;
    }
    if(rel_y>127){
        rel_y = 127;
    }else if(rel_y < -127){
        rel_y = -127;
    }
    rel_y *= -1;
    data0 = DEFAULT_PACKET_BYTE0;
    if(e.motion.state==SDL_BUTTON_LMASK){
        data0 = data0 | LEFT_BUTTON;
    }
    if(rel_x < 0){
        data0 = data0 | X_SIGN_BIT;
    }
    if(rel_y < 0){
        data0 = data0 | Y_SIGN_BIT;
    }
    this->mouse->Push(data0);
    this->mouse->Push((uint8_t)rel_x);
    this->mouse->Push((uint8_t)rel_y);
}

inline void Gui::Impl::HandleMouseButton(SDL_Event& e){
    this->mouse->Push(DEFAULT_PACKET_BYTE0|LEFT_BUTTON);
    this->mouse->Push(0);
    this->mouse->Push(0);
}

//この関数はVgaクラスのvga_mutexをロックします。
void Gui::Display(){
    SDL_Event e;
    unsigned int start;
    unsigned int end;
    //SDL_WarpMouseInWindow(this->window, guest_x, guest_y);
    uint8_t* prev_snap = (uint8_t*)malloc(MAX_HEIGHT*MAX_WIDTH);
    uint8_t* new_snap  = (uint8_t*)malloc(MAX_HEIGHT*MAX_WIDTH);
    memset(prev_snap, 0x00, MAX_HEIGHT*MAX_WIDTH);
    memset(new_snap, 0x00, MAX_HEIGHT*MAX_WIDTH);
    bool full_update;
    while (!this->impl->quit){
        try{
            start = SDL_GetTicks();
            while (SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT){
                    this->impl->quit = true;
                }
                if(e.type==SDL_KEYDOWN){
                    this->impl->HandleKeyDown(e);
                    break;
                }
                if(e.type==SDL_KEYUP){
                    this->impl->HandleKeyUp(e);
                    break;
                }
                if(e.type==SDL_MOUSEMOTION){
                    if(!this->impl->mouse->IsEnable()){
                        break;
                    }
                    this->impl->HandleMouseMotion(e);
                    break;
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(!this->impl->mouse->IsEnable()){
                        break;
                    }
                    if(!this->impl->grab){//画面にマウスが取り込まれていない時。
                        this->impl->HideCursor();
                    }
                    this->impl->HandleMouseButton(e);
                }
            }
            this->impl->vga->LockVga();
            full_update = false;
            if((this->impl->vga->GetHeight()!=this->impl->screen_height)||(this->impl->vga->GetWidth()!=this->impl->screen_width)){
                this->impl->screen_height = this->impl->vga->GetHeight();
                this->impl->screen_width  = this->impl->vga->GetWidth();
                this->impl->Resize();
                full_update = true;
            }
            if(this->impl->vga->GetMode()==TEXT_MODE){
                full_update = true;//変更部分のみの描画処理はグラフィックモードでしかサポートしていない。
            }
            if(!full_update){
                this->impl->vga->SetSnap(new_snap, this->impl->screen_height, this->impl->screen_width);
                int y_start = -1;
                int y;//forループ抜け出した後も利用する。
                for(y=0; y<this->impl->screen_height; y++){
                    //一致しないとき、再描画
                    if(memcmp(prev_snap+y*this->impl->screen_width, new_snap+y*this->impl->screen_width, this->impl->screen_width)){//1行比較
                        for(int x=0; x<this->impl->screen_width; x++){
                            this->impl->image[x+y*this->impl->screen_width] = *(this->impl->vga->GetPixel(x, y));//一致していないので、1行転送, memcpyしない理由はGetPixelを参照してくだされば分かります。
                        }
                        if(y_start<0){
                            y_start = y;
                        }
                    }else{//メモリの内容は一致。それまでの内容を書き出す
                        if(y_start>=0){
                            this->impl->Update(0, y_start, this->impl->screen_width, y-y_start);
                            y_start = -1;
                        }   
                    }
                }
                if(y_start>=0){//最後の行まで一致しなかった時の条件式
                    this->impl->Update(0, y_start, this->impl->screen_width, y-y_start);
                }
                memcpy(prev_snap, new_snap, this->impl->screen_height*this->impl->screen_width);
            }else{
                for(int y=0; y<this->impl->screen_height; y++){
                    for(int x=0; x<this->impl->screen_width; x++){
                        this->impl->image[x+y*this->impl->screen_width] = *(this->impl->vga->GetPixel(x, y));
                    }
                }
                this->impl->Update();
            }
            this->impl->vga->UnlockVga();
            end = SDL_GetTicks();
            end = end - start;
            if(16>end){
                SDL_Delay(16-end);
            }   
        }catch(const char* error_message){
            this->impl->quit = true;
        }
    }
}