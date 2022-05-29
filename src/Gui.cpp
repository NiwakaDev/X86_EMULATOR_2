#include "Gui.h"
#include <SDL2/SDL.h>
#include "KbcEnum.h"

using namespace std;

// SCALEの設定に深い意味はないです。
//それっぽい数値にしただけです。
const int MOVE_SCALE = 5;
const int WIDTH_DISPLAY_SCALE = 1;
const int HEIGHT_DISPLAY_SCALE = 2;

const int MAX_WIDTH = 1280;
const int MAX_HEIGHT = 1024;

class Gui::Pimpl {
 public:
  unique_ptr<Object> obj;
  unique_ptr<Pixel[]> image;
  bool grab;
  void Update();
  void Update(const int x, const int y, const int w, const int h);
  bool quit = false;
  int screen_width;
  int screen_height;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_Texture* texture = NULL;
  // Mix_Music *music;
  void HandleKeyDown(SDL_Event& e);
  void HandleKeyUp(SDL_Event& e);
  uint8_t SdlScancode2KeyCode(SDL_Event& e);
  void HandleMouseMotion(SDL_Event& e);
  void HandleMouseButton(SDL_Event& e);
  void HideCursor();
  void ShowCursor();
  void Resize();
  function<void(uint8_t)> mouse_push;
  function<void(uint8_t)> keyboard_push;
};

// publicメンバしか利用しないor何のメンバも使用しない関数はGuiHelper空間に実装する
namespace GuiHelper {
static inline int GetModState();  //左ctrl、左altの状態を得る。
};

static inline int GuiHelper::GetModState() {
  int mod_state = SDL_GetModState();
  int ctrl_alt_state = KMOD_LALT | KMOD_LCTRL;
  return (mod_state & ctrl_alt_state) == ctrl_alt_state;
}

Gui::Gui(int default_height, int default_width)
    : pimpl(make_unique<Gui::Pimpl>()) {
  this->pimpl->screen_height = default_height;
  this->pimpl->screen_width = default_width;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << SDL_GetError() << endl;
    this->pimpl->obj->Error("at Gui::Gui");
  }
  this->pimpl->window = SDL_CreateWindow(
      "EMULATOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      this->pimpl->screen_width * WIDTH_DISPLAY_SCALE,
      this->pimpl->screen_height * HEIGHT_DISPLAY_SCALE,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (this->pimpl->window == NULL) {
    cout << SDL_GetError() << endl;
    this->pimpl->obj->Error("at Gui::Gui");
  }
  this->pimpl->renderer =
      SDL_CreateRenderer(this->pimpl->window, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  this->pimpl->texture =
      SDL_CreateTexture(this->pimpl->renderer, SDL_PIXELFORMAT_BGRA8888,
                        SDL_TEXTUREACCESS_STREAMING, this->pimpl->screen_width,
                        this->pimpl->screen_height);
  if (SDL_RenderSetLogicalSize(
          this->pimpl->renderer, this->pimpl->screen_width,
          this->pimpl->screen_height * HEIGHT_DISPLAY_SCALE) < 0) {
    cout << SDL_GetError() << endl;
    this->pimpl->obj->Error("at Gui::Gui");
  }
  this->pimpl->image = make_unique<Pixel[]>(
      MAX_WIDTH * MAX_HEIGHT);  //最大領域の場合のサイズで確保しておく。
  this->pimpl->grab = false;
  this->pimpl->obj = make_unique<Object>();
}

Gui::~Gui() {
  SDL_DestroyTexture(this->pimpl->texture);
  SDL_DestroyRenderer(this->pimpl->renderer);
  SDL_DestroyWindow(this->pimpl->window);
  SDL_Quit();
}

void Gui::AddIoDevice(IO_DEVICE_KIND io_device_kind,
                      function<void(uint8_t)> push) {
  if (io_device_kind == IO_DEVICE_KIND_TOTAL) return;
  if (io_device_kind == Gui::MOUSE) {
    this->pimpl->mouse_push = push;
  } else {
    this->pimpl->keyboard_push = push;
  }
}

bool Gui::IsQuit() { return this->pimpl->quit; }

void Gui::Finish() { this->pimpl->quit = true; }

inline void Gui::Pimpl::Resize() {
  SDL_SetWindowSize(this->window, this->screen_width, this->screen_height);
  SDL_RenderSetLogicalSize(this->renderer, this->screen_width,
                           this->screen_height);
  SDL_DestroyTexture(this->texture);
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_BGRA8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    this->screen_width, this->screen_height);
}

inline void Gui::Pimpl::Update() {
  SDL_UpdateTexture(this->texture, NULL, this->image.get(),
                    this->screen_width * sizeof(Pixel));
  SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
  SDL_RenderPresent(this->renderer);
}

inline void Gui::Pimpl::Update(const int x, const int y, const int w,
                               const int h) {
  SDL_Rect rect;
  rect.x = x;                    //左上の座標
  rect.y = y;                    //左上の座標
  rect.w = this->screen_width;   //長方形の幅
  rect.h = this->screen_height;  //長方形の高さ
  SDL_UpdateTexture(this->texture, &rect,
                    this->image.get() + x + y * this->screen_width,
                    this->screen_width * sizeof(Pixel));
  SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
  SDL_RenderPresent(this->renderer);
}

