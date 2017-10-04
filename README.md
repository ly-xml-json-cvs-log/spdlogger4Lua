## spdlogger4Lua是lua的日志库

spdlog是c++的一个log库，具有速度快，功能全面，使用方便的特点，具体可参考：https://github.com/gabime/spdlog。

spdlogger4Lua是spdlog的lua移植，它通过简单的封装spdlog的方法和类，使得在lua程序中可以方便的使用spdlog。

spdlogger4Lua使用的是spdlog的单线程同步日志模式。


### 函数说明

在spdlogger4Lua中，输出了11个函数到lua中，具体说明如下：
```lua
-- 创建并返回一个basic_logger对象。这种logger对象在文件中记录日志，name指定了logger的名称，file_name说明了存储的文件名。
function basic_loggger(name, file_name)

-- 创建并返回一个rotating_logger对象。这种logger对象在多个文件中，循环记录日志。name指定了logger的名称，file_name说明了存储的文件名，max_file_size指定每个logger文件的最大容量是多少个字节，max_file指定了最多在多少个文件中循环记录日志。
function rotating_logger(name, file_name, max_file_size, max_file)

-- 创建并返回一个daily_logger对象。这种logger对象每天都会创建一个新的文件，用于记录当天的日志。name指定了logger的名称，file_name说明了存储的文件名，hour和minute指定了在每天的什么时间创建当天的日志文件。
function daily_logger(name, file_name, hour, minute)

-- 创建并返回一个stdout_logger对象。这种logger对象在标准输出设备上记录日志，就是将日志在屏幕上输出。name指定了logger的名称。
function stdout_logger(name)

-- 创建并返回一个stderr_logger对象。这种logger对象在标准错误设备上记录日志，一般的标准错误设备就是屏幕，实际也是在屏幕上输出日志。name指定了logger的名称。
function stderr_logger(name)

-- 创建并返回一个stdout_color对象。这种logger对象在具备彩色能力的标准输出设备上记录日志，就是将日志在屏幕上以不同的颜色输出。name指定了logger的名称。
function stdout_color(name)

-- 创建并返回一个stderr_color对象。这种logger对象在具备彩色能力的标准错误设备上记录日志，一般的标准错误设备就是屏幕，实际也就是将日志在屏幕上以不同颜色输出。name指定了logger的名称。
function stderr_color(name)

-- 查找并返回名叫name的logger，如果没有找到，返回的是nil
function get(name)

-- 删除名叫name的logger对象
function drop(name)

-- 删除所有的logger对象，一般是在退出时调用
function drop_all()

-- 设置所以logger对象的日志记录格式
function set_pattern()

-- 设置所有logger对象日志记录的level，低于这个level的日志将不被记录，
function set_level(level)
```

### 各种logger对象说明

共有7个logger对象，功能各有不同：

1. basic_logger对象

最简单的日志对象，就是在一个文件中记录日志，随着日志的记录，文件的大小会一直增加。

2. rotating_logger对象

在几个文件中循环记录日志的对象，文件的大小和个数可以在创建对象时设置。

3. daily_logger对象

每天新件一个日志文件，并在文件中记录日志。新建文件的时间可以在创建对象时设置。

4. stdout_logger对象

在标准输出设备中记录日志。

5. stderr_logger对象

在标准错误输出设备中记录日志。

6. stdout_color对象

在彩色标准输出设备中，以不同的颜色记录不同级别的日志。

7. stderr_color对象

在彩色错误输出设备中，以不同的颜色记录不同级别的日志。

### 对象方法说明

每种日志对象的方法都是相同的，在此统一对每个方法进行说明
```lua
-- 记录info级别的日志信息，msg是一个标准的字符串，可以使用string.format函数生成。
function info(msg)

-- 记录warn级别的日志信息，msg是一个标准的字符串，可以使用string.format函数生成。
function warn(msg)

-- 记录error级别的日志信息，msg是一个标准的字符串，可以使用string.format函数生成。
function error(msg)

-- 记录critical级别的日志信息，msg是一个标准的字符串，可以使用string.format函数生成。
function critical(msg)

-- 查询参数level级别的日志是否会被记录
function should_log(level)

-- 设置logger对象的日志记录的最低级别
function set_level(level)

-- 查询logger对象的日志记录的最低级别
function level()

-- 查询logger对象的名称
function name()

-- 设置logger对象的日志记录格式
function set_pattern(pattern)

-- 设置level级别的日志被记录后，马上刷新到输出文件或设备
function flush_on(level)
```

