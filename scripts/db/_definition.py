from datetime import datetime
import sqlite3
import sys
import traceback
import yaml

def readfile(filepath: str):
  with open(filepath, "r") as fd:
    try:
      data = yaml.safe_load(fd)
    except yaml.YAMLError as exc:
      print(exc)
  return data

def main() -> int:

  connection = sqlite3.connect('tmp/db/syntax.sqlite3')
  query: str = 'insert into definition (id,name,value,created,modified) values (?,?,?,?,?)'

  _definitions = readfile("etc/definitions.yaml")
  if "definitions" not in _definitions:
    raise Exception("definitions does not contain definitions parent element")

  for _definition in _definitions["definitions"]:

    for target_key in ["name","value"]:
      if target_key not in _definition:
        raise Exception(f"token does not contain {target_key} parameter")

    current_timestamp: datetime = datetime.utcnow()
    data: tuple = (None, _definition["name"], 0, current_timestamp, current_timestamp,)

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
