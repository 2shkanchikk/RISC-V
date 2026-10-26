#include "hostfs.h"
#include "bus.h"
#include <vector>

HostFS::HostFS(Bus* bus){
    this->bus = bus;
}

HostFS::~HostFS(){
    for(int i = 0; i < kMaxFiles; ++i)
        if(files[i]){
            fclose(files[i]);
            files[i] = nullptr;
        }
}

void HostFS::write32(uint32_t addr, uint32_t value){
     switch (addr) {
        case 0x04: arg0 = value; break;
        case 0x08: arg1 = value; break;
        case 0x0C: arg2 = value; break;
        case 0x00: execute(value); break;
    }
}

int HostFS::find_free_slot(){
    for (int i = 0; i < kMaxFiles; ++i)
        if (!files[i]) return i;
    return -1;
}

void HostFS::execute(uint32_t opcode){
    switch(opcode){
        case 0x00:{ //open
            std::string s;
            uint8_t b;
            uint32_t cur = arg0;
            while((b = bus->read8(cur)) != 0){
                s.push_back(static_cast<char>(b));
                ++cur;
                if (s.size() > 255){break;}
            }

            if ((s.empty() || s.find("..") != std::string::npos) || s[0] == '/' || s.size() > 255){result = 0xFFFFFFFF; break;}

            s = path + s;
            const char* mode{nullptr};

            switch(arg1){
                case 0x00:{
                    mode = "rb";
                    break;
                }

                case 0x01:{
                    mode = "wb";
                    break;
                }

                case 0x02:{
                    mode = "ab";
                    break;
                }

                case 0x03:{
                    mode = "r+b";
                    break;
                }

                default: break;
            }

            if (!mode){result = 0xFFFFFFFF; break;}

            int n = find_free_slot();

            if (n == -1){result = 0xFFFFFFFF; break;}

            FILE* file = fopen(s.c_str(), mode);

            if(!file){result = 0xFFFFFFFF; break;}

            files[n] = file;
            result = n;
            break;
        }

        case 0x01:{ //close
            if(arg0 >= kMaxFiles || !files[arg0]){result = 0xFFFFFFFF; break;}
            fclose(files[arg0]);
            files[arg0] = nullptr;
            result = 0;
            break;
        }

        case 0x10:{ //read
            if(arg0 >= kMaxFiles || !files[arg0] || !bus->isRAM(arg1, arg2)){result = 0xFFFFFFFF; break;}
            std::vector<uint8_t> buf(arg2);
            size_t count = fread(buf.data(), 1, arg2, files[arg0]);

            for(size_t i = 0; i < count; ++i){
                bus->write8(arg1 + i, buf[i]);
            }

            result = count;
            break;
        }

        case 0x11:{ //seek
            if(arg0 >= kMaxFiles || !files[arg0] || arg2 > 2){result = 0xFFFFFFFF; break;}
            if(fseek(files[arg0], int32_t(arg1), arg2) != 0){result = 0xFFFFFFFF; break;}
            result = ftell(files[arg0]);
            break;
        }

        case 0x20:{ //write
            if(arg0 >= kMaxFiles || !files[arg0] || !bus->isRAM(arg1, arg2)){result = 0xFFFFFFFF; break;}
            std::vector<uint8_t> buf;
            buf.reserve(arg2);
            
            for(size_t i = 0; i < arg2; ++i){
                buf.push_back(bus->read8(arg1 + i));
            }

            size_t count = fwrite(buf.data(), 1, buf.size(), files[arg0]);
            result = count;
            break;
        }

        default:{result = 0xFFFFFFFF; break;}
    }
}

uint32_t HostFS::read32(uint32_t addr){
    if(addr == 0x10){
        return result;
    }
    return 0;
}