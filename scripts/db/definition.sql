/**
 *
 */
create table if not exists `definition` (
  [id] integer primary key autoincrement,
  [name] varchar(64) not null default "",
  [value] integer not null default 0,
  [created] timestamp null default current_timestamp,
  [modified] timestamp null default current_timestamp
);
