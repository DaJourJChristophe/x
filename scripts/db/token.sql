/**
 *
 */
create table if not exists `token` (
  [id] integer primary key autoincrement,
  [repr] varchar(64) not null default "",
  [type] integer not null default 0,
  [data] varchar(64) null default null,
  [size] integer not null default 0,
  [created] timestamp null default current_timestamp,
  [modified] timestamp null default current_timestamp
);
