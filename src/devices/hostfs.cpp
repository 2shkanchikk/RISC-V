#include "hostfs.h"
#include "../core/bus.h"
#include <vector>
#include <filesystem>

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

bool HostFS::readPath(uint32_t addr, std::string& out){
    std::string s;
    uint8_t b;
    while((b = bus->read8(addr)) != 0){
        s.push_back(static_cast<char>(b));
        ++addr;
        if (s.size() > 255){return false;}
    }

    if ((s.empty() || s[0] == '/' || s.find("..") != std::string::npos)){return false;}

    out = path + s;
    return true;
}

void HostFS::execute(uint32_t opcode){
    switch(opcode){
        case 0x00:{ //open
            std::string s;
            if (!readPath(arg0, s)) { result = 0xFFFFFFFF; break; }

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

        case 0x30:{ //mkdir
            std::string p;
            if (!readPath(arg0, p)) {result = 0xFFFFFFFF; break;}

            std::error_code ec;
            std::filesystem::create_directory(p, ec);
            result = ec ? 0xFFFFFFFF : 0;
            break;
        }

        case 0x31:{ //unlink
            std::string p;
            if (!readPath(arg0, p)) {result = 0xFFFFFFFF; break;}

            if (std::remove(p.c_str()) != 0) {result = 0xFFFFFFFF; break;}
            result = 0;
            break;
        }

        case 0x32:{ //rename
            std::string p1;
            if (!readPath(arg0, p1)) {result = 0xFFFFFFFF; break;}
            std::string p2;
            if (!readPath(arg1, p2)) {result = 0xFFFFFFFF; break;}

            if(std::rename(p1.c_str(), p2.c_str()) != 0) {result = 0xFFFFFFFF; break;}
            result = 0;
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