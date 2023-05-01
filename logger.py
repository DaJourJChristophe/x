from datetime import datetime
import sys

"""
Log format:
[TYPE] [DATESTAMP] — [MODULE].[CLASS].[METHOD] — [MESSAGE].
"""

def datestr() -> str:
  months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec']
  days = [ "Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun" ]
  date: datetime = datetime.now()
  day = "0" + str(date.day) if date.day < 10 else date.day
  hour = "0" + str(date.hour) if date.hour < 10 else date.hour
  minute = "0" + str(date.minute) if date.minute < 10 else date.minute
  second = "0" + str(date.second) if date.second < 10 else date.second
  return f"{days[date.weekday()]}, {months[date.month - 1]} {day}, {date.year} {hour}:{minute}:{second}.{date.microsecond} {date.astimezone().tzinfo}"

NORMAL = 0
BOLD = 1
OPACITY = 2
ITALIC = 3
UNDERLINE = 4

RED = 31
GREEN = 32
YELLOW = 33
BLUE = 34
PURPLE = 35
TURQUOISE = 36
WHITE = 37
GREY = 39

BACKGROUND_RED = 41
BACKGROUND_GREEN = 42
BACKGROUND_YELLOW = 43
BACKGROUND_BLUE = 44
BACKGROUND_PURPLE = 45
BACKGROUND_TURQUOISE = 46
BACKGROUND_WHITE = 47
BACKGROUND_GREY = 49

def colorize(style: int, color: int, background: int) -> str:
  if background:
    return f"\033[{style};{color};{background}m"
  return f"\033[{style};{color}m"

def color_reset() -> str:
  return "\033[0;0m"

def build_prefix(tag: str, tagstyle: int, tagcolor: int, tagbackground: int) -> str:
  return f"{colorize(NORMAL,GREY,None)}[{color_reset()}{colorize(tagstyle,tagcolor,tagbackground)}{tag}{color_reset()}{colorize(NORMAL,GREY,None)}]{color_reset()}"

def log(tag: str, tagstyle: int, tagcolor: int, tagbackground: int, priclr: str, file: str, line: str, ch: str, mod: str, cls: str, func:str, msg: str, msgclr: str, desc: str) -> None:
  print(build_prefix(tag, tagstyle, tagcolor, tagbackground) + f" {priclr}{datestr()} — \033[4;39m{file}\033[0;0m{priclr} — \033[4;39m{mod}.{cls}.{func}\033[0;0m{priclr} — \033[4;39mLn: {line}, Ch: {ch}\033[0;0m \033[0;39m—\033[0;0m {msgclr}{msg}\033[0;0m\033[0;39m —\033[0;0m {msgclr}{desc}\033[0;0m\033[0;39m.\033[0;0m")

def log_info(file: str, line: str, ch: str, mod: str, cls: str, func: str, msg: str, desc: str) -> None:
  log(" info", NORMAL, GREY, None, "\033[0;39m", file, line, ch, mod, cls, func, msg, "\033[0;39m", desc)

def log_debug(file: str, line: str, ch: str, mod: str, cls: str, func: str, msg: str, desc: str) -> None:
  log("debug", NORMAL, BLUE, None, "\033[0;34m", file, line, ch, mod, cls, func, msg, "\033[0;34m", desc)

def log_warn(file: str, line: str, ch: str, mod: str, cls: str, func: str, msg: str, desc: str) -> None:
  log(" warn", NORMAL, YELLOW, None, "\033[0;33m", file, line, ch, mod, cls, func, msg, "\033[0;33m", desc)

def log_scream(file: str, line: str, ch: str, mod: str, cls: str, func: str, msg: str, desc: str) -> None:
  log("error", BOLD, RED, BACKGROUND_RED, "\033[1;31;41m", file, line, ch, mod, cls, func, msg, "\033[1;31;41m", desc)

def log_error(file: str, line: str, ch: str, mod: str, cls: str, func: str, msg: str, desc: str) -> None:
  log("error", NORMAL, RED, None, "\033[0;31m", file, line, ch, mod, cls, func, msg, "\033[0;31m", desc)

def main() -> int:

  log_info("logger.py", "12", "36", "Logger", "Logger", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")

  log_debug("logger.py", "12", "36", "Logger", "Printer", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")

  log_warn("logger.py", "12", "36", "Logger", "Logger", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")
  log_warn("logger.py", "12", "36", "Logger", "Logger", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")

  log_error("logger.py", "12", "36", "Logger", "Logger", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")
  log_scream("logger.py", "12", "36", "Logger", "Logger", "print", "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module")

  return 0

if __name__ == "__main__":
  sys.exit(main())

