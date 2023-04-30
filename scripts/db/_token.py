from datetime import datetime
import sqlite3
import sys
import traceback
import yaml

def readfile(filepath: str):
  with open("etc/tokens.yaml", "r") as fd:
    try:
      data = yaml.safe_load(fd)
    except yaml.YAMLError as exc:
      print(exc)
  return data

def main() -> int:

  connection = sqlite3.connect('tmp/db/syntax.sqlite3')
  query: str = 'insert into token (id,repr,type,data,size,created,modified) values (?,?,?,?,?,?,?)'

  _tokens = readfile("etc/tokens.yaml")
  if "tokens" not in _tokens:
    raise Exception("tokens does not contain tokens parent element")

  for _token in _tokens["tokens"]:

    for target_key in ["repr","type","data","size"]:
      if target_key not in _token:
        raise Exception(f"token does not contain {target_key} parameter")

    current_timestamp: datetime = datetime.utcnow()
    data: tuple = (None, _token["repr"], _token["type"], _token["data"], _token["size"], current_timestamp, current_timestamp,)

    try:
      cursor = connection.cursor()
      cursor.execute(query, data)
      connection.commit()
      cursor.close()
    except sqlite3.Error as err:
      print('SQLite error: %s' % (' '.join(err.args)))
      print("Exception class is: ", err.__class__)
      print('SQLite traceback: ')
      exc_type, exc_value, exc_tb = sys.exc_info()
      print(traceback.format_exception(exc_type, exc_value, exc_tb))

  if connection:
    connection.close()

  return 0

if __name__ == "__main__":
  sys.exit(main())
