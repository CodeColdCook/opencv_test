#include <arpa/inet.h>

#include <functional>  // fucntion/bind
#include <iostream>

class ProgramA {
 public:
  void FunA1() { printf("I'am ProgramA.FunA1() and be called..\n"); }

  void FunA2() { printf("I'am ProgramA.FunA2() and be called..\n"); }

  static void FunA3() { printf("I'am ProgramA.FunA3() and be called..\n"); }
};

class ProgramB {
  typedef std::function<void()> CallbackFun;

 public:
  void FunB1(CallbackFun callback) {
    printf("I'am ProgramB.FunB2() and be called..\n");
    callback();
  }
};

void normFun() { printf("I'am normFun() and be called..\n"); }

// int main(int argc, char **argv) {
//   ProgramA PA;
//   PA.FunA1();

//   printf("\n");
//   ProgramB PB;
//   PB.FunB1(normFun);
//   printf("\n");
//   PB.FunB1(ProgramA::FunA3);
//   printf("\n");
//   PB.FunB1(std::bind(&ProgramA::FunA2, &PA));
// }

inline uint64_t ByteSwap_Uint64(uint64_t u64_host) {
  unsigned int u32_host_h, u32_host_l;
  u32_host_l = u64_host & 0xffffffff;
  u32_host_h = (u64_host >> 32) & 0xffffffff;
  uint64_t u64_net = htonl(u32_host_l);
  u64_net = (u64_net << 32) | htonl(u32_host_h);
  std::cout << "raw data: " << static_cast<long long>(u64_host) << std::endl;
  printf("big endian : %016llx\n", u64_host);
  printf("small endian : %016llx\n", u64_net);
  return u64_net;
}

int main(int argc, char **argv) {
  double time = 1717948397035;
  uint64_t time_64 = static_cast<uint64_t>(time);
  unsigned long long u64_big_endian, u64_small_endian;
  u64_big_endian = time_64;
  u64_small_endian = ByteSwap_Uint64(u64_big_endian);
}