// SDLのキーボードキーコードのヘッダーファイル:https://github.com/davidsiaw/SDL2/blob/6ecaa6b61372e5b2f9bd01201814d07e34bb4186/include/SDL_keycode.h
//定数値が分かるURL : http://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html
inline uint8_t Gui::Pimpl::SdlScancode2KeyCode(SDL_Event& e) {
  uint8_t key_code;
  switch (e.key.keysym.sym) {
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
      if (this->grab && GuiHelper::GetModState() &&
          (!e.key.repeat)) {  // ctrlを押しているならば、画面外に出す。
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
    case 0xA5:  // TODO:SDLK_BACKSLASHは日本語では0xA5,
                // このマジックナンバーを後で修正
      key_code = KEY_CODE_BACKSLASH;
      break;
    default:
      fprintf(stderr,
              "Not implemented: SDL_Keycode = "
              "%08X(http://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html)",
              e.key.keysym.sym);
      throw "at Gui::HandleKeyDown\n";
  }
  return key_code;
}

inline void Gui::Pimpl::HandleKeyDown(SDL_Event& e) {
  switch (e.key.keysym.sym) {  //使うことのないキーコードはここでスルーする
    case SDLK_LGUI:
      return;
    case SDLK_RGUI:
      return;
  }
  uint8_t key_code = this->SdlScancode2KeyCode(e);
  this->keyboard_push(key_code);
}

inline void Gui::Pimpl::HandleKeyUp(SDL_Event& e) {
  switch (e.key.keysym.sym) {  //使うことのないキーコードはここでスルーする
    case SDLK_LGUI:
      return;
    case SDLK_RGUI:
      return;
  }
  uint8_t key_code = KEY_CODE_BREAK | this->SdlScancode2KeyCode(e);
  this->keyboard_push(key_code);
}

inline void Gui::Pimpl::HideCursor() {
  this->grab = true;
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetRelativeMouseMode(
      SDL_TRUE);  // http://sdl2referencejp.osdn.jp/SDL_SetRelativeMouseMode.html
}

inline void Gui::Pimpl::ShowCursor() {
  this->grab = false;
  SDL_ShowCursor(SDL_ENABLE);
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

inline void Gui::Pimpl::HandleMouseMotion(SDL_Event& e) {
  int rel_x = e.motion.xrel / MOVE_SCALE;
  int rel_y = e.motion.yrel / MOVE_SCALE;
  if (rel_x > 127) {
    rel_x = 127;
  } else if (rel_x < -127) {
    rel_x = -127;
  }
  if (rel_y > 127) {
    rel_y = 127;
  } else if (rel_y < -127) {
    rel_y = -127;
  }
  rel_y *= -1;

  uint8_t data0 = DEFAULT_PACKET_BYTE0;
  if (e.motion.state == SDL_BUTTON_LMASK) {
    data0 = data0 | LEFT_BUTTON;
  }
  if (rel_x < 0) {
    data0 = data0 | X_SIGN_BIT;
  }
  if (rel_y < 0) {
    data0 = data0 | Y_SIGN_BIT;
  }
  this->mouse_push(data0);
  this->mouse_push((uint8_t)rel_x);
  this->mouse_push((uint8_t)rel_y);
}

inline void Gui::Pimpl::HandleMouseButton(SDL_Event& e) {
  this->mouse_push(DEFAULT_PACKET_BYTE0 | LEFT_BUTTON);
  this->mouse_push(0);
  this->mouse_push(0);
}

//この関数はVgaクラスのvga_mutexをロックします。
void Gui::Display(
    std::function<void(Pixel* image, int* display_width, int* display_height,
                       std::function<void()> resize_callback)>
        set_image_callback) {
  while (!this->pimpl->quit) {
    try {
      unsigned int start = SDL_GetTicks();
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          this->pimpl->quit = true;
        }
        if (e.type == SDL_KEYDOWN) {
          this->pimpl->HandleKeyDown(e);
          break;
        }
        if (e.type == SDL_KEYUP) {
          this->pimpl->HandleKeyUp(e);
          break;
        }
        if (e.type == SDL_MOUSEMOTION) {
          this->pimpl->HandleMouseMotion(e);
          break;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
          if (!this->pimpl->grab) {  //画面にマウスが取り込まれていない時。
            this->pimpl->HideCursor();
          }
          this->pimpl->HandleMouseButton(e);
        }
      }
      set_image_callback(this->pimpl->image.get(), &this->pimpl->screen_width,
                         &this->pimpl->screen_height,
                         [&]() { this->pimpl->Resize(); });
      this->pimpl->Update();
      unsigned int end = SDL_GetTicks();
      end = end - start;
      if (16 > end) {
        SDL_Delay(16 - end);
      }
    } catch (const char* error_message) {
      this->pimpl->quit = true;
    }
  }
}