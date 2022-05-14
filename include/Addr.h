//MEMO : まだ使っていない。キャストのオーバーロードを実装したら導入予定
class Addr{
    private:
        uint32_t addr_;
    public:
        Addr(uint32_t addr): addr_(addr){

        }
        operator uint32_t(){
            return addr_;
        }
        Addr operator uint16_t(){
            return (Addr)(uint16_t)this->addr_;
        }
        operator uint8_t(){
            return (uint8_t)this->addr_;
        }
        operator int8_t(){
            return (int8_t)this->addr_;
        }
        operator int16_t(){
            return (int16_t)this->addr_;
        }
};