### 日志级别的说明

在spdlog中，日志的级别定义如下：
```cpp
typedef enum
{
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5,
    off = 6
} level_enum;
```

trace和debug级别，是c++程序在调试跟踪时使用的，在spdlogger4Lua中，不使用这两个级别。

其他的四种级别分别对应logger对象中的info, warn, error, critical分别记录的日志级别。

如果不想记录任何日志，可以设置level为off 。

### 日志格式的说明

set_pattern中的patter参数，实际就是一个格式化字符串，参考如下：
```lua
对对象设置pattern:
mylogger:set_pattern(">>>>>>>>> %H:%M:%S %z %v <<<<<<<<<");

对所有logger对象设置日志格式：
spdlogger.set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
```
格式化的标志可以参考下表：

flag	  | meaning																			| xample
----------|---------------------------------------------------------------------------------|----------------------------
%v	      | The actual text to log															| some user text"
%t	      | Thread id																		| 1232"
%P	      | Process id																		| 3456"
%n	      | Logger's name																	| some logger name"
%l	      | The log level of the message "debug", "info", etc                               |
%L	      | Short log level of the message													| D", "I", etc
%a	      | Abbreviated weekday name														| Thu"
%A	      | Full weekday name																| Thursday"
%b	      | Abbreviated month name															| Aug"
%B	      | Full month name																	| August"
%c	      | Date and time representation													| Thu Aug 23 15:35:46 2014"
%C	      | Year in 2 digits																| 14"
%Y	      | Year in 4 digits																| 2014"
%D or %x  | hort MM/DD/YY date																| 08/23/14"
%m	      | Month 1-12																		| 11"
%d	      | Day of month 1-31																| 29"
%H	      | Hours in 24 format 0-23															| 23"
%I	      | Hours in 12 format 1-12															| 11"
%M	      | Minutes 0-59																	| 59"
%S	      | Seconds 0-59																	| 58"
%e	      | Millisecond part of the current second 0-999									| 678"
%f	      | Microsecond part of the current second 0-999999									| 056789"
%F	      | Nanosecond part of the current second 0-999999999								| 256789123"
%p	      | AM/PM																			| AM"
%r	      | 12 hour clock																	| 02:55:02 pm"
%R	      | 24-hour HH:MM time, equivalent to %H:%M											| 23:55"
%T or %X  | ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S							| 23:55:59"
%z	      | ISO 8601 offset from UTC in timezone ([+/-]HH:MM)								| +02:00"
%i	      | Message sequence number (disabled by default - edit 'tweakme.h' to enable)		| 1154"
%%	      | The % sign																		| %"
%+	      | spdlog's default format															| [2014-31-10 23:46:59.678] [mylogger] [info] Some message"

### 使用举例

使用qt_creator编译后，可得到libspdlogger4Lua.so动态库，在lua中引入spdlogger4Lua库。

```lua
require("libspdlogger4Lua")
```

然后就可以记录日志了，如下：
```lua
l1 = spdlogger.basic_logger("b_loger", "b_loger.txt")
l1:set_level(5)

l2 = spdlogger.stderr_color("err_color")
l2:set_level(4)

l3 = spdlogger.daily_logger("daily_logger", "d_logger", 0, 0)
l4 = spdlogger.rotating_logger("rotating_logger", "r_logger", 3 * 1024, 3)

l1:error("this is a error message")
l2:warn("this is a warn message")
l3:info(string.format("%s, %d, %f", "info", 2, 3.3))
l4:critical(string.format("%s, %d, %f", "critical", 8, 6.3))
```


