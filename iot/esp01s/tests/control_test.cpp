#include "../../../Protocol/bms_control.h"
constexpr bool command_ok(const char* line) { BmsControlCommand command={}; return bms_parse_command(line,&command); }
constexpr bool state_ok(const char* line) { BmsControlState state={}; return bms_parse_state(line,&state); }
static_assert(bms_crc16("123456789",9)==0x29b1,"CCITT test vector");
static_assert(command_ok("BMS,SET,7,0,2500,180,35*1080"),"command case 0");
static_assert(command_ok("BMS,SET,4294967295,4294967295,16500,3200,99*6710"),"command case 1");
static_assert(!command_ok("BMS,SET,0,0,2500,180,35*6817"),"command case 2");
static_assert(!command_ok("BMS,SET,4294967296,0,2500,180,35*3033"),"command case 3");
static_assert(!command_ok("BMS,SET,7,0,-2500,180,35*1165"),"command case 4");
static_assert(!command_ok("BMS,SET,7,0, 2500,180,35*046C"),"command case 5");
static_assert(!command_ok("BMS,SET,7,0,2500,180*30E0"),"command case 6");
static_assert(!command_ok("BMS,SET,7,0,2500,180,35,1*F702"),"command case 7");
static_assert(!command_ok("BMS,SET,7,0,2500,,35*BD8D"),"command case 8");
static_assert(!command_ok("BMS,GET,7,0,2500,180,35*68C2"),"command case 9");
static_assert(state_ok("BMS,CFG,2500,180,35,0,0,0*8E42"),"state case 0");
static_assert(state_ok("BMS,CFG,16500,3200,99,4294967295,4294967295,1*B45A"),"state case 1");
static_assert(state_ok("BMS,CFG,500,10,0,1,7,6*B57C"),"state case 2");
static_assert(!state_ok("BMS,CFG,499,180,35,0,0,0*5DE0"),"state case 3");
static_assert(!state_ok("BMS,CFG,2500,180,35,0,0,1*9E63"),"state case 4");
static_assert(!state_ok("BMS,CFG,2500,180,35,0,7,0*0BD2"),"state case 5");
static_assert(!state_ok("BMS,CFG,2500,180,35,0,7,7*7B35"),"state case 6");
static_assert(!command_ok("BMS,SET,7,0,2500,180,35*1081"),"damaged CRC");
static_assert(!command_ok("BMS,SET,7,0,2500,180,35"),"missing CRC");
static_assert(bms_limits_valid(500,10,0),"minimum limits");
static_assert(bms_limits_valid(16500,3200,99),"maximum limits");
static_assert(!bms_limits_valid(2501,180,35),"voltage precision");
static_assert(!bms_limits_valid(2500,9,35),"current low");
static_assert(!bms_limits_valid(2500,3201,35),"current high");
static_assert(!bms_limits_valid(2500,180,100),"temperature high");
