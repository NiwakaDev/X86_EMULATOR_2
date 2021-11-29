#include "Vga.h"
#include "Kbc.h"
#include "Gui.h"
#include "Font.h"
#include "Mouse.h"

#define MOVE_SCALE 5
#define DISPLAY_SCALE 2

#define MAX_WIDTH 1280
#define MAX_HEIGHT 1024

Gui::Gui(Vga* vga, Kbc* kbc, Mouse* mouse){
    this->vga = vga;
    assert(this->vga!=NULL);
    this->kbc = kbc;
    assert(this->kbc!=NULL);
    this->mouse = mouse;
    assert(this->mouse!=NULL);
    this->SCREEN_HEIGHT = DEFAULT_HEIGHT;
    this->SCREEN_WIDTH  = DEFAULT_WIDTH;
    this->InitFontAscii();
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        cerr << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->window = SDL_CreateWindow("EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->SCREEN_WIDTH*DISPLAY_SCALE, this->SCREEN_HEIGHT*DISPLAY_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(this->window==NULL){
        cout << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->renderer      = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    this->texture       = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, this->SCREEN_WIDTH, this->SCREEN_HEIGHT); 
    if(SDL_RenderSetLogicalSize(this->renderer, this->SCREEN_WIDTH, this->SCREEN_HEIGHT)<0){
        cout << SDL_GetError() << endl;
        this->Error("at Gui::Gui");
    }
    this->image = (Pixel*)malloc(MAX_WIDTH*MAX_HEIGHT*sizeof(Pixel));//最大領域の場合のサイズで確保しておく。
}

Gui::~Gui(){
    //SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Gui::InitFontAscii(){
    int i;

    for(i=0; i<16; i++){
        this->font_ascii['\n'][i] = 0x00;
    }
    for(i=0; i<16; i++){
        this->font_ascii['('][i] = font_RIGHT_PAR[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii[' '][i] = font_SPACE[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['+'][i] = font_PLUS[i];
    }
    for(i=0; i<16; i++){
        this->font_ascii[','][i] = font_COMMA[i];
    }
    for(i=0; i < 16; i++){
        font_ascii['='][i] = font_EQUAL[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['-'][i] = font_HYPHEN[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['.'][i] = font_DOT[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['0'][i] = font_0[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['1'][i] = font_1[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['2'][i] = font_2[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['3'][i] = font_3[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['4'][i] = font_4[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['5'][i] = font_5[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['6'][i] = font_6[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['7'][i] = font_7[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['8'][i] = font_8[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['9'][i] = font_9[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii[':'][i] = font_COLON[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['A'][i] = font_A[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['B'][i] = font_B[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['C'][i] = font_C[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['D'][i] = font_D[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['E'][i] = font_E[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['F'][i] = font_F[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['G'][i] = font_G[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['H'][i] = font_H[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['I'][i] = font_I[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['J'][i] = font_J[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['K'][i] = font_K[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['L'][i] = font_L[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['M'][i] = font_M[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['N'][i] = font_N[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['O'][i] = font_O[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['P'][i] = font_P[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['Q'][i] = font_Q[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['R'][i] = font_R[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['S'][i] = font_S[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['T'][i] = font_T[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['U'][i] = font_U[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['V'][i] = font_V[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['W'][i] = font_W[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['X'][i] = font_X[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['Y'][i] = font_Y[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['Z'][i] = font_Y[i];
    }
    for(i=0; i < 16; i++){
        this->font_ascii['a'][i] = font_Y[i];
    }
}

void Gui::Resize(){
    SDL_SetWindowSize(this->window, this->SCREEN_WIDTH, this->SCREEN_HEIGHT);
    SDL_RenderSetLogicalSize(this->renderer,this->SCREEN_WIDTH,this->SCREEN_HEIGHT);
    SDL_DestroyTexture(this->texture);
    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, this->SCREEN_WIDTH, this->SCREEN_HEIGHT); 
}

void Gui::Update(){
    SDL_UpdateTexture(this->texture, NULL, this->image, this->SCREEN_WIDTH * sizeof(Pixel));
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
}

//この関数はVgaクラスのvga_mutexをロックします。
void Gui::Display(){
    SDL_Event e;
    bool quit = false;
    unsigned int start;
    unsigned int end;

    //SDL_WarpMouseInWindow(this->window, guest_x, guest_y);
    while (!quit){
        start = SDL_GetTicks();
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type==SDL_KEYDOWN){
                this->HandleKeyDown(&e);
                break;
            }
            if(e.type==SDL_KEYUP){
                this->HandleKeyUp(&e);
                break;
            }
            if(e.type==SDL_MOUSEMOTION){
                if(!this->mouse->IsEnable()){
                    break;
                }
                this->HandleMouseMotion(&e);
                break;
            }
            if(e.type==SDL_MOUSEBUTTONDOWN){
                if(!this->mouse->IsEnable()){
                    break;
                }
                this->HideCursor();
                this->HandleMouseButton(&e);
            }
        }
        this->vga->vga_mtx.lock();
        if((this->vga->GetHeight()!=this->SCREEN_HEIGHT)||(this->vga->GetWidth()!=this->SCREEN_WIDTH)){
            this->SCREEN_HEIGHT = this->vga->GetHeight();
            this->SCREEN_WIDTH  = this->vga->GetWidth();
            this->Resize();
        }
        for(int y=0; y<this->SCREEN_HEIGHT; y++){
            for(int x=0; x<this->SCREEN_WIDTH; x++){
                this->image[x+y*this->SCREEN_WIDTH] = *(this->vga->GetPixel(x, y));
            }
        }
        this->vga->vga_mtx.unlock();
        end = SDL_GetTicks();
        end = end - start;
        if(16>end){
            SDL_Delay(16-end);
        }   
        this->Update();
    }
}

uint8_t Gui::SdlScancode2KeyCode(SDL_Event *e){
    uint8_t key_code;
    switch (e->key.keysym.sym){
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
        default:
            this->Error("Not implemented: SDL_Keycode = %08X(http://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html) at Gui::HandleKeyDown", e->key.keysym.sym);
    }
    return key_code;
}

void Gui::HandleKeyDown(SDL_Event *e){
    uint8_t key_code;
    key_code = this->SdlScancode2KeyCode(e);
    this->kbc->Send(key_code);
}

void Gui::HandleKeyUp(SDL_Event *e){
    uint8_t key_code;
    key_code = KEY_CODE_BREAK | this->SdlScancode2KeyCode(e);
    this->kbc->Send(key_code);
}

void Gui::HideCursor(){
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);//http://sdl2referencejp.osdn.jp/SDL_SetRelativeMouseMode.html
}

void Gui::HandleMouseMotion(SDL_Event *e){

    uint8_t data0, data1, data2;
    int rel_x, rel_y;
   
    rel_x = e->motion.xrel / MOVE_SCALE;
    rel_y = e->motion.yrel / MOVE_SCALE;
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
    if(e->motion.state==SDL_BUTTON_LMASK){
        data0 = data0 | LEFT_BUTTON;
    }
    if(rel_x < 0){
        data0 = data0 | X_SIGN_BIT;
    }
    if(rel_y < 0){
        data0 = data0 | Y_SIGN_BIT;
    }
    this->mouse->Send(data0);
    this->mouse->Send((uint8_t)rel_x);
    this->mouse->Send((uint8_t)rel_y);
}

void Gui::HandleMouseButton(SDL_Event *e){
    this->mouse->Send(DEFAULT_PACKET_BYTE0|LEFT_BUTTON);
    this->mouse->Send(0);
    this->mouse->Send(0);